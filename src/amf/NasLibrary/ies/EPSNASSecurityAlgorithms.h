#include <stdint.h>
#include "bstrlib.h"

#define EPSNAS_SECURITY_ALGORITHMS_MINIMUM_LENGTH 2
#define EPSNAS_SECURITY_ALGORITHMS_MAXIMUM_LENGTH 2

typedef bstring EPSNASSecurityAlgorithms;

int encode_epsnas_security_algorithms ( EPSNASSecurityAlgorithms epsnassecurityalgorithms, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;
int decode_epsnas_security_algorithms ( EPSNASSecurityAlgorithms * epsnassecurityalgorithms, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;

