#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#include "TLVEncoder.h"
#include "TLVDecoder.h"
#include "EAPMessage.h"

/*
 *
 oct1           EAPMessage IEI
 oct2-3         Length of EAPMessage
 oct 4
 oct 1503
 *
 */
int decode_EAP_Message(EAPMessage *eapmessage, uint8_t iei, uint8_t *buffer, uint32_t len)
{
  int                                     decoded = 0;
  uint8_t                                 ielen = 0;
  int                                     decode_result;

  if (iei > 0) {
    CHECK_IEI_DECODER (iei, *buffer);//buffer 的首个byte带便iei, ts24.501
    //看看iei和 buffer的首个byte一样否
    decoded++;//偏低地址加1
  }

  ielen = *((uint16_t *)(buffer+ decoded ));//ie的长度
  decoded+=2;
  CHECK_LENGTH_DECODER (len - decoded, ielen);//看看剩下的长度,更ielen一不一样

  //把ie取出来,把IEI/Len of IE都丢弃了
  if ((decode_result = decode_bstring (eapmessage, ielen, buffer + decoded, len - decoded)) < 0)//目的地址,目的长度,长度,源地址,源长度,返回值为目的地址被赋予的实际长度
    return decode_result;
  else
    decoded += decode_result;

  return decoded;
}

int encode_EAP_Message(EAPMessage eapmessage, uint8_t iei, uint8_t *buffer, uint32_t len)
{
  uint8_t                                *lenPtr;
  uint32_t                                encoded = 0;
  int                                     encode_result;

  /*
   * Checking IEI and pointer
   */
  CHECK_PDU_POINTER_AND_LENGTH_ENCODER (buffer, EAP_Message_MINIMUM_LENGTH, len);

  if (iei > 0) {
    *buffer = iei;
    encoded++;
  }

  lenPtr = (buffer + encoded);//指向len的地方
  encoded+=2;

  if ((encode_result = encode_bstring (eapmessage, buffer + encoded, len - encoded)) < 0)//加密,实体,首地址,长度
    return encode_result;
  else
    encoded += encode_result;

  *((uint16_t*)lenPtr)= encoded - 2 - ((iei > 0) ? 1 : 0);//对len赋值
  return encoded;
}
