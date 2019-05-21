#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "TLVEncoder.h"
#include "TLVDecoder.h"
#include "AuthenticationFailure.h"

int decode_authentication_failure( authentication_failure_msg *authentication_failure, uint8_t* buffer, uint32_t len)
{
    uint32_t decoded = 0;
    int decoded_result = 0;

    // Check if we got a NULL pointer and if buffer length is >= minimum length expected for the message.
    CHECK_PDU_POINTER_AND_LENGTH_DECODER (buffer, AUTHENTICATION_FAILURE_MINIMUM_LENGTH, len);
/*
    if((decoded_result = decode_extended_protocol_discriminator (&authentication_failure->extendedprotocoldiscriminator, 0, buffer+decoded,len-decoded))<0)
        return decoded_result;
    else
        decoded+=decoded_result;

    if((decoded_result = decode_security_header_type (&authentication_failure->securityheadertype, 0, buffer+decoded,len-decoded))<0)
        return decoded_result;
    else
        decoded+=decoded_result;

    if((decoded_result = decode_message_type (&authentication_failure->messagetype, 0, buffer+decoded,len-decoded))<0)
        return decoded_result;
    else
        decoded+=decoded_result;
*/
    if((decoded_result = decode__5gmm_cause (&authentication_failure->_5gmmcause, 0, buffer+decoded,len-decoded))<0)
        return decoded_result;
    else
        decoded+=decoded_result;

  /*
   * Decoding optional fields
   */
  while (len - decoded > 0) {               
    uint8_t                                 ieiDecoded = *(buffer + decoded);
    
    /*
     * Type | value iei are below 0x80 so just return the first 4 bits
     */
    if (ieiDecoded >= 0x80)
      ieiDecoded = ieiDecoded & 0xf0;
    
    switch (ieiDecoded) {
    case AUTHENTICATION_FAILURE_PARAMETER_IEI:
      if ((decoded_result = decode_authentication_failure_parameter (&authentication_failure->authenticationfailureparameter, AUTHENTICATION_FAILURE_PARAMETER_IEI, buffer + decoded, len - decoded)) <= 0)
        return decoded_result;
      
      decoded += decoded_result;
      /*
       * Set corresponding mask to 1 in presencemask
       */
      authentication_failure->presencemask |= AUTHENTICATION_FAILURE_PARAMETER_PRESENT;
      break;
    
    default:
      errorCodeDecoder = TLV_UNEXPECTED_IEI;
      return TLV_UNEXPECTED_IEI;
    }
  }


    return decoded;
}


int encode_authentication_failure( authentication_failure_msg *authentication_failure, uint8_t* buffer, uint32_t len)
{
    uint32_t encoded = 0;
    int encoded_result = 0;
    
    // Check if we got a NULL pointer and if buffer length is >= minimum length expected for the message.
    CHECK_PDU_POINTER_AND_LENGTH_ENCODER (buffer, AUTHENTICATION_FAILURE_MINIMUM_LENGTH, len);
/*
    if((encoded_result = encode_extended_protocol_discriminator (authentication_failure->extendedprotocoldiscriminator, 0, buffer+encoded,len-encoded))<0)
        return encoded_result;
    else
        encoded+=encoded_result;

    if((encoded_result = encode_security_header_type (authentication_failure->securityheadertype, 0, buffer+encoded,len-encoded))<0)
        return encoded_result;
    else
        encoded+=encoded_result;

    if((encoded_result = encode_message_type (authentication_failure->messagetype, 0, buffer+encoded,len-encoded))<0)
        return encoded_result;
    else
        encoded+=encoded_result;
*/
    if((encoded_result = encode__5gmm_cause (&authentication_failure->_5gmmcause, 0, buffer+encoded,len-encoded))<0)
        return encoded_result;
    else
        encoded+=encoded_result;

  if ((authentication_failure->presencemask & AUTHENTICATION_FAILURE_PARAMETER_PRESENT)
      == AUTHENTICATION_FAILURE_PARAMETER_PRESENT) {
    if ((encoded_result = encode_authentication_failure_parameter (authentication_failure->authenticationfailureparameter, AUTHENTICATION_FAILURE_PARAMETER_IEI, buffer + encoded, len - encoded)) < 0)
      // Return in case of error
      return encoded_result;
    else
      encoded += encoded_result;
  }


    return encoded;
}
