#include <stdint.h>

typedef enum extended_protocol_discriminator_e {
  /* Protocol discriminator identifier for 5G Session Management */
  FIVEGS_SESSION_MANAGEMENT_MESSAGES =    0x2e,
  /* Protocol discriminator identifier for 5G Mobility Management */
  FIVEGS_MOBILITY_MANAGEMENT_MESSAGES =   0x7e,

} extended_protocol_discriminator_t;
