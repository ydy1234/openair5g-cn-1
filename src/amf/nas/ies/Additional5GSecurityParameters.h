#include <stdint.h>
#include "bstrlib.h"

#define ADDITIONAL_5G_SECURITY_PARAMETERS_MINIMUM_LENGTH 3
#define ADDITIONAL_5G_SECURITY_PARAMETERS_MAXIMUM_LENGTH 3

typedef bstring Additional5GSecurityParameters;

int encode_additional_5g_security_parameters ( Additional5GSecurityParameters additional5gsecurityparameters, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;
int decode_additional_5g_security_parameters ( Additional5GSecurityParameters * additional5gsecurityparameters, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;

