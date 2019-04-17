#include <stdint.h>
#include "bstrlib.h"

#define S1_MODE_TO_N1_MODE_NAS_TRANSPARENT_CONTAINER_MINIMUM_LENGTH 10
#define S1_MODE_TO_N1_MODE_NAS_TRANSPARENT_CONTAINER_MAXIMUM_LENGTH 12

typedef bstring S1ModeToN1ModeNASTransparentContainer;

int encode_s1_mode_to_n1_mode_nas_transparent_container ( S1ModeToN1ModeNASTransparentContainer s1modeton1modenastransparentcontainer, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;
int decode_s1_mode_to_n1_mode_nas_transparent_container ( S1ModeToN1ModeNASTransparentContainer * s1modeton1modenastransparentcontainer, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;

