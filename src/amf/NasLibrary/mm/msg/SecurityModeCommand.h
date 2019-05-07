#include <stdint.h>

#include "ExtendedProtocolDiscriminator.h"
#include "SecurityHeaderType.h"
#include "MessageType.h"
#include "NASSecurityAlgorithms.h"
#include "NASKeySetIdentifier.h"
#include "UESecurityCapability.h"
#include "IMEISVRequest.h"
#include "EPSNASSecurityAlgorithms.h"
#include "Additional5GSecurityInformation.h"
#include "EAPMessage.h"
#include "ABBA.h"
#include "S1UESecurityCapability.h"


/* Minimum length macro. Formed by minimum length of each mandatory field */
#define SECURITY_MODE_COMMAND_MINIMUM_LENGTH ( \
		EXTENDED_PROTOCOL_DISCRIMINATOR_MINIMUM_LENGTH + \
		SECURITY_HEADER_TYPE_MINIMUM_LENGTH + \
		MESSAGE_TYPE_MINIMUM_LENGTH + \
		NAS_SECURITY_ALGORITHMS_MINIMUM_LENGTH + \
		NAS_KEY_SET_IDENTIFIER_MINIMUM_LENGTH + \
		UE_SECURITY_CAPABILITY_MINIMUM_LENGTH + \
		IMEISV_REQUEST_MINIMUM_LENGTH + \
		EPSNAS_SECURITY_ALGORITHMS_MINIMUM_LENGTH + \
		ADDITIONAL_5G_SECURITY_INFORMATION_MINIMUM_LENGTH + \
		EAP_MESSAGE_MINIMUM_LENGTH + \
		ABBA_MINIMUM_LENGTH + \
		S1_UE_SECURITY_CAPABILITY_MINIMUM_LENGTH + \
0)

/* Maximum length macro. Formed by maximum length of each field */
#define SECURITY_MODE_COMMAND_MAXIMUM_LENGTH ( \
		EXTENDED_PROTOCOL_DISCRIMINATOR_MAXIMUM_LENGTH + \
		SECURITY_HEADER_TYPE_MAXIMUM_LENGTH + \
		MESSAGE_TYPE_MAXIMUM_LENGTH + \
		NAS_SECURITY_ALGORITHMS_MAXIMUM_LENGTH + \
		NAS_KEY_SET_IDENTIFIER_MAXIMUM_LENGTH + \
		UE_SECURITY_CAPABILITY_MAXIMUM_LENGTH + \
		IMEISV_REQUEST_MAXIMUM_LENGTH + \
		EPSNAS_SECURITY_ALGORITHMS_MAXIMUM_LENGTH + \
		ADDITIONAL_5G_SECURITY_INFORMATION_MAXIMUM_LENGTH + \
		EAP_MESSAGE_MAXIMUM_LENGTH + \
		ABBA_MAXIMUM_LENGTH + \
		S1_UE_SECURITY_CAPABILITY_MAXIMUM_LENGTH + \
0)

typedef struct security_mode_command_msg_tag{
	ExtendedProtocolDiscriminator extendedprotocoldiscriminator;
	SecurityHeaderType securityheadertype;
	MessageType messagetype;
	NASSecurityAlgorithms nassecurityalgorithms;
	NASKeySetIdentifier naskeysetidentifier;
	UESecurityCapability uesecuritycapability;
	IMEISVRequest imeisvrequest;
	EPSNASSecurityAlgorithms epsnassecurityalgorithms;
	Additional5GSecurityInformation additional5gsecurityinformation;
	EAPMessage eapmessage;
	ABBA abba;
	S1UESecurityCapability s1uesecuritycapability;
}security_mode_command_msg;


int decode_security_mode_command(security_mode_command_msg *securitymodecommand, uint8_t *buffer, uint32_t len);
int encode_security_mode_command(security_mode_command_msg *securitymodecommand, uint8_t *buffer, uint32_t len);
