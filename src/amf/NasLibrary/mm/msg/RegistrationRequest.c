#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "TLVEncoder.h"
#include "TLVDecoder.h"
#include "RegistrationRequest.h"

int decode_registration_request( registration_request_msg *registration_request, uint8_t* buffer, uint32_t len)
{
    uint32_t decoded = 0;
    int decoded_result = 0;

    // Check if we got a NULL pointer and if buffer length is >= minimum length expected for the message.
    CHECK_PDU_POINTER_AND_LENGTH_DECODER (buffer, REGISTRATION_REQUEST_MINIMUM_LENGTH, len);

    if((decoded_result = decode__5gs_registration_type (&registration_request->_5gsregistrationtype, 0, buffer+decoded,len-decoded))<0)
        return decoded_result;
    else
        decoded+=decoded_result;
    if ((decoded_result = decode_u8_nas_key_set_identifier (&registration_request->naskeysetidentifier, 0, *(buffer + decoded) >> 4, len - decoded)) < 0)
      return decoded_result;
    decoded++;
    while (len - decoded > 0) {
      uint8_t ieiDecoded = *(buffer+decoded);
      if(ieiDecoded == 0)
        break;
      switch(ieiDecoded)  {
        case REGISTRATION_REQUEST_NAS_KEY_SET_IDENTIFIER_IEI:
          if((decoded_result = decode_nas_key_set_identifier(&registration_request->non_current_native_nas_key_set_identifier,REGISTRATION_REQUEST_NAS_KEY_SET_IDENTIFIER_IEI,buffer+decoded,len-decoded))<0)
            return decoded_result;
          else{
            decoded+=decoded_result;
            registration_request->presence |= REGISTRATION_REQUEST_NAS_KEY_SET_IDENTIFIER_PRESENT;
          }
        break;
        case REGISTRATION_REQUEST_5GMM_CAPABILITY_IEI:
          if((decoded_result = decode__5gmm_capability (&registration_request->_5gmmcapability, REGISTRATION_REQUEST_5GMM_CAPABILITY_IEI, buffer+decoded,len-decoded))<0)
            return decoded_result;
          else{
            decoded+=decoded_result;
            registration_request->presence |= REGISTRATION_REQUEST_5GMM_CAPABILITY_PRESENT;
          }
        break;
        case REGISTRATION_REQUEST_UE_SECURITY_CAPABILITY_IEI:
          if((decoded_result = decode_ue_security_capability (&registration_request->uesecuritycapability, REGISTRATION_REQUEST_UE_SECURITY_CAPABILITY_IEI, buffer+decoded,len-decoded))<0)
            return decoded_result;
          else{
            decoded+=decoded_result;
            registration_request->presence |= REGISTRATION_REQUEST_UE_SECURITY_CAPABILITY_PRESENT;
          }
        break;
        case REGISTRATION_REQUEST_5GS_TRACKING_AREA_IDENTITY_IEI:
          if((decoded_result = decode__5gs_tracking_area_identity (&registration_request->_5gstrackingareaidentity, 0REGISTRATION_REQUEST_5GS_TRACKING_AREA_IDENTITY_IEI, buffer+decoded,len-decoded))<0)
            return decoded_result;
          else{
            decoded+=decoded_result;
            registration_request->presence |= REGISTRATION_REQUEST_5GS_TRACKING_AREA_IDENTITY_PRESENT;
          }
        break;
        case REGISTRATION_REQUEST_S1_UE_NETWORK_CAPABILITY_IEI:
          if((decoded_result = decode_s1_ue_network_capability (&registration_request->s1uenetworkcapability, REGISTRATION_REQUEST_S1_UE_NETWORK_CAPABILITY_IEI, buffer+decoded,len-decoded))<0)
            return decoded_result;
          else{
            decoded+=decoded_result;
            registration_request->presence |= REGISTRATION_REQUEST_S1_UE_NETWORK_CAPABILITY_PRESENT;
          }
      }
    }  

/*
    if((decoded_result = decode_extended_protocol_discriminator (&registration_request->extendedprotocoldiscriminator, 0, buffer+decoded,len-decoded))<0)
        return decoded_result;
    else
        decoded+=decoded_result;

    if((decoded_result = decode_security_header_type (&registration_request->securityheadertype, 0, buffer+decoded,len-decoded))<0)
        return decoded_result;
    else
        decoded+=decoded_result;

    if((decoded_result = decode_message_type (&registration_request->messagetype, 0, buffer+decoded,len-decoded))<0)
        return decoded_result;
    else
        decoded+=decoded_result;

    if((decoded_result = decode__5gs_registration_type (&registration_request->_5gsregistrationtype, 0, buffer+decoded,len-decoded))<0)
        return decoded_result;
    else
        decoded+=decoded_result;

    if((decoded_result = decode_nas_key_set_identifier (&registration_request->naskeysetidentifier, 0, buffer+decoded,len-decoded))<0)
        return decoded_result;
    else
        decoded+=decoded_result;

    if((decoded_result = decode__5gs_mobile_identity (&registration_request->_5gsmobileidentity, 0, buffer+decoded,len-decoded))<0)
        return decoded_result;
    else
        decoded+=decoded_result;

    if((decoded_result = decode__5gmm_capability (&registration_request->_5gmmcapability, 0, buffer+decoded,len-decoded))<0)
        return decoded_result;
    else
        decoded+=decoded_result;

    if((decoded_result = decode_ue_security_capability (&registration_request->uesecuritycapability, 0, buffer+decoded,len-decoded))<0)
        return decoded_result;
    else
        decoded+=decoded_result;

    if((decoded_result = decode_nssai (&registration_request->nssai, 0, buffer+decoded,len-decoded))<0)
        return decoded_result;
    else
        decoded+=decoded_result;

    if((decoded_result = decode__5gs_tracking_area_identity (&registration_request->_5gstrackingareaidentity, 0, buffer+decoded,len-decoded))<0)
        return decoded_result;
    else
        decoded+=decoded_result;

    if((decoded_result = decode_s1_ue_network_capability (&registration_request->s1uenetworkcapability, 0, buffer+decoded,len-decoded))<0)
        return decoded_result;
    else
        decoded+=decoded_result;

    if((decoded_result = decode_uplink_data_status (&registration_request->uplinkdatastatus, 0, buffer+decoded,len-decoded))<0)
        return decoded_result;
    else
        decoded+=decoded_result;

    if((decoded_result = decode_pdu_session_status (&registration_request->pdusessionstatus, 0, buffer+decoded,len-decoded))<0)
        return decoded_result;
    else
        decoded+=decoded_result;

    if((decoded_result = decode_mico_indication (&registration_request->micoindication, 0, buffer+decoded,len-decoded))<0)
        return decoded_result;
    else
        decoded+=decoded_result;

    if((decoded_result = decode_ue_status (&registration_request->uestatus, 0, buffer+decoded,len-decoded))<0)
        return decoded_result;
    else
        decoded+=decoded_result;

    if((decoded_result = decode_allowed_pdu_session_status (&registration_request->allowedpdusessionstatus, 0, buffer+decoded,len-decoded))<0)
        return decoded_result;
    else
        decoded+=decoded_result;

    if((decoded_result = decode_ues_usage_setting (&registration_request->uesusagesetting, 0, buffer+decoded,len-decoded))<0)
        return decoded_result;
    else
        decoded+=decoded_result;

    if((decoded_result = decode__5gsdrx_parameters (&registration_request->_5gsdrxparameters, 0, buffer+decoded,len-decoded))<0)
        return decoded_result;
    else
        decoded+=decoded_result;

    if((decoded_result = decode_epsnas_message_container (&registration_request->epsnasmessagecontainer, 0, buffer+decoded,len-decoded))<0)
        return decoded_result;
    else
        decoded+=decoded_result;

    if((decoded_result = decode_ladn_indication (&registration_request->ladnindication, 0, buffer+decoded,len-decoded))<0)
        return decoded_result;
    else
        decoded+=decoded_result;

    if((decoded_result = decode_payload_container (&registration_request->payloadcontainer, 0, buffer+decoded,len-decoded))<0)
        return decoded_result;
    else
        decoded+=decoded_result;

    if((decoded_result = decode_network_slicing_indication (&registration_request->networkslicingindication, 0, buffer+decoded,len-decoded))<0)
        return decoded_result;
    else
        decoded+=decoded_result;

    if((decoded_result = decode__5gs_update_type (&registration_request->_5gsupdatetype, 0, buffer+decoded,len-decoded))<0)
        return decoded_result;
    else
        decoded+=decoded_result;

    if((decoded_result = decode_nas_message_container (&registration_request->nasmessagecontainer, 0, buffer+decoded,len-decoded))<0)
        return decoded_result;
    else
        decoded+=decoded_result;
*/

    return decoded;
}


