#ifndef _UE_SECURITY_CAPABILITY_H_
#define _UE_SECURITY_CAPABILITY_H_

#include <stdint.h>
#include "bstrlib.h"
#include <stdbool.h>

#define UE_SECURITY_CAPABILITY_MINIMUM_LENGTH 4
#define UE_SECURITY_CAPABILITY_MAXIMUM_LENGTH 10

typedef struct{
  bool is_NEA0_supported;
  bool is_NEA1_supported;
  bool is_NEA2_supported;
  bool is_NEA3_supported;
  bool is_NEA4_supported;
  bool is_NEA5_supported;
  bool is_NEA6_supported;
  bool is_NEA7_supported;

  bool is_NIA0_supported;
  bool is_NIA1_supported;
  bool is_NIA2_supported;
  bool is_NIA3_supported;
  bool is_NIA4_supported;
  bool is_NIA5_supported;
  bool is_NIA6_supported;
  bool is_NIA7_supported;

  bool is_EEA0_supported;
  bool is_EEA1_supported;
  bool is_EEA2_supported;
  bool is_EEA3_supported;
  bool is_EEA4_supported;
  bool is_EEA5_supported;
  bool is_EEA6_supported;
  bool is_EEA7_supported;

  bool is_EIA0_supported;
  bool is_EIA1_supported;
  bool is_EIA2_supported;
  bool is_EIA3_supported;
  bool is_EIA4_supported;
  bool is_EIA5_supported;
  bool is_EIA6_supported;
  bool is_EIA7_supported;
} UESecurityCapability;

int encode_ue_security_capability ( UESecurityCapability uesecuritycapability, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;
int decode_ue_security_capability ( UESecurityCapability * uesecuritycapability, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;

#endif
