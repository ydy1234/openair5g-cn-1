#include <stdint.h>

#define SEQUENCE_NUMBER_MINIMUM_LENGTH 1
#define SEQUENCE_NUMBER_MAXIMUM_LENGTH 1

typedef uint8_t SequenceNumber;

int encode_sequence_number(SequenceNumber sequencenumber, uint8_t *buffer, uint32_t len);

//void dump_protocol_discriminator_xml(ProtocolDiscriminator *protocoldiscriminator, uint8_t iei);

int decode_sequence_number(SequenceNumber *sequencenumber, uint8_t iei, uint8_t *buffer, uint32_t len);
