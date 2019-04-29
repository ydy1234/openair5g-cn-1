#include <stdint.h>
#include "bstrlib.h"

#define UE_SECURITY_CAPABILITY_MINIMUM_LENGTH 4
#define UE_SECURITY_CAPABILITY_MAXIMUM_LENGTH 10

typedef bstring UESecurityCapability;

int encode_ue_security_capability ( UESecurityCapability uesecuritycapability, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;
int decode_ue_security_capability ( UESecurityCapability * uesecuritycapability, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;

