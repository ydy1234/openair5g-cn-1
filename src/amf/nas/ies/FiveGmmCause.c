#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#include "TLVEncoder.h"
#include "TLVDecoder.h"
#include "FiveGmmCause.h"

int
decode_mm_cause (
  FiveGmmCause * mmcause,
  uint8_t iei,
  uint8_t * buffer,
  uint32_t len)
{
  int                                     decoded = 0;

  if (iei > 0) {
    CHECK_IEI_DECODER (iei, *buffer);
    decoded++;
  }

  *mmcause = *(buffer + decoded);
  decoded++;
  return decoded;
}

int
encode_mm_cause (
  FiveGmmCause * mmcause,
  uint8_t iei,
  uint8_t * buffer,
  uint32_t len)
{
  uint32_t                                encoded = 0;

  /*
   * Checking IEI and pointer
   */
  CHECK_PDU_POINTER_AND_LENGTH_ENCODER (buffer, FIVEGMM_CAUSE_MINIMUM_LENGTH, len);

  if (iei > 0) {
    *buffer = iei;
    encoded++;
  }

  *(buffer + encoded) = *mmcause;
  encoded++;
  return encoded;
}
