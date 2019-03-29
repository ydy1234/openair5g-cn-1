#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#include "TLVEncoder.h"
#include "TLVDecoder.h"
#include "DNN.h"

/*
 *
 oct1           DNN IEI
 oct2           Length of DNN
 oct3           AdditionalInformation Elements

 oct 102
 *
 */
int decode_DNN(DNN *dnn, uint8_t iei, uint8_t *buffer, uint32_t len)
{
  int                                     decoded = 0;
  uint8_t                                 ielen = 0;
  int                                     decode_result;

  if (iei > 0) {
    CHECK_IEI_DECODER (iei, *buffer);//buffer 的首个byte带便iei, ts24.501
    //看看iei和 buffer的首个byte一样否
    decoded++;//偏低地址加1
  }

  ielen = *(buffer + decoded);//ie的长度
  decoded++;
  CHECK_LENGTH_DECODER (len - decoded, ielen);//看看剩下的长度,更ielen一不一样

  //把ie取出来,把IEI/Len of IE都丢弃了
  if ((decode_result = decode_bstring (dnn, ielen, buffer + decoded, len - decoded)) < 0)//目的地址,目的长度,长度,源地址,源长度,返回值为目的地址被赋予的实际长度
    return decode_result;
  else
    decoded += decode_result;

  return decoded;
}

int encode_DNN(DNN dnn, uint8_t iei, uint8_t *buffer, uint32_t len)
{
  uint8_t                                *lenPtr;
  uint32_t                                encoded = 0;
  int                                     encode_result;

  /*
   * Checking IEI and pointer
   */
  CHECK_PDU_POINTER_AND_LENGTH_ENCODER (buffer, DNN_MINIMUM_LENGTH, len);

  if (iei > 0) {
    *buffer = iei;
    encoded++;
  }

  lenPtr = (buffer + encoded);//指向len的地方
  encoded++;

  if ((encode_result = encode_bstring (dnn, buffer + encoded, len - encoded)) < 0)//加密,实体,首地址,长度
    return encode_result;
  else
    encoded += encode_result;

  *lenPtr = encoded - 1 - ((iei > 0) ? 1 : 0);//对len赋值
  return encoded;
}
