#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "dynamic_memory_check.h"
#include "intertask_interface.h"
#include "assertions.h"
#include "intertask_interface_types.h"
#include "common_defs.h"
#include "log.h"
#include "ngap_ies_defs.h"
#include "asn_internal.h"
#include "hashtable.h"
#include "ngap_amf.h"


uint32_t                                nb_gnb_associated = 0;
hash_table_ts_t g_ngap_gnb_coll = {.mutex = PTHREAD_MUTEX_INITIALIZER, 0};// contains gNB_description_s, key is gNB_description_s.gnb_id (uint32_t);
hash_table_ts_t g_ngap_amf_id2assoc_id_coll = {.mutex = PTHREAD_MUTEX_INITIALIZER, 0};// contains sctp association id, key is amf_ue_ngap_id;



//TODO: get config info from a configuration file
static int ngap_send_init_sctp(void)
{
  MessageDef                             *message_p = NULL;
  message_p = itti_alloc_new_message (TASK_NGAP, SCTP_INIT_MSG);
  message_p->ittiMsg.sctpInit.port = 36412;
  message_p->ittiMsg.sctpInit.ppid = 60;
  message_p->ittiMsg.sctpInit.ipv4 = 1;
  message_p->ittiMsg.sctpInit.ipv6 = 0;
  message_p->ittiMsg.sctpInit.nb_ipv4_addr = 1;
  message_p->ittiMsg.sctpInit.ipv4_address[0] = 0; //1684303882;//1546416138;//117506058;
  message_p->ittiMsg.sctpInit.nb_ipv6_addr = 0;
  message_p->ittiMsg.sctpInit.ipv6_address[0] = "0:0:0:0:0:0:0:1";
  return itti_send_msg_to_task (TASK_SCTP, INSTANCE_DEFAULT, message_p);
}

void
ngap_amf_thread (
  __attribute__((unused))void * args)
{
    itti_mark_task_ready (TASK_NGAP);
    OAILOG_START_USE();
    while(1) {
      MessageDef * received_message_p = NULL;
      MessagesIds   message_id = MESSAGES_ID_MAX;
      itti_receive_msg (TASK_NGAP , &received_message_p);
      DevAssert (received_message_p != NULL);
      switch(ITTI_MSG_ID (received_message_p)){
        case MESSAGE_TEST:{
            OAILOG_DEBUG(LOG_S1AP,"NGAP TEST MSG\n");
          }
          break;
        case ACTIVATE_MESSAGE:{
        	 OAILOG_DEBUG(LOG_S1AP,"ACTIVATE MESSAGE\n");
            if (ngap_send_init_sctp () < 0) {
              OAILOG_CRITICAL (LOG_S1AP, "Error while sending SCTP_INIT_MSG to SCTP\n");
            }
          }
          break;
        case SCTP_NEW_ASSOCIATION:{
            OAILOG_DEBUG(LOG_S1AP,"SCTP_NEW_ASSOCIATION\n");
            ngap_handle_new_association(&received_message_p->ittiMsg.sctp_new_peer);
          }
          break;
        case SCTP_DATA_IND:{
            OAILOG_DEBUG(LOG_S1AP,"SCTP_DATA_IND(recv N2 Messages from AN)\n");
            ngap_message  message = {0};

            if (ngap_amf_decode_pdu (&message, SCTP_DATA_IND (received_message_p).payload, &message_id) < 0) {
              OAILOG_ERROR (LOG_S1AP, "Failed to decode new buffer\n");
            } else {
              ngap_amf_handle_message (SCTP_DATA_IND (received_message_p).assoc_id,
                                   SCTP_DATA_IND (received_message_p).stream, &message);
            }
            if (message_id != MESSAGES_ID_MAX) {
              ngap_free_amf_decode_pdu(&message, message_id);
            }
            bdestroy (SCTP_DATA_IND (received_message_p).payload);

          }
          break;
      }
    itti_free (ITTI_MSG_ORIGIN_ID (received_message_p), received_message_p);
    received_message_p = NULL;
    }
    return NULL;
}

