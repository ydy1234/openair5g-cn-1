#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#include "TLVEncoder.h"
#include "TLVDecoder.h"
#include "NasKeySetIdentifier.h"



uint8_t
encode_u8_nas_key_set_identifier (
  NasKeySetIdentifier * naskeysetidentifier)
{
  uint8_t                                 bufferReturn;
  uint8_t                                *buffer = &bufferReturn;
  uint8_t                                 encoded = 0;
  uint8_t                                 iei = 0;
  
  *(buffer + encoded) = 0x00 | (iei & 0xf0) | ((naskeysetidentifier->tsc & 0x1) << 3) | (naskeysetidentifier->naskeysetidentifier & 0x7);
  encoded++;
  return bufferReturn;
}

int
decode_u8_nas_key_set_identifier (
  NasKeySetIdentifier * naskeysetidentifier,
  uint8_t iei,
  uint8_t value,
  uint32_t len)
{
  int                                     decoded = 0;
  uint8_t                                *buffer = &value;

  naskeysetidentifier->tsc = (*(buffer + decoded) >> 3) & 0x1;
  naskeysetidentifier->naskeysetidentifier = *(buffer + decoded) & 0x7;
  decoded++;
  return decoded;
}
