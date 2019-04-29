#include <stdint.h>
#include "bstrlib.h"

#define UE_STATUS_MINIMUM_LENGTH 3
#define UE_STATUS_MAXIMUM_LENGTH 3

typedef bstring UEStatus;

int encode_ue_status ( UEStatus uestatus, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;
int decode_ue_status ( UEStatus * uestatus, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;

