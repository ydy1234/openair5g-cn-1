#include "ngap_amf_itti_messaging.h"


int ngap_amf_itti_nas_uplink_data_ind(const amf_ue_ngap_id_t ue_id,
                                      STOLEN_REF bstring *payload,
                                      const tai_t      const* tai,
                                      const cgi_t     const* cgi)
{
  MessageDef                             *message_p = NULL;
  
  message_p = itti_alloc_new_message (TASK_NGAP, NGAP_DATA_IND);
  NGAP_UL_DATA_IND (message_p).ue_id          = ue_id;
  NGAP_UL_DATA_IND (message_p).nas_msg        = *payload;
  *payload = NULL;
  NGAP_UL_DATA_IND (message_p).tai            = *tai;
  NGAP_UL_DATA_IND (message_p).cgi            = *cgi;

  return itti_send_msg_to_task (TASK_NAS_AMF, INSTANCE_DEFAULT, message_p);
}

int ngap_amf_itti_send_sctp_request(
  STOLEN_REF bstring *payload,
  const sctp_assoc_id_t assoc_id,
  const sctp_stream_id_t stream,
  const amf_ue_ngap_id_t ue_id)
{
  MessageDef                             *message_p = NULL;

  message_p = itti_alloc_new_message (TASK_NGAP, SCTP_DATA_REQ);
  SCTP_DATA_REQ (message_p).payload = *payload;
  *payload = NULL;
  SCTP_DATA_REQ (message_p).assoc_id = assoc_id;
  SCTP_DATA_REQ (message_p).stream = stream;
  SCTP_DATA_REQ (message_p).amf_ue_ngap_id = ue_id;
  return itti_send_msg_to_task (TASK_SCTP, INSTANCE_DEFAULT, message_p);
}
