#include <stdint.h>
#include "bstrlib.h"

#define NAS_KEY_SET_IDENTIFIER_MINIMUM_LENGTH 1
#define NAS_KEY_SET_IDENTIFIER_MAXIMUM_LENGTH 1

typedef bstring NASKeySetIdentifier;

int encode_nas_key_set_identifier ( NASKeySetIdentifier naskeysetidentifier, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;
int decode_nas_key_set_identifier ( NASKeySetIdentifier * naskeysetidentifier, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;

