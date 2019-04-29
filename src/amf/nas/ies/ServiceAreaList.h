#include <stdint.h>
#include "bstrlib.h"

#define SERVICE_AREA_LIST_MINIMUM_LENGTH 6
#define SERVICE_AREA_LIST_MAXIMUM_LENGTH 114

typedef bstring ServiceAreaList;

int encode_service_area_list ( ServiceAreaList servicearealist, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;
int decode_service_area_list ( ServiceAreaList * servicearealist, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;

