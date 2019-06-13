#include <stdint.h>

#include "common_defs.h"
#include "bstrlib.h"
#include "intertask_interface.h"
#include "Ngap_NGAP-PDU.h"

int ngap_amf_handle_initial_ue_message(const sctp_assoc_id_t assocId,
                                       const sctp_stream_id_t stream,
                                       struct Ngap_NGAP_PDU_t *message);

void
ngap_handle_amf_ue_id_notification (
  const itti_amf_app_ngap_amf_ue_id_notification_t * const notification_p);
