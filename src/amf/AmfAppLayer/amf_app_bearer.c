#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assertions.h"
#include "log.h"
#include "conversions.h"
#include "common_types.h"
#include "intertask_interface.h"
#include "timer.h"
#include "ngap_amf.h"


static void notify_ngap_new_ue_amf_ngap_id_association (amf_ue_ngap_id_t amf_ue_ngap_id,ran_ue_ngap_id_t ran_ue_ngap_id, sctp_assoc_id_t sctp_assoc_id)
{
  MessageDef                             *message_p = NULL;
  itti_amf_app_ngap_amf_ue_id_notification_t *notification_p = NULL;

  OAILOG_FUNC_IN (LOG_AMF_APP);
  message_p = itti_alloc_new_message (TASK_AMF_APP, AMF_APP_NGAP_AMF_UE_ID_NOTIFICATION);
  notification_p = &message_p->ittiMsg.amf_app_ngap_amf_ue_id_notification;
  memset (notification_p, 0, sizeof (itti_amf_app_ngap_amf_ue_id_notification_t));
  notification_p->ran_ue_ngap_id = ran_ue_ngap_id;
  notification_p->amf_ue_ngap_id = amf_ue_ngap_id;
  notification_p->sctp_assoc_id  = sctp_assoc_id;

  itti_send_msg_to_task (TASK_NGAP, INSTANCE_DEFAULT, message_p);
  OAILOG_DEBUG (LOG_AMF_APP, " Sent AMF_APP_NGAP_AMF_UE_ID_NOTIFICATION to NGAP for UE Id %u\n", notification_p->amf_ue_ngap_id);
  OAILOG_FUNC_OUT (LOG_AMF_APP);
}
