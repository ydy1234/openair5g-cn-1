#include <stdint.h>
#include "bstrlib.h"

#define N1_MODE_TO_S1_MODE_NAS_TRANSPARENT_CONTAINER_MINIMUM_LENGTH 2
#define N1_MODE_TO_S1_MODE_NAS_TRANSPARENT_CONTAINER_MAXIMUM_LENGTH 2

typedef bstring N1ModeToS1ModeNASTransparentContainer;

int encode_n1_mode_to_s1_mode_nas_transparent_container ( N1ModeToS1ModeNASTransparentContainer n1modetos1modenastransparentcontainer, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;
int decode_n1_mode_to_s1_mode_nas_transparent_container ( N1ModeToS1ModeNASTransparentContainer * n1modetos1modenastransparentcontainer, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;

