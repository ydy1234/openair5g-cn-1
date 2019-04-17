#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>

#include "TLVEncoder.h"
#include "TLVDecoder.h"
#include "N1ModeToS1ModeNASTransparentContainer.h"

int encode_n1_mode_to_s1_mode_nas_transparent_container ( N1ModeToS1ModeNASTransparentContainer n1modetos1modenastransparentcontainer, uint8_t iei, uint8_t * buffer, uint32_t len  )
{
    uint8_t *lenPtr;
    uint32_t encoded = 0;
    int encode_result;
    CHECK_PDU_POINTER_AND_LENGTH_ENCODER (buffer,N1_MODE_TO_S1_MODE_NAS_TRANSPARENT_CONTAINER_MINIMUM_LENGTH , len);


    if( iei >0  )
    {
        *buffer=iei;
        encoded++;
    }





    if ((encode_result = encode_bstring (n1modetos1modenastransparentcontainer, buffer + encoded, len - encoded)) < 0)//加密,实体,首地址,长度
        return encode_result;
    else
        encoded += encode_result;


    return encoded;}

int decode_n1_mode_to_s1_mode_nas_transparent_container ( N1ModeToS1ModeNASTransparentContainer * n1modetos1modenastransparentcontainer, uint8_t iei, uint8_t * buffer, uint32_t len  )
{
	int decoded=0;
	uint8_t ielen=0;
	int decode_result;

    if (iei > 0)
    {
        CHECK_IEI_DECODER (iei, *buffer);
        decoded++;
    }




    if((decode_result = decode_bstring (n1modetos1modenastransparentcontainer, ielen, buffer + decoded, len - decoded)) < 0)
        return decode_result;
    else
        decoded += decode_result;
    return decoded;
}