int encode_registration_request( registration_request_msg *registration_request, uint8_t* buffer, uint32_t len)
{
    uint32_t encoded = 0;
    int encoded_result = 0;
    
    // Check if we got a NULL pointer and if buffer length is >= minimum length expected for the message.
    CHECK_PDU_POINTER_AND_LENGTH_ENCODER (buffer, REGISTRATION_REQUEST_MINIMUM_LENGTH, len);

    if((encoded_result = encode__5gs_registration_type (&registration_request->_5gsregistrationtype, 0, buffer+encoded,len-encoded))<0)
        return encoded_result;
    else
        encoded+=encoded_result;

    *(buffer + encoded) = ((encode_u8_nas_key_set_identifier(&registration_request->naskeysetidentifier) & 0x0f) << 4) | 0x00;
    encoded ++;
//encode mobile identity
    if(registration_request->presence & REGISTRATION_REQUEST_NAS_KEY_SET_IDENTIFIER_PRESENT
       == REGISTRATION_REQUEST_NAS_KEY_SET_IDENTIFIER_PRESENT){
      if((encoded_result = encode_nas_key_set_identifier(&registration_request->non_current_native_nas_key_set_identifier,REGISTRATION_REQUEST_NAS_KEY_SET_IDENTIFIER_PRESENT,buffer+encoded,len-encoded))<0)
        return encoded_result;
      else
        encoded+=encoded_result;
    }

    if(registration_request->presence & REGISTRATION_REQUEST_5GMM_CAPABILITY_PRESENT
       == REGISTRATION_REQUEST_5GMM_CAPABILITY_PRESENT){
      if((encoded_result = encode__5gmm_capability (registration_request->_5gmmcapability, REGISTRATION_REQUEST_5GMM_CAPABILITY_IEI, buffer+encoded,len-encoded))<0)
        return encoded_result;
      else
        encoded+=encoded_result; 
    }

    if(registration_request->presence & REGISTRATION_REQUEST_UE_SECURITY_CAPABILITY_PRESENT
       == REGISTRATION_REQUEST_UE_SECURITY_CAPABILITY_PRESENT){
      if((encoded_result = encode_ue_security_capability (registration_request->uesecuritycapability, REGISTRATION_REQUEST_UE_SECURITY_CAPABILITY_IEI, buffer+encoded,len-encoded))<0)
        return encoded_result;
      else
        encoded+=encoded_result; 
    }
//encode nssai
    if(registration_request->presence & REGISTRATION_REQUEST_5GS_TRACKING_AREA_IDENTITY_PRESENT
       == REGISTRATION_REQUEST_5GS_TRACKING_AREA_IDENTITY_PRESENT){
      if((encoded_result = encode__5gs_tracking_area_identity (registration_request->_5gstrackingareaidentity, REGISTRATION_REQUEST_5GS_TRACKING_AREA_IDENTITY_IEI, buffer+encoded,len-encoded))<0)
        return encoded_result;
      else
        encoded+=encoded_result;
    }
    if(registration_request->presence & REGISTRATION_REQUEST_S1_UE_NETWORK_CAPABILITY_PRESENT
       == REGISTRATION_REQUEST_S1_UE_NETWORK_CAPABILITY_PRESENT){
      if((encoded_result = encode_s1_ue_network_capability (registration_request->s1uenetworkcapability, REGISTRATION_REQUEST_S1_UE_NETWORK_CAPABILITY_IEI, buffer+encoded,len-encoded))<0)
        return encoded_result;
      else
        encoded+=encoded_result;
    }


/*
    if((encoded_result = encode_extended_protocol_discriminator (registration_request->extendedprotocoldiscriminator, 0, buffer+encoded,len-encoded))<0)
        return encoded_result;
    else
        encoded+=encoded_result;

    if((encoded_result = encode_security_header_type (registration_request->securityheadertype, 0, buffer+encoded,len-encoded))<0)
        return encoded_result;
    else
        encoded+=encoded_result;

    if((encoded_result = encode_message_type (registration_request->messagetype, 0, buffer+encoded,len-encoded))<0)
        return encoded_result;
    else
        encoded+=encoded_result;

    if((encoded_result = encode__5gs_registration_type (registration_request->_5gsregistrationtype, 0, buffer+encoded,len-encoded))<0)
        return encoded_result;
    else
        encoded+=encoded_result;

    if((encoded_result = encode_nas_key_set_identifier (registration_request->naskeysetidentifier, 0, buffer+encoded,len-encoded))<0)
        return encoded_result;
    else
        encoded+=encoded_result;

    if((encoded_result = encode__5gs_mobile_identity (registration_request->_5gsmobileidentity, 0, buffer+encoded,len-encoded))<0)
        return encoded_result;
    else
        encoded+=encoded_result;

    if((encoded_result = encode__5gmm_capability (registration_request->_5gmmcapability, 0, buffer+encoded,len-encoded))<0)
        return encoded_result;
    else
        encoded+=encoded_result;

    if((encoded_result = encode_ue_security_capability (registration_request->uesecuritycapability, 0, buffer+encoded,len-encoded))<0)
        return encoded_result;
    else
        encoded+=encoded_result;

    if((encoded_result = encode_nssai (registration_request->nssai, 0, buffer+encoded,len-encoded))<0)
        return encoded_result;
    else
        encoded+=encoded_result;

    if((encoded_result = encode__5gs_tracking_area_identity (registration_request->_5gstrackingareaidentity, 0, buffer+encoded,len-encoded))<0)
        return encoded_result;
    else
        encoded+=encoded_result;

    if((encoded_result = encode_s1_ue_network_capability (registration_request->s1uenetworkcapability, 0, buffer+encoded,len-encoded))<0)
        return encoded_result;
    else
        encoded+=encoded_result;

    if((encoded_result = encode_uplink_data_status (registration_request->uplinkdatastatus, 0, buffer+encoded,len-encoded))<0)
        return encoded_result;
    else
        encoded+=encoded_result;

    if((encoded_result = encode_pdu_session_status (registration_request->pdusessionstatus, 0, buffer+encoded,len-encoded))<0)
        return encoded_result;
    else
        encoded+=encoded_result;

    if((encoded_result = encode_mico_indication (registration_request->micoindication, 0, buffer+encoded,len-encoded))<0)
        return encoded_result;
    else
        encoded+=encoded_result;

    if((encoded_result = encode_ue_status (registration_request->uestatus, 0, buffer+encoded,len-encoded))<0)
        return encoded_result;
    else
        encoded+=encoded_result;

    if((encoded_result = encode_allowed_pdu_session_status (registration_request->allowedpdusessionstatus, 0, buffer+encoded,len-encoded))<0)
        return encoded_result;
    else
        encoded+=encoded_result;

    if((encoded_result = encode_ues_usage_setting (registration_request->uesusagesetting, 0, buffer+encoded,len-encoded))<0)
        return encoded_result;
    else
        encoded+=encoded_result;

    if((encoded_result = encode__5gsdrx_parameters (registration_request->_5gsdrxparameters, 0, buffer+encoded,len-encoded))<0)
        return encoded_result;
    else
        encoded+=encoded_result;

    if((encoded_result = encode_epsnas_message_container (registration_request->epsnasmessagecontainer, 0, buffer+encoded,len-encoded))<0)
        return encoded_result;
    else
        encoded+=encoded_result;

    if((encoded_result = encode_ladn_indication (registration_request->ladnindication, 0, buffer+encoded,len-encoded))<0)
        return encoded_result;
    else
        encoded+=encoded_result;

    if((encoded_result = encode_payload_container (registration_request->payloadcontainer, 0, buffer+encoded,len-encoded))<0)
        return encoded_result;
    else
        encoded+=encoded_result;

    if((encoded_result = encode_network_slicing_indication (registration_request->networkslicingindication, 0, buffer+encoded,len-encoded))<0)
        return encoded_result;
    else
        encoded+=encoded_result;

    if((encoded_result = encode__5gs_update_type (registration_request->_5gsupdatetype, 0, buffer+encoded,len-encoded))<0)
        return encoded_result;
    else
        encoded+=encoded_result;

    if((encoded_result = encode_nas_message_container (registration_request->nasmessagecontainer, 0, buffer+encoded,len-encoded))<0)
        return encoded_result;
    else
        encoded+=encoded_result;

*/
    return encoded;
}
