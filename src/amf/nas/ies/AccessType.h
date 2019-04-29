#include <stdint.h>
#include "bstrlib.h"

#define ACCESS_TYPE_MINIMUM_LENGTH 1
#define ACCESS_TYPE_MAXIMUM_LENGTH 1

typedef bstring AccessType;

int encode_access_type ( AccessType accesstype, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;
int decode_access_type ( AccessType * accesstype, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;

