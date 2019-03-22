#include <stdint.h>
#include "bstrlib.h"

#define AUTHENTICATION_PARAMETER_RAND_MINIMUM_LENGTH 16
#define AUTHENTICATION_PARAMETER_RAND_MAXIMUM_LENGTH 16
  
typedef bstring AuthenticationParameterRand;
  
int encode_authentication_parameter_rand(AuthenticationParameterRand authenticationparameterrand, uint8_t iei, uint8_t *buffer, uint32_t len);
  
//void dump_authentication_parameter_rand_xml(AuthenticationParameterRand authenticationparameterrand, uint8_t iei);

int decode_authentication_parameter_rand(AuthenticationParameterRand * authenticationparameterrand, uint8_t iei, uint8_t *buffer, uint32_t len);
