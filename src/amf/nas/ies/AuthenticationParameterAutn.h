#include <stdint.h>
#include "bstrlib.h"

#define AUTHENTICATION_PARAMETER_AUTN_MINIMUM_LENGTH 17
#define AUTHENTICATION_PARAMETER_AUTN_MAXIMUM_LENGTH 17
  
typedef bstring AuthenticationParameterAutn;
  
int encode_authentication_parameter_autn(AuthenticationParameterAutn authenticationparameterautn, uint8_t iei, uint8_t *buffer, uint32_t len);
  
int decode_authentication_parameter_autn(AuthenticationParameterAutn * authenticationparameterautn, uint8_t iei, uint8_t *buffer, uint32_t len);

//void dump_authentication_parameter_autn_xml(AuthenticationParameterAutn authenticationparameterautn, uint8_t iei);
