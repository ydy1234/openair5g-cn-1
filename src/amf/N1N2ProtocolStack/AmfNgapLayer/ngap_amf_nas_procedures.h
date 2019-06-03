#include <stdint.h>

#include "common_defs.h"
#include "bstrlib.h"
#include "intertask_interface.h"
#include "Ngap_NGAP-PDU.h"

int ngap_amf_handle_initial_ue_message(const sctp_assoc_id_t assocId,
                                       const sctp_stream_id_t stream,
                                       struct Ngap_NGAP_PDU_t *message);
