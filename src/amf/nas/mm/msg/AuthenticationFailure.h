#include <stdint.h>

#include "ExtendedProtocolDiscriminator.h"
#include "SecurityHeaderType.h"
#include "MessageType.h"
#include "FiveGmmCause.h"
#include "AuthenticationFailureParameter.h"


/* Minimum length macro. Formed by minimum length of each mandatory field */
#define AUTHENTICATION_FAILURE_MINIMUM_LENGTH ( \
    FIVEGMM_CAUSE_MINIMUM_LENGTH )

/* Maximum length macro. Formed by maximum length of each field */
#define AUTHENTICATION_FAILURE_MAXIMUM_LENGTH ( \
    FIVEGMM_CAUSE_MAXIMUM_LENGTH + \
    AUTHENTICATION_FAILURE_PARAMETER_MAXIMUM_LENGTH )

/* If an optional value is present and should be encoded, the corresponding
 * Bit mask should be set to 1.
 */
# define AUTHENTICATION_FAILURE_AUTHENTICATION_FAILURE_PARAMETER_PRESENT (1<<0)


typedef enum authentication_failure_iei_tag {
  AUTHENTICATION_FAILURE_AUTHENTICATION_FAILURE_PARAMETER_IEI  = 0x30, /* 0x30 = 48 */
} authentication_failure_iei;

typedef struct authentication_failure_msg_tag {
  /* Mandatory fields */
  ExtendedProtocolDiscriminator            extendedprotocoldiscriminator;
  SecurityHeaderType               securityheadertype;
  MessageType                      messagetype;
  FiveGmmCause                         mmcause;
  /* Optional fields */
  uint32_t                         presencemask;
  AuthenticationFailureParameter   authenticationfailureparameter;
} authentication_failure_msg;

int decode_authentication_failure(authentication_failure_msg *authenticationfailure, uint8_t *buffer, uint32_t len);

int encode_authentication_failure(authentication_failure_msg *authenticationfailure, uint8_t *buffer, uint32_t len);
