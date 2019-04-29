#include <stdint.h>
#include "bstrlib.h"

#define NAS_SECURITY_ALGORITHMS_MINIMUM_LENGTH 2
#define NAS_SECURITY_ALGORITHMS_MAXIMUM_LENGTH 2

typedef bstring NASSecurityAlgorithms;

int encode_nas_security_algorithms ( NASSecurityAlgorithms nassecurityalgorithms, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;
int decode_nas_security_algorithms ( NASSecurityAlgorithms * nassecurityalgorithms, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;

