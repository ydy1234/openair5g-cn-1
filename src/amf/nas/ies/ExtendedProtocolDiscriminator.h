#include <stdint.h>

#define PROTOCOL_DISCRIMINATOR_MINIMUM_LENGTH 1
#define PROTOCOL_DISCRIMINATOR_MAXIMUM_LENGTH 1

typedef uint8_t ExtendedProtocolDiscriminator;

int encode_protocol_discriminator(ExtendedProtocolDiscriminator extendedprotocoldiscriminator,
        uint8_t iei,uint8_t *buffer, uint32_t len);

//void dump_protocol_discriminator_xml(ProtocolDiscriminator *protocoldiscriminator, uint8_t iei);

int decode_protocol_discriminator(ExtendedProtocolDiscriminator *protocoldiscriminator,
        uint8_t iei, uint8_t *buffer, uint32_t len);
