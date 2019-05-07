#include <stdint.h>
#include "bstrlib.h"

#define MICO_INDICATION_MINIMUM_LENGTH 1
#define MICO_INDICATION_MAXIMUM_LENGTH 1

typedef bstring MICOIndication;

int encode_mico_indication ( MICOIndication micoindication, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;
int decode_mico_indication ( MICOIndication * micoindication, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;

