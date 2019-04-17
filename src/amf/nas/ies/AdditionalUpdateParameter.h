#include <stdint.h>
#include "bstrlib.h"

#define ADDITIONAL_UPDATE_PARAMETER_MINIMUM_LENGTH 1
#define ADDITIONAL_UPDATE_PARAMETER_MAXIMUM_LENGTH 1

typedef bstring AdditionalUpdateParameter;

int encode_additional_update_parameter ( AdditionalUpdateParameter additionalupdateparameter, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;
int decode_additional_update_parameter ( AdditionalUpdateParameter * additionalupdateparameter, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;