int 
ngap_amf_init(void)
{  
    OAILOG_DEBUG (LOG_S1AP, "Initializing NGAP interface\n");
    //if (get_asn1c_environment_version () < ASN1_MINIMUM_VERSION) {
      //OAILOG_ERROR (LOG_S1AP, "ASN1C version %d fount, expecting at least %d\n", get_asn1c_environment_version (), ASN1_MINIMUM_VERSION);
    //  return RETURNerror;
    //} else {
      //OAILOG_DEBUG (LOG_S1AP, "ASN1C version %d\n", get_asn1c_environment_version ());
    //}
  
    //OAILOG_DEBUG (LOG_S1AP, "S1AP Release v10.5\n");

    bstring bs1 = bfromcstr("ngap_gNB_coll");
    hash_table_ts_t * h = hashtable_ts_init(&g_ngap_gnb_coll,16,NULL,free_wrapper,bs1);
    bdestroy(bs1);
    if(!h) return RETURNerror;

    bstring bs2 = bfromcstr("ngap_amf_id2assoc_id_coll");
    h = hashtable_ts_init(&g_ngap_amf_id2assoc_id_coll,16,NULL,hash_free_int_func,bs2);
    bdestroy(bs2);
    if(!h) return RETURNerror;

    if (itti_create_task (TASK_NGAP, &ngap_amf_thread, NULL) < 0) {
      OAILOG_ERROR (LOG_S1AP, "Error while creating NGAP task\n");
      return RETURNerror;
    }
/*
    if (ngap_send_init_sctp () < 0) {
      OAILOG_ERROR (LOG_S1AP, "Error while sendind SCTP_INIT_MSG to SCTP \n");
      return RETURNerror;
    }
*/
    OAILOG_DEBUG (LOG_S1AP, "Initializing NGAP interface: DONE\n");
    return RETURNok;
}


gnb_description_t                      *
ngap_is_gnb_assoc_id_in_list (
  const sctp_assoc_id_t sctp_assoc_id)
{   
  gnb_description_t                      *gnb_ref = NULL;
  hashtable_ts_get(&g_ngap_gnb_coll, (const hash_key_t)sctp_assoc_id, (void**)&gnb_ref);
  return gnb_ref;
}

gnb_description_t                      *
ngap_new_gnb (
  void)
{   
  gnb_description_t                      *gnb_ref = NULL;
    
  gnb_ref = calloc (1, sizeof (gnb_description_t));
  /*
   * Something bad happened during malloc...
   * * * * May be we are running out of memory.
   * * * * TODO: Notify eNB with a cause like Hardware Failure.
   */
  DevAssert (gnb_ref != NULL);
  // Update number of eNB associated
  nb_gnb_associated++;
  bstring bs = bfromcstr("ngap_ue_coll");
  hashtable_ts_init(&gnb_ref->ue_coll,16, NULL, free_wrapper, bs);
  bdestroy(bs);
  gnb_ref->nb_ue_associated = 0;
  return gnb_ref;
}

ue_description_t                       *
ngap_is_ue_gnb_id_in_list (
  gnb_description_t * gnb_ref,              
  const ran_ue_ngap_id_t ran_ue_ngap_id)    
{   
  ue_description_t                       *ue_ref = NULL;
  hashtable_ts_get ((hash_table_ts_t * const)&gnb_ref->ue_coll, (const hash_key_t)ran_ue_ngap_id, (void **)&ue_ref);
  return ue_ref;
}

ue_description_t                       *
ngap_new_ue (
  const sctp_assoc_id_t sctp_assoc_id, ran_ue_ngap_id_t ran_ue_ngap_id)
{     
  gnb_description_t                      *gnb_ref = NULL;
  ue_description_t                       *ue_ref = NULL;

  gnb_ref = ngap_is_gnb_assoc_id_in_list (sctp_assoc_id);
  DevAssert (gnb_ref != NULL);
  ue_ref = calloc (1, sizeof (ue_description_t));
  /*
   * Something bad happened during malloc...
   * * * * May be we are running out of memory.
   * * * * TODO: Notify eNB with a cause like Hardware Failure.
   */
  DevAssert (ue_ref != NULL);
  ue_ref->gnb = gnb_ref;
  ue_ref->ran_ue_ngap_id = ran_ue_ngap_id;
    
  hashtable_rc_t  hashrc = hashtable_ts_insert (&gnb_ref->ue_coll, (const hash_key_t) ran_ue_ngap_id, (void *)ue_ref);
  if (HASH_TABLE_OK != hashrc) {
    OAILOG_ERROR(LOG_S1AP, "Could not insert UE descr in ue_coll: %s\n", hashtable_rc_code2string(hashrc));
    free_wrapper((void**) &ue_ref);
    return NULL;
  }
  // Increment number of UE
  gnb_ref->nb_ue_associated++;
  return ue_ref;
}   
