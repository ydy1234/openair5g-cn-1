#include <stdint.h>
#include "bstrlib.h"

#define SOR_TRANSPARENT_CONTAINER_MINIMUM_LENGTH 20
#define SOR_TRANSPARENT_CONTAINER_MAXIMUM_LENGTH 2048

typedef bstring SORTransparentContainer;

int encode_sor_transparent_container ( SORTransparentContainer sortransparentcontainer, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;
int decode_sor_transparent_container ( SORTransparentContainer * sortransparentcontainer, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;

