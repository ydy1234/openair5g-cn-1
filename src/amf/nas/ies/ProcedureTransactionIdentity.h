#include <stdint.h>

#define PROCEDURE_TRANSACTION_IDENTITY_MINIMUM_LENGTH 1
#define PROCEDURE_TRANSACTION_IDENTITY_MAXIMUM_LENGTH 1

typedef uint8_t ProcedureTransactionIdentity;

int encode_procedure_transaction_identity(ProcedureTransactionIdentity proceduretransactionidentity, uint8_t *buffer, uint32_t len);

//void dump_protocol_discriminator_xml(ProtocolDiscriminator *protocoldiscriminator, uint8_t iei);

int decode_procedure_transaction_identity(ProcedureTransactionIdentity *proceduretransactionidentity, uint8_t iei, uint8_t *buffer, uint32_t len);
