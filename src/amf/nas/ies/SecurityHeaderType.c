#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#include "TLVEncoder.h"
#include "TLVDecoder.h"
#include "SecurityHeaderType.h"



//SecurityHeaderType 只占4个bit,另外4个bit为空,但字节仍按照1字节处理

int encode_security_header_type(SecurityHeaderType securityheadertype, uint8_t iei, uint8_t *buffer, uint32_t len)
{
    //iei useless

    uint32_t                                encoded = 0;
    int                                     encode_result;

    /*
     * Checking IEI and pointer
     */
    //检查buffer是否为NULL,len是否比AUTHENTICATION_FAILURE_PARAMETER_MINIMUM_LENGTH短,并报错的宏
    CHECK_PDU_POINTER_AND_LENGTH_ENCODER (buffer, SECURITY_HEADER_TYPE_MINIMUM_LENGTH, len);



    if ((encode_result = encode_bstring (securityheadertype, buffer + encoded, len - encoded)) < 0)//加密,实体,首地址,长度
        return encode_result;
    else
        encoded+=encode_result;

    return encoded;
}
int decode_security_header_type(SecurityHeaderType *securityheadertype, uint8_t iei, uint8_t *buffer, uint32_t len)
{
  int                                     decoded = 0;
  int                                     decode_result;

  if ((decode_result = decode_bstring (securityheadertype, 1, buffer + decoded, len - decoded)) < 0)//目的地址,目的长度,长度,源地址,源长度,返回值为目的地址被赋予的实际长度
    return decode_result;
  else
    decoded += decode_result;

  return decoded;
}
