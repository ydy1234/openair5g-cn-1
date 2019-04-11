#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


#include "TLVEncoder.h"
#include "TLVDecoder.h"
#include "AuthenticationRequest.h"

int
decode_authentication_request (    //把buffer中的内容解析到 authentication request中,len 是buffer的长度, 返回值是authentication_request的长度
  authentication_request_msg * authentication_request,
  uint8_t * buffer,
  uint32_t len)
{
  uint32_t                                decoded = 0;
  int                                     decoded_result = 0;

  // Check if we got a NULL pointer and if buffer length is >= minimum length expected for the message.
  CHECK_PDU_POINTER_AND_LENGTH_DECODER (buffer, AUTHENTICATION_REQUEST_MINIMUM_LENGTH, len);//AUTHENTICATION_REQUEST_MINIMUM_LENGTH 通过.h定义

  /*
   * Decoding mandatory fields
   */
  if((decoded_result = decode_protocol_discriminator(&authentication_request->extendedprotocoldiscriminator,0,buffer+decoded,len-decoded))<0)
      return decoded_result;
  else
      decoded+=decoded_result;

  /*int decode_security_header_type(SecurityHeaderType *securityheadertype, uint8_t iei, uint8_t *buffer, uint32_t len);*/
  if((decoded_result = decode_security_header_type(&authentication_request->securityheadertype, 0 , buffer+decoded, len-decoded))<0)
      return decoded_result;
  else
      return decoded+=decoded_result;

  if((decoded_result = decode_message_type(&authentication_request->messagetype, 0, buffer + decoded, len- decoded))<0)
      return decoded_result;
  else
      decoded+=decoded_result;




  if ((decoded_result = decode_u8_nas_key_set_identifier (&authentication_request->naskeysetidentifier, 0, *(buffer + decoded) >> 4, len - decoded)) < 0)
    return decoded_result;
  else
      decoded+=decoded_result;

  if ((decoded_result = decode_authentication_parameter_rand (&authentication_request->authenticationparameterrand, 0, buffer + decoded, len - decoded)) < 0)
    return decoded_result;
  else
    decoded += decoded_result;

  if ((decoded_result = decode_authentication_parameter_autn (&authentication_request->authenticationparameterautn, 0, buffer + decoded, len - decoded)) < 0)
    return decoded_result;
  else
    decoded += decoded_result;

  return decoded;
}

int
encode_authentication_request (
  authentication_request_msg * authentication_request,
  uint8_t * buffer,
  uint32_t len)
{
  int                                     encoded = 0;
  int                                     encoded_result = 0;

  /*
   * Checking IEI and pointer
   */
  CHECK_PDU_POINTER_AND_LENGTH_ENCODER (buffer, AUTHENTICATION_REQUEST_MINIMUM_LENGTH, len);

  if((encoded_result = encode_protocol_discriminator(authentication_request->extendedprotocoldiscriminator,0,buffer+encoded,len-encoded))<0)
      return encoded_result;
  else
      encoded+=encoded_result;

  /*int encode_security_header_type(SecurityHeaderType *securityheadertype, uint8_t iei, uint8_t *buffer, uint32_t len);*/
  if((encoded_result = encode_security_header_type(authentication_request->securityheadertype, 0 , buffer+encoded, len-encoded))<0)
      return encoded_result;
  else
      return encoded+=encoded_result;

  if((encoded_result = encode_message_type(authentication_request->messagetype, 0, buffer + encoded, len- encoded))<0)
      return encoded_result;
  else
      encoded+=encoded_result;


  if((encoded_result = encode_message_type(authentication_request->naskeysetidentifier, 0, buffer + encoded, len- encoded))<0)
      return encoded_result;
  else
      encoded+=encoded_result;
  /*if((encoded_result = encode_u8_nas_key_set_identifier(&)))*/
  /**(buffer + encoded) = ((encode_u8_nas_key_set_identifier (&authentication_request->naskeysetidentifier) & 0x0f) << 4) | 0x00;*/
  /*encoded++;*/

  if ((encoded_result = encode_authentication_parameter_rand (authentication_request->authenticationparameterrand, 0, buffer + encoded, len - encoded)) < 0)    //Return in case of error
    return encoded_result;
  else
    encoded += encoded_result;

  if ((encoded_result = encode_authentication_parameter_autn (authentication_request->authenticationparameterautn, 0, buffer + encoded, len - encoded)) < 0)    //Return in case of error
    return encoded_result;
  else
    encoded += encoded_result;

  return encoded;
}
