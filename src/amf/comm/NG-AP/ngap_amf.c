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


static int ngap_send_init_sctp(void)
{
  MessageDef                             *message_p = NULL;
  message_p = itti_alloc_new_message (TASK_NGAP, SCTP_INIT_MSG);
  message_p->ittiMsg.sctpInit.port = 36412;
  message_p->ittiMsg.sctpInit.ppid = 60;
  message_p->ittiMsg.sctpInit.ipv4 = 1;
  message_p->ittiMsg.sctpInit.ipv6 = 0;
  message_p->ittiMsg.sctpInit.nb_ipv4_addr = 1;
  message_p->ittiMsg.sctpInit.ipv4_address[0] =1684303882;//1546416138;//117506058;
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
//    printf("Initializing NGAP interface\n");
    //if (get_asn1c_environment_version () < ASN1_MINIMUM_VERSION) {
      //OAILOG_ERROR (LOG_S1AP, "ASN1C version %d fount, expecting at least %d\n", get_asn1c_environment_version (), ASN1_MINIMUM_VERSION);
    //  return RETURNerror;
    //} else {
      //OAILOG_DEBUG (LOG_S1AP, "ASN1C version %d\n", get_asn1c_environment_version ());
    //}
  
    //OAILOG_DEBUG (LOG_S1AP, "S1AP Release v10.5\n");

    if (itti_create_task (TASK_NGAP, &ngap_amf_thread, NULL) < 0) {
      OAILOG_ERROR (LOG_S1AP, "Error while creating NGAP task\n");
      return RETURNerror;
    }

    if (ngap_send_init_sctp () < 0) {
      OAILOG_ERROR (LOG_S1AP, "Error while sendind SCTP_INIT_MSG to SCTP \n");
      return RETURNerror;
    }

    OAILOG_DEBUG (LOG_S1AP, "Initializing NGAP interface: DONE\n");
    return RETURNok;
}

