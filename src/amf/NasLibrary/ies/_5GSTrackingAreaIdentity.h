#include <stdint.h>
#include "bstrlib.h"

#define _5GS_TRACKING_AREA_IDENTITY_MINIMUM_LENGTH 7
#define _5GS_TRACKING_AREA_IDENTITY_MAXIMUM_LENGTH 7

typedef bstring _5GSTrackingAreaIdentity;

int encode__5gs_tracking_area_identity ( _5GSTrackingAreaIdentity _5gstrackingareaidentity, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;
int decode__5gs_tracking_area_identity ( _5GSTrackingAreaIdentity * _5gstrackingareaidentity, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;

