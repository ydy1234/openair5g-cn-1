#include <stdint.h>
#include "bstrlib.h"

#define MM_TIMER_MINIMUM_LENGTH 3
#define MM_TIMER_MAXIMUM_LENGTH 3

typedef bstring MMTimer;

int encode_mm_timer ( MMTimer mmtimer, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;
int decode_mm_timer ( MMTimer * mmtimer, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;

