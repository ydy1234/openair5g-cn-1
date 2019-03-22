#include <stdint.h>
#include <asm/byteorder.h>


typedef struct {
#ifdef __LITTLE_ENDIAN_BITFIELD
  uint8_t extended_protocol_discriminator;
  uint8_t security_header_type:4;
#endif
#ifdef __BIG_ENDIAN_BITFIELD
  uint8_t security_header_type:4;
  uint8_t extended_protocol_discriminator;
#endif
  uint8_t message_type;
} __attribute__((__packed__)) mm_msg_header_t;
