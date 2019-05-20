#include <stdint.h>
#include "ngap_common.h"

#ifndef FILE_NGAP_AMF_ENCODER_SEEN
#define FILE_NGAP_AMF_ENCODER_SEEN

int ngap_amf_encode_pdu(Ngap_NGAP_PDU_t * pdu, uint8_t **buffer, uint32_t *len)
__attribute__ ((warn_unused_result));

#endif
