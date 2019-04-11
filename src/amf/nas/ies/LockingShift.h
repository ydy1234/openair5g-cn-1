#include <stdint.h>
#include "bstrlib.h"

#define LOCKING_SHIFT_MINIMUM_LENGTH 1
#define LOCKING_SHIFT_MAXIMUM_LENGTH 1

typedef bstring LockingShift;

int encode_locking_shift ( LockingShift lockingshift, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;
int decode_locking_shift ( LockingShift * lockingshift, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;

