#include <stdint.h>
#include "bstrlib.h"

#define _5GSDRX_PARAMETERS_MINIMUM_LENGTH 3
#define _5GSDRX_PARAMETERS_MAXIMUM_LENGTH 3

typedef bstring _5GSDRXParameters;

int encode__5gsdrx_parameters ( _5GSDRXParameters _5gsdrxparameters, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;
int decode__5gsdrx_parameters ( _5GSDRXParameters * _5gsdrxparameters, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;

