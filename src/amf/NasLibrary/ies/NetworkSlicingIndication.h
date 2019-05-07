#include <stdint.h>
#include "bstrlib.h"

#define NETWORK_SLICING_INDICATION_MINIMUM_LENGTH 1
#define NETWORK_SLICING_INDICATION_MAXIMUM_LENGTH 1

typedef bstring NetworkSlicingIndication;

int encode_network_slicing_indication ( NetworkSlicingIndication networkslicingindication, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;
int decode_network_slicing_indication ( NetworkSlicingIndication * networkslicingindication, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;

