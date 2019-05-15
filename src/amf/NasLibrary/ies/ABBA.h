#include <stdint.h>
#include "bstrlib.h"

#define ABBA_MINIMUM_LENGTH 6
#define ABBA_MAXIMUM_LENGTH 6

typedef bstring ABBA;

int encode_abba ( ABBA abba, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;
int decode_abba ( ABBA * abba, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;

