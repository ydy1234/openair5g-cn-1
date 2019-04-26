#include <stdint.h>

#include "ExtendedProtocolDiscriminator.h"
#include "SecurityHeaderType.h"
#include "MessageType.h"
#include "_5GSRegistrationType.h"
#include "NASKeySetIdentifier.h"
#include "_5GSMobileIdentity.h"
#include "NASKeySetIdentifier.h"
#include "_5GMMCapability.h"
#include "UESecurityCapability.h"
#include "NSSAI.h"
#include "_5GSTrackingAreaIdentity.h"
#include "S1UENetworkCapability.h"
#include "UplinkDataStatus.h"
#include "PDUSessionStatus.h"
#include "MICOIndication.h"
#include "UEStatus.h"
#include "_5GSMobileIdentity.h"
#include "AllowedPDUSessionStatus.h"
#include "USsUsageSetting.h"
#include "_5GSDRXParameters.h"
#include "EPSNASMessageContainer.h"
#include "LADNIndication.h"
#include "PayloadContainer.h"
#include "NetworkSlicingIndication.h"
#include "_5GSUpdateType.h"
#include "NASMessageContainer.h"


/* Minimum length macro. Formed by minimum length of each mandatory field */
#define REGISTRATION_REQUEST_MINIMUM_LENGTH ( \
		EXTENDED_PROTOCOL_DISCRIMINATOR_MINIMUM_LENGTH + \
		SECURITY_HEADER_TYPE_MINIMUM_LENGTH + \
		MESSAGE_TYPE_MINIMUM_LENGTH + \
		_5GS_REGISTRATION_TYPE_MINIMUM_LENGTH + \
		NAS_KEY_SET_IDENTIFIER_MINIMUM_LENGTH + \
		_5GS_MOBILE_IDENTITY_MINIMUM_LENGTH + \
		NAS_KEY_SET_IDENTIFIER_MINIMUM_LENGTH + \
		_5GMM_CAPABILITY_MINIMUM_LENGTH + \
		UE_SECURITY_CAPABILITY_MINIMUM_LENGTH + \
		NSSAI_MINIMUM_LENGTH + \
		_5GS_TRACKING_AREA_IDENTITY_MINIMUM_LENGTH + \
		S1_UE_NETWORK_CAPABILITY_MINIMUM_LENGTH + \
		UPLINK_DATA_STATUS_MINIMUM_LENGTH + \
		PDU_SESSION_STATUS_MINIMUM_LENGTH + \
		MICO_INDICATION_MINIMUM_LENGTH + \
		UE_STATUS_MINIMUM_LENGTH + \
		_5GS_MOBILE_IDENTITY_MINIMUM_LENGTH + \
		ALLOWED_PDU_SESSION_STATUS_MINIMUM_LENGTH + \
		U_SS_USAGE_SETTING_MINIMUM_LENGTH + \
		_5GSDRX_PARAMETERS_MINIMUM_LENGTH + \
		EPSNAS_MESSAGE_CONTAINER_MINIMUM_LENGTH + \
		LADN_INDICATION_MINIMUM_LENGTH + \
		PAYLOAD_CONTAINER_MINIMUM_LENGTH + \
		NETWORK_SLICING_INDICATION_MINIMUM_LENGTH + \
		_5GS_UPDATE_TYPE_MINIMUM_LENGTH + \
		NAS_MESSAGE_CONTAINER_MINIMUM_LENGTH + \
0)

/* Maximum length macro. Formed by maximum length of each field */
#define REGISTRATION_REQUEST_MAXIMUM_LENGTH ( \
		EXTENDED_PROTOCOL_DISCRIMINATOR_MAXIMUM_LENGTH + \
		SECURITY_HEADER_TYPE_MAXIMUM_LENGTH + \
		MESSAGE_TYPE_MAXIMUM_LENGTH + \
		_5GS_REGISTRATION_TYPE_MAXIMUM_LENGTH + \
		NAS_KEY_SET_IDENTIFIER_MAXIMUM_LENGTH + \
		_5GS_MOBILE_IDENTITY_MAXIMUM_LENGTH + \
		NAS_KEY_SET_IDENTIFIER_MAXIMUM_LENGTH + \
		_5GMM_CAPABILITY_MAXIMUM_LENGTH + \
		UE_SECURITY_CAPABILITY_MAXIMUM_LENGTH + \
		NSSAI_MAXIMUM_LENGTH + \
		_5GS_TRACKING_AREA_IDENTITY_MAXIMUM_LENGTH + \
		S1_UE_NETWORK_CAPABILITY_MAXIMUM_LENGTH + \
		UPLINK_DATA_STATUS_MAXIMUM_LENGTH + \
		PDU_SESSION_STATUS_MAXIMUM_LENGTH + \
		MICO_INDICATION_MAXIMUM_LENGTH + \
		UE_STATUS_MAXIMUM_LENGTH + \
		_5GS_MOBILE_IDENTITY_MAXIMUM_LENGTH + \
		ALLOWED_PDU_SESSION_STATUS_MAXIMUM_LENGTH + \
		U_SS_USAGE_SETTING_MAXIMUM_LENGTH + \
		_5GSDRX_PARAMETERS_MAXIMUM_LENGTH + \
		EPSNAS_MESSAGE_CONTAINER_MAXIMUM_LENGTH + \
		LADN_INDICATION_MAXIMUM_LENGTH + \
		PAYLOAD_CONTAINER_MAXIMUM_LENGTH + \
		NETWORK_SLICING_INDICATION_MAXIMUM_LENGTH + \
		_5GS_UPDATE_TYPE_MAXIMUM_LENGTH + \
		NAS_MESSAGE_CONTAINER_MAXIMUM_LENGTH + \
0)

typedef struct registration_request_msg_tag{
	ExtendedProtocolDiscriminator extendedprotocoldiscriminator;
	SecurityHeaderType securityheadertype;
	MessageType messagetype;
	_5GSRegistrationType _5gsregistrationtype;
	NASKeySetIdentifier naskeysetidentifier;
	_5GSMobileIdentity _5gsmobileidentity;
	NASKeySetIdentifier naskeysetidentifier;
	_5GMMCapability _5gmmcapability;
	UESecurityCapability uesecuritycapability;
	NSSAI nssai;
	_5GSTrackingAreaIdentity _5gstrackingareaidentity;
	S1UENetworkCapability s1uenetworkcapability;
	UplinkDataStatus uplinkdatastatus;
	PDUSessionStatus pdusessionstatus;
	MICOIndication micoindication;
	UEStatus uestatus;
	_5GSMobileIdentity _5gsmobileidentity;
	AllowedPDUSessionStatus allowedpdusessionstatus;
	USsUsageSetting ussusagesetting;
	_5GSDRXParameters _5gsdrxparameters;
	EPSNASMessageContainer epsnasmessagecontainer;
	LADNIndication ladnindication;
	PayloadContainer payloadcontainer;
	NetworkSlicingIndication networkslicingindication;
	_5GSUpdateType _5gsupdatetype;
	NASMessageContainer nasmessagecontainer;
}registration_request_msg;


int decode_registration_request(registration_request_msg *registrationrequest, uint8_t *buffer, uint32_t len);
int encode_registration_request(registration_request_msg *registrationrequest, uint8_t *buffer, uint32_t len);
