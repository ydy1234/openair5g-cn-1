#include <stdint.h>
#include "ExtendedProtocolDiscriminator.h"
#include "SecurityHeaderType.h"
#include "MessageType.h"
#include "AuthenticationResponseParameter.h"


/* Minimum length macro. Formed by minimum length of each mandatory field */
#define AUTHENTICATION_RESPONSE_MINIMUM_LENGTH ( \
    AUTHENTICATION_RESPONSE_PARAMETER_MINIMUM_LENGTH )

/* Maximum length macro. Formed by maximum length of each field */
#define AUTHENTICATION_RESPONSE_MAXIMUM_LENGTH ( \
    AUTHENTICATION_RESPONSE_PARAMETER_MAXIMUM_LENGTH )

typedef struct authentication_response_msg_tag {
  /* Mandatory fields */
  ExtendedProtocolDiscriminator             extendedprotocoldiscriminator;
  SecurityHeaderType                securityheadertype;
  MessageType                       messagetype;
  /* Optional fields  */
  AuthenticationResponseParameter   authenticationresponseparameter;
} authentication_response_msg;

int decode_authentication_response( authentication_response_msg *authenticationresponse, uint8_t *buffer, uint32_t len);

int encode_authentication_response( authentication_response_msg *authenticationresponse, uint8_t *buffer, uint32_t len);
