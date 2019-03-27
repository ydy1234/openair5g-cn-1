#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#include "TLVEncoder.h"
#include "TLVDecoder.h"
#include "MessageType.h"

int encode_message_type(MessageType *messagetype, uint8_t iei, uint8_t *buffer, uint32_t len)
{
  uint8_t                                *lenPtr;
  uint32_t                                encoded = 0;
  int                                     encode_result;

  /*
   * Checking IEI and pointer
   */
  //检查buffer是否为NULL,len是否比AUTHENTICATION_FAILURE_PARAMETER_MINIMUM_LENGTH短,并报错的宏
  CHECK_PDU_POINTER_AND_LENGTH_ENCODER (buffer, MESSAGE_TYPE_MINIMUM_LENGTH, len);

  if (iei > 0) {
    *buffer = iei;
    encoded++;
  }

  lenPtr = (buffer + encoded);//指向len的地方
  encoded++;

  if ((encode_result = encode_bstring (authenticationfailureparameter, buffer + encoded, len - encoded)) < 0)//加密,实体,首地址,长度
    return encode_result;
  else
    encoded += encode_result;

  *lenPtr = encoded - 1 - ((iei > 0) ? 1 : 0);//对len赋值
  return encoded;
}
int decode_message_type(MessageType *messagetype, uint8_t iei, uint8_t *buffer, uint32_t len)
{
  int                                     decoded = 0;
  int                                     decode_result;


  //把ie取出来,把IEI/Len of IE都丢弃了
  if ((decode_result = decode_bstring (authenticationfailureparameter, ielen, buffer + decoded, len - decoded)) < 0)//目的地址,目的长度,长度,源地址,源长度,返回值为目的地址被赋予的实际长度
    return decode_result;
  else
    decoded += decode_result;

  return decoded;
}
