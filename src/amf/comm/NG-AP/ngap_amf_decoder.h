#ifndef FILE_S1AP_MME_DECODER_SEEN
#define FILE_S1AP_MME_DECODER_SEEN
#include "bstrlib.h"
//#include "ngap_common.h"
#include "ngap_ies_defs.h"

int ngap_amf_decode_pdu(ngap_message *message, const_bstring const raw, MessagesIds *messages_id) __attribute__ ((warn_unused_result));
int ngap_free_amf_decode_pdu(ngap_message *message, MessagesIds messages_id);

#endif
