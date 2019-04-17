#include <stdint.h>
#include "bstrlib.h"

#define GPRS_TIMER_3_MINIMUM_LENGTH 3
#define GPRS_TIMER_3_MAXIMUM_LENGTH 3

typedef bstring GPRSTimer3;

int encode_gprs_timer_3 ( GPRSTimer3 gprstimer3, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;
int decode_gprs_timer_3 ( GPRSTimer3 * gprstimer3, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;

