#include <stdint.h>
#include "bstrlib.h"

#define _5GS_NETWORK_FEATURE_SUPPORT_MINIMUM_LENGTH 4
#define _5GS_NETWORK_FEATURE_SUPPORT_MAXIMUM_LENGTH 5

typedef bstring _5GSNetworkFeatureSupport;

int encode__5gs_network_feature_support ( _5GSNetworkFeatureSupport _5gsnetworkfeaturesupport, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;
int decode__5gs_network_feature_support ( _5GSNetworkFeatureSupport * _5gsnetworkfeaturesupport, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;

