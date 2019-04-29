#include <stdint.h>
#include "bstrlib.h"

#define _5GS_TRACKING_AREA_IDENTITY_LIST_MINIMUM_LENGTH 9
#define _5GS_TRACKING_AREA_IDENTITY_LIST_MAXIMUM_LENGTH 114

typedef bstring _5GSTrackingAreaIdentityList;

int encode__5gs_tracking_area_identity_list ( _5GSTrackingAreaIdentityList _5gstrackingareaidentitylist, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;
int decode__5gs_tracking_area_identity_list ( _5GSTrackingAreaIdentityList * _5gstrackingareaidentitylist, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;

