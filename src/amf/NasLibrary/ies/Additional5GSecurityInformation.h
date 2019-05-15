#include <stdint.h>
#include "bstrlib.h"

#define ADDITIONAL_5G_SECURITY_INFORMATION_MINIMUM_LENGTH 3
#define ADDITIONAL_5G_SECURITY_INFORMATION_MAXIMUM_LENGTH 3

typedef bstring Additional5GSecurityInformation;

int encode_additional_5g_security_information ( Additional5GSecurityInformation additional5gsecurityinformation, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;
int decode_additional_5g_security_information ( Additional5GSecurityInformation * additional5gsecurityinformation, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;

