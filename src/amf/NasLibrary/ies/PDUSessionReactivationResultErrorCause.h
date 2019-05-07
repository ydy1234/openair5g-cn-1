#include <stdint.h>
#include "bstrlib.h"

#define PDU_SESSION_REACTIVATION_RESULT_ERROR_CAUSE_MINIMUM_LENGTH 5
#define PDU_SESSION_REACTIVATION_RESULT_ERROR_CAUSE_MAXIMUM_LENGTH 515

typedef bstring PDUSessionReactivationResultErrorCause;

int encode_pdu_session_reactivation_result_error_cause ( PDUSessionReactivationResultErrorCause pdusessionreactivationresulterrorcause, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;
int decode_pdu_session_reactivation_result_error_cause ( PDUSessionReactivationResultErrorCause * pdusessionreactivationresulterrorcause, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;

