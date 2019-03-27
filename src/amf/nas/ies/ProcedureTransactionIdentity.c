
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#include "TLVEncoder.h"
#include "TLVDecoder.h"
#include "ProcedureTransactionIdentity.h"

int encode_procedure_transaction_identity(ProcedureTransactionIdentity proceduretransactionidentity, uint8_t *buffer, uint32_t len);
{
    //iei useless

    uint32_t        encoded=0;


    CHECK_PDU_POINTER_AND_LENGTH_ENCODER (buffer, PROCEDURE_TRANSACTION_IDENTITY_MINIMUM_LENGTH, len);

    //iei uselesss

    if((encode_result = encode_bstring(extendedprotocoldiscriminator, buffer, len))<0))//目的变量, 源指针, len为指针指向地址的长度
        return encode_result;
    else
        encoded++;
    return encoded;

}
int decode_procedure_transaction_identity(ProcedureTransactionIdentity *proceduretransactionidentity, uint8_t iei, uint8_t *buffer, uint32_t len);
{
    int decoded=0;
    int decode_result;

    if(len<0) return decode_result;//buffer to short

    //把ie取出来,把IEI/Len of IE都丢弃了
    if ((decode_result = decode_bstring (extendedprotocoldiscriminator, len, buffer, len)) < 0)//目的地址,目的长度,长度,源地址,源长度,返回值为目的地址被赋予的实际长度
        return decode_result;
    else
        decoded++;
    return decoded;
}
