#include <stdint.h>
#include "bstrlib.h"

#define S_NSSAI_MINIMUM_LENGTH 3
#define S_NSSAI_MAXIMUM_LENGTH 10

typedef bstring SNSSAI;

int encode_s_nssai ( SNSSAI snssai, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;
int decode_s_nssai ( SNSSAI * snssai, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;

