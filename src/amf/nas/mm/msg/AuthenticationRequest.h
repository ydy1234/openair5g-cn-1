#include <stdint.h>

#include "ExtendedProtocolDiscriminator.h"
#include "SecurityHeaderType.h"
#include "MessageType.h"
#include "NASKeySetIdentifier.h"
#include "AuthenticationParameterRand.h"
#include "AuthenticationParameterAutn.h"


/* Minimum length macro. Formed by minimum length of each mandatory field */
#define AUTHENTICATION_REQUEST_MINIMUM_LENGTH ( \
        PROTOCOL_DISCRIMINATOR_MINIMUM_LENGTH + \
        SECURITY_HEADER_TYPE_MINIMUM_LENGTH + \
        MESSAGE_TYPE_MINIMUM_LENGTH + \
        NAS_KEY_SET_IDENTIFIER_MINIMUM_LENGTH + \
        AUTHENTICATION_PARAMETER_RAND_MINIMUM_LENGTH + \
        AUTHENTICATION_PARAMETER_AUTN_MINIMUM_LENGTH )

/* Maximum length macro. Formed by maximum length of each field */
#define AUTHENTICATION_REQUEST_MAXIMUM_LENGTH ( \
        PROTOCOL_DISCRIMINATOR_MAXIMUM_LENGTH + \
        SECURITY_HEADER_TYPE_MAXIMUM_LENGTH + \
        MESSAGE_TYPE_MAXIMUM_LENGTH + \
        NAS_KEY_SET_IDENTIFIER_MAXIMUM_LENGTH + \
        AUTHENTICATION_PARAMETER_RAND_MAXIMUM_LENGTH + \
        AUTHENTICATION_PARAMETER_AUTN_MAXIMUM_LENGTH )


typedef struct authentication_request_msg_tag{
    /*  Mandatory fields  */
    ExtendedProtocolDiscriminator extendedprotocoldiscriminator;
    SecurityHeaderType securityheadertype;
    //Spare half octet
    MessageType messagetype;
    NASKeySetIdentifier naskeysetidentifier;
    //Spare half octet
    /*  Optional fields   */
    AuthenticationParameterRand authenticationparameterrand;
    AuthenticationParameterAutn authenticationparameterautn;
    //EAPMessage eapmessage;
}authentication_request_msg;

int decode_authentication_request(authentication_request_msg *authenticationrequest, uint8_t *buffer, uint32_t len);

int encode_authentication_request(authentication_request_msg *authenticationrequest, uint8_t *buffer, uint32_t len);
