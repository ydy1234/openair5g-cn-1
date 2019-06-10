#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common_types.h"
#include "amf_app_defs.h" 

void amf_app_handle_initial_ue_message       (itti_amf_app_initial_ue_message_t * const conn_est_ind_pP){

#if 0
  MessageDef                             *message_p = NULL;
  message_p = itti_alloc_new_message (TASK_AMF_APP, NAS_INITIAL_UE_MESSAGE);
  message_p->ittiMsg.nas_initial_ue_message.nas.ue_id           = ue_context_p->mme_ue_s1ap_id;
  message_p->ittiMsg.nas_initial_ue_message.nas.tai             = initial_pP->tai;
  message_p->ittiMsg.nas_initial_ue_message.nas.cgi             = initial_pP->cgi;
  message_p->ittiMsg.nas_initial_ue_message.nas.as_cause        = initial_pP->as_cause;
  if (initial_pP->is_s_tmsi_valid) {
    message_p->ittiMsg.nas_initial_ue_message.nas.s_tmsi        = initial_pP->opt_s_tmsi;
  } else {
    message_p->ittiMsg.nas_initial_ue_message.nas.s_tmsi.mme_code = 0;
    message_p->ittiMsg.nas_initial_ue_message.nas.s_tmsi.m_tmsi   = INVALID_M_TMSI;
  }
  message_p->ittiMsg.nas_initial_ue_message.nas.initial_nas_msg   =  initial_pP->nas;
  memcpy (&message_p->ittiMsg.nas_initial_ue_message.transparent, (const void*)&initial_pP->transparent, sizeof (message_p->ittiMsg.nas_initial_ue_message.transparent));
  MSC_LOG_TX_MESSAGE (MSC_MMEAPP_MME, MSC_NAS_MME, NULL, 0, "0 NAS_INITIAL_UE_MESSAGE");
  itti_send_msg_to_task (TASK_NAS_MME, INSTANCE_DEFAULT, message_p);
#endif

}
