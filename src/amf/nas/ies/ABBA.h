#include <stdint.h>
#include "bstrlib.h"

#define ABBA_MINIMUM_LENGTH 4
#define ABBA_MAXIMUM_LENGTH 9999

typedef bstring ABBA;

int encode_abba ( ABBA abba, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;
int decode_abba ( ABBA * abba, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;

