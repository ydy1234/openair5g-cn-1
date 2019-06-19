#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "intertask_interface.h"
#include "timer.h"
#include "amf_app.h"
#include "assertions.h"
#include "log.h"
#include "common_defs.h"
#include "amf_config.h"
#include "amf_app_defs.h"

amf_app_desc_t                          amf_app_desc = {.rw_lock = PTHREAD_RWLOCK_INITIALIZER, 0} ;

void *amf_app_thread (
  void *args)
{
  //struct ue_context_s                    *ue_context_p = NULL;
  itti_mark_task_ready (TASK_AMF_APP);
  OAILOG_START_USE ();
  while (1) {
    MessageDef                             *received_message_p = NULL;

    /*
     * Trying to fetch a message from the message queue.
     * If the queue is empty, this function will block till a
     * message is sent to the task.
     */
    itti_receive_msg (TASK_AMF_APP, &received_message_p);
    DevAssert (received_message_p );

    switch (ITTI_MSG_ID (received_message_p)) {
      case AMF_APP_INITIAL_UE_MESSAGE:{
        amf_app_handle_initial_ue_message (&AMF_APP_INITIAL_UE_MESSAGE (received_message_p));
        }
        break;
      case TIMER_HAS_EXPIRED:{
        /*
         * Check statistic timer
         */
        if (received_message_p->ittiMsg.timer_has_expired.timer_id == amf_app_desc.statistic_timer_id) {
          amf_app_statistics_display ();
        } else if (received_message_p->ittiMsg.timer_has_expired.arg != NULL) {
          amf_ue_ngap_id_t amf_ue_ngap_id = *((amf_ue_ngap_id_t *)(received_message_p->ittiMsg.timer_has_expired.arg));
          //ue_context_p = amf_ue_context_exists_amf_ue_ngap_id (&amf_app_desc.amf_ue_contexts, amf_ue_ngap_id);
          //if (ue_context_p == NULL) {
          //  OAILOG_WARNING (LOG_MME_APP, "Timer expired but no assoicated UE context for UE id %d\n",amf_ue_ngap_id);
          //  break;
          //}
          //if (received_message_p->ittiMsg.timer_has_expired.timer_id == ue_context_p->mobile_reachability_timer.id) {
            // Mobile Reachability Timer expiry handler 
            //mme_app_handle_mobile_reachability_timer_expiry (ue_context_p);
          //} else if (received_message_p->ittiMsg.timer_has_expired.timer_id == ue_context_p->implicit_detach_timer.id) {
            // Implicit Detach Timer expiry handler 
            //mme_app_handle_implicit_detach_timer_expiry (ue_context_p);
          //} else if (received_message_p->ittiMsg.timer_has_expired.timer_id == ue_context_p->initial_context_setup_rsp_timer.id) {
            // Initial Context Setup Rsp Timer expiry handler
            //mme_app_handle_initial_context_setup_rsp_timer_expiry (ue_context_p);
          //} else {
          //  OAILOG_WARNING (LOG_MME_APP, "Timer expired but no assoicated timer_id for UE id %d\n",amf_ue_ngap_id);
          //}
        }
      }
      break;
    }
  }
}

int
amf_app_init (
  const amf_config_t * amf_config_p)
{
  OAILOG_FUNC_IN (LOG_AMF_APP);
  memset (&amf_app_desc, 0, sizeof (amf_app_desc));
  pthread_rwlock_init (&amf_app_desc.rw_lock, NULL);
/*
  bstring b = bfromcstr("amf_app_imsi_ue_context_htbl");
  amf_app_desc.amf_ue_contexts.imsi_ue_context_htbl = hashtable_ts_create (amf_config.max_ues, NULL, hash_free_int_func, b);
  btrunc(b, 0);
  bassigncstr(b, "amf_app_tun11_ue_context_htbl");
  amf_app_desc.amf_ue_contexts.tun11_ue_context_htbl = hashtable_ts_create (amf_config.max_ues, NULL, hash_free_int_func, b);
  AssertFatal(sizeof(uintptr_t) >= sizeof(uint64_t), "Problem with amf_ue_ngap_id_ue_context_htbl in MME_APP");
  btrunc(b, 0);
  bassigncstr(b, "amf_app_amf_ue_ngap_id_ue_context_htbl");
  amf_app_desc.amf_ue_contexts.amf_ue_ngap_id_ue_context_htbl = hashtable_ts_create (amf_config.max_ues, NULL, NULL, b);
  btrunc(b, 0);
  bassigncstr(b, "amf_app_enb_ue_ngap_id_ue_context_htbl");
  amf_app_desc.amf_ue_contexts.enb_ue_ngap_id_ue_context_htbl = hashtable_ts_create (amf_config.max_ues, NULL, hash_free_int_func, b);
  btrunc(b, 0);
  bassigncstr(b, "amf_app_guti_ue_context_htbl");
  amf_app_desc.amf_ue_contexts.guti_ue_context_htbl = obj_hashtable_ts_create (amf_config.max_ues, NULL, hash_free_int_func, hash_free_int_func, b);
  bdestroy(b);
*/
  /*
   * Create the thread associated with MME applicative layer
   */
  if (itti_create_task (TASK_AMF_APP, &amf_app_thread, NULL) < 0) {
    OAILOG_ERROR (LOG_AMF_APP, "MME APP create task failed\n");
    OAILOG_FUNC_RETURN (LOG_AMF_APP, RETURNerror);
  }

  amf_app_desc.statistic_timer_period = amf_config_p->amf_statistic_timer;
  /*
   * Request for periodic timer
   */

  if (timer_setup (amf_config_p->amf_statistic_timer, 0, TASK_AMF_APP, INSTANCE_DEFAULT, TIMER_PERIODIC, NULL, &amf_app_desc.statistic_timer_id) < 0) {
    OAILOG_ERROR (LOG_AMF_APP, "Failed to request new timer for statistics with %ds " "of periocidity\n", amf_config_p->amf_statistic_timer);
    amf_app_desc.statistic_timer_id = 0;
  }

  OAILOG_DEBUG (LOG_AMF_APP, "Initializing AMF applicative layer: DONE\n");
  OAILOG_FUNC_RETURN (LOG_AMF_APP, RETURNok);
}
