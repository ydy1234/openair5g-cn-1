#include <stdint.h>
#include "bstrlib.h"

#define _5GS_UPDATE_TYPE_MINIMUM_LENGTH 3
#define _5GS_UPDATE_TYPE_MAXIMUM_LENGTH 3

typedef bstring _5GSUpdateType;

int encode__5gs_update_type ( _5GSUpdateType _5gsupdatetype, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;
int decode__5gs_update_type ( _5GSUpdateType * _5gsupdatetype, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;

