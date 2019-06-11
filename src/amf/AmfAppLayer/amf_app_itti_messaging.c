#include "amf_app_itti_messaging.h"

int amf_app_itti_send_ngap_dl_nas_transport_request(amf_ue_ngap_id_t  amf_ue_ngap_id, ran_ue_ngap_id_t  ran_ue_ngap_id, bstring nas_msg)
{
  MessageDef                             *message_p    = NULL;
  message_p = itti_alloc_new_message (TASK_AMF_APP, NGAP_NAS_DL_DATA_REQ);
  memset ((void *)&message_p->ittiMsg.ngap_nas_dl_data_req, 0, sizeof (itti_ngap_nas_dl_data_req_t));
  NGAP_NAS_DL_DATA_REQ(message_p).amf_ue_ngap_id = amf_ue_ngap_id;
  NGAP_NAS_DL_DATA_REQ(message_p).ran_ue_ngap_id = ran_ue_ngap_id;
  NGAP_NAS_DL_DATA_REQ(message_p).nas_msg = nas_msg;
  int rc = itti_send_msg_to_task (TASK_NGAP, INSTANCE_DEFAULT, message_p);
}
