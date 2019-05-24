#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>

#include "TLVEncoder.h"
#include "TLVDecoder.h"
#include "NSSAIInclusionMode.h"

int encode_nssai_inclusion_mode ( NSSAIInclusionMode nssaiinclusionmode, uint8_t iei, uint8_t * buffer, uint32_t len  ) 
{
    uint32_t encoded = 0;
    CHECK_PDU_POINTER_AND_LENGTH_ENCODER (buffer,NSSAI_INCLUSION_MODE_MINIMUM_LENGTH , len);
    
    ENCODE_U8(buffer+encoded,((iei&0x0f)<<4)|0x00|(nssaiinclusionmode&0x03),encoded);

    return encoded;
}

int decode_nssai_inclusion_mode ( NSSAIInclusionMode * nssaiinclusionmode, uint8_t iei, uint8_t * buffer, uint32_t len  ) 
{
    int decoded=0;
    uint8_t bitStream = 0;
 
    DECODE_U8(buffer+decoded,bitStream,decoded);

   if(iei != ((bitStream&0xf0)>>4))
     return -1;
   *nssaiinclusionmode = bitStream&0x03;

    return decoded;
}

