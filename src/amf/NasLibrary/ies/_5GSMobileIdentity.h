#include <stdint.h>
#include "bstrlib.h"

#define _5GS_MOBILE_IDENTITY_MINIMUM_LENGTH 4
#define _5GS_MOBILE_IDENTITY_MAXIMUM_LENGTH 9999

typedef bstring _5GSMobileIdentity;

int encode__5gs_mobile_identity ( _5GSMobileIdentity _5gsmobileidentity, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;
int decode__5gs_mobile_identity ( _5GSMobileIdentity * _5gsmobileidentity, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;

