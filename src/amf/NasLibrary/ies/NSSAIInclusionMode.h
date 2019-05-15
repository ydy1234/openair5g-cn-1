#include <stdint.h>
#include "bstrlib.h"

#define NSSAI_INCLUSION_MODE_MINIMUM_LENGTH 1
#define NSSAI_INCLUSION_MODE_MAXIMUM_LENGTH 1

typedef bstring NSSAIInclusionMode;

int encode_nssai_inclusion_mode ( NSSAIInclusionMode nssaiinclusionmode, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;
int decode_nssai_inclusion_mode ( NSSAIInclusionMode * nssaiinclusionmode, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;

