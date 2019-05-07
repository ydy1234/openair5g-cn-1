#include <stdint.h>
#include "bstrlib.h"

#define SNSSAI_MINIMUM_LENGTH 3
#define SNSSAI_MAXIMUM_LENGTH 10

typedef bstring SNSSAI;

int encode_snssai ( SNSSAI snssai, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;
int decode_snssai ( SNSSAI * snssai, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;

