#include <stdint.h>
#include "bstrlib.h"

#define _5GS_REGISTRATION_TYPE_MINIMUM_LENGTH 1
#define _5GS_REGISTRATION_TYPE_MAXIMUM_LENGTH 1

typedef bstring _5GSRegistrationType;

int encode__5gs_registration_type ( _5GSRegistrationType _5gsregistrationtype, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;
int decode__5gs_registration_type ( _5GSRegistrationType * _5gsregistrationtype, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;

