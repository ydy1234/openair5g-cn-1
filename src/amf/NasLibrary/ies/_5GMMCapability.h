#include <stdint.h>
#include "bstrlib.h"

#define _5GMM_CAPABILITY_MINIMUM_LENGTH 3
#define _5GMM_CAPABILITY_MAXIMUM_LENGTH 15

typedef bstring _5GMMCapability;

int encode__5gmm_capability ( _5GMMCapability _5gmmcapability, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;
int decode__5gmm_capability ( _5GMMCapability * _5gmmcapability, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;

