#include <stdint.h>
#include "bstrlib.h"

#define _5GS_IDENTITY_TYPE_MINIMUM_LENGTH 1
#define _5GS_IDENTITY_TYPE_MAXIMUM_LENGTH 1

typedef bstring _5GSIdentityType;

int encode__5gs_identity_type ( _5GSIdentityType _5gsidentitytype, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;
int decode__5gs_identity_type ( _5GSIdentityType * _5gsidentitytype, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;

