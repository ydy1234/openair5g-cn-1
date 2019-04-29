#include <stdint.h>
#include "bstrlib.h"

#define NSSAI_MINIMUM_LENGTH 4
#define NSSAI_MAXIMUM_LENGTH 146

typedef bstring NSSAI;

int encode_nssai ( NSSAI nssai, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;
int decode_nssai ( NSSAI * nssai, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;

