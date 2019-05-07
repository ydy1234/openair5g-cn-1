#include <stdint.h>
#include "bstrlib.h"

#define UES_USAGE_SETTING_MINIMUM_LENGTH 3
#define UES_USAGE_SETTING_MAXIMUM_LENGTH 3

typedef bstring UESUsageSetting;

int encode_ues_usage_setting ( UESUsageSetting uesusagesetting, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;
int decode_ues_usage_setting ( UESUsageSetting * uesusagesetting, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;

