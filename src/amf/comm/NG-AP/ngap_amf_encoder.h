#include <stdint.h>

#ifndef FILE_NGAP_AMF_ENCODER_SEEN
#define FILE_NGAP_AMF_ENCODER_SEEN

int ngap_mme_encode_pdu(ngap_message *message, uint8_t **buffer, uint32_t *len)
__attribute__ ((warn_unused_result));

#endif
