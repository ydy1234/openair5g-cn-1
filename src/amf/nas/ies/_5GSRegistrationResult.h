#include <stdint.h>
#include "bstrlib.h"

#define _5GS_REGISTRATION_RESULT_MINIMUM_LENGTH 3
#define _5GS_REGISTRATION_RESULT_MAXIMUM_LENGTH 3

typedef bstring _5GSRegistrationResult;

int encode__5gs_registration_result ( _5GSRegistrationResult _5gsregistrationresult, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;
int decode__5gs_registration_result ( _5GSRegistrationResult * _5gsregistrationresult, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;

