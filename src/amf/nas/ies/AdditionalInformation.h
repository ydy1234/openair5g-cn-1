
#include <stdint.h>
#include "bstrlib.h"

#define ADDITIONAL_INFORMATION_MINIMUM_LENGTH 3
//no maximum
//#define ADDITIONAL_INFORMATION_MAXIMUM_LENGTH

typedef bstring AdditionalInformation;

int encode_additional_information(AdditionalInformation additionalinformation, uint8_t iei, uint8_t *buffer, uint32_t len);

int decode_additional_informaiton(AdditionalInformation *additionalinformation, uint8_t iei, uint8_t *buffer, uint32_t len);

//void dump_authentication_failure_parameter_xml(AuthenticationFailureParameter authenticationfailureparameter, uint8_t iei);
