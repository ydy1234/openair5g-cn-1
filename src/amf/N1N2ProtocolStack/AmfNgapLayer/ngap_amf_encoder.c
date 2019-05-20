#include "intertask_interface.h"

#include "ngap_common.h"
#include "ngap_amf_encoder.h"
#include "assertions.h"

#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
#include <pthread.h>

#include "bstrlib.h"

#include "intertask_interface.h"
#include "log.h"

int
ngap_amf_encode_pdu (
  Ngap_NGAP_PDU_t * pdu,
  uint8_t ** buffer,
  uint32_t * length)
{
  asn_encode_to_new_buffer_result_t res = { NULL, {0, NULL, NULL} };

  DevAssert (pdu != NULL);
  DevAssert (buffer != NULL);
  DevAssert (length != NULL);

  res = asn_encode_to_new_buffer(NULL, ATS_ALIGNED_CANONICAL_PER, &asn_DEF_Ngap_NGAP_PDU, pdu);
  if (res.result.encoded > 0) {
    *buffer = res.buffer;
    *length = res.result.encoded;
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_Ngap_NGAP_PDU, pdu);
    return 0;
  }

  return -1;
}


