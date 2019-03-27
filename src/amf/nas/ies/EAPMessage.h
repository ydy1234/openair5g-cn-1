#include <stdint.h>
#include "bstrlib.h"

#define EAP_Message_MINIMUM_LENGTH 7
#define EAP_Message_MAXIMUM_LENGTH 1503

typedef bstring EAPMessage;

int encode_EAP_Message(EAPMessage eapmessage, uint8_t iei, uint8_t *buffer, uint32_t len);

int decode_EAP_Message(EAPMessage *eapmessage, uint8_t iei, uint8_t *buffer, uint32_t len);

//void dump_authentication_failure_parameter_xml(AuthenticationFailureParameter authenticationfailureparameter, uint8_t iei);
