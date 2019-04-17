#include <stdint.h>
#include "bstrlib.h"

#define INTRA_N1_MODE_NAS_TRANSPARENT_CONTAINER_MINIMUM_LENGTH 9
#define INTRA_N1_MODE_NAS_TRANSPARENT_CONTAINER_MAXIMUM_LENGTH 9

typedef bstring IntraN1ModeNASTransparentContainer;

int encode_intra_n1_mode_nas_transparent_container ( IntraN1ModeNASTransparentContainer intran1modenastransparentcontainer, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;
int decode_intra_n1_mode_nas_transparent_container ( IntraN1ModeNASTransparentContainer * intran1modenastransparentcontainer, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;

