#include <stdint.h>

#include "ExtendedProtocolDiscriminator.h"
#include "SecurityHeaderType.h"
#include "MessageType.h"
#include "_5GSRegistrationResult.h"
#include "_5GSMobileIdentity.h"
#include "PLMNList.h"
#include "_5GSTrackingAreaIdentityList.h"
#include "NSSAI.h"
#include "RejectedNSSAI.h"
#include "NSSAI.h"
#include "_5GSNetworkFeatureSupport.h"
#include "PDUSessionStatus.h"
#include "PDUSessionReactivationResult.h"
#include "PDUSessionReactivationResultErrorCause.h"
#include "LADNInformation.h"
#include "MICOIndication.h"
#include "NetworkSlicingIndication.h"
#include "ServiceAreaList.h"
#include "GPRSTimer3.h"
#include "GPRSTimer2.h"
#include "GPRSTimer2.h"
#include "EmergencyNumberList.h"
#include "ExtendedEmergencyNumberList.h"
#include "SORTransparentContainer.h"
#include "EAPMessage.h"
#include "NSSAIInclusionMode.h"
#include "OperatorDefinedAccessCategoryDefinitions.h"
#include "_5GSDRXParameters.h"


/* Minimum length macro. Formed by minimum length of each mandatory field */
#define REGISTRATION_ACCEPT_MINIMUM_LENGTH ( \
		EXTENDED_PROTOCOL_DISCRIMINATOR_MINIMUM_LENGTH + \
		SECURITY_HEADER_TYPE_MINIMUM_LENGTH + \
		MESSAGE_TYPE_MINIMUM_LENGTH + \
		_5GS_REGISTRATION_RESULT_MINIMUM_LENGTH + \
		_5GS_MOBILE_IDENTITY_MINIMUM_LENGTH + \
		PLMN_LIST_MINIMUM_LENGTH + \
		_5GS_TRACKING_AREA_IDENTITY_LIST_MINIMUM_LENGTH + \
		NSSAI_MINIMUM_LENGTH + \
		REJECTED_NSSAI_MINIMUM_LENGTH + \
		NSSAI_MINIMUM_LENGTH + \
		_5GS_NETWORK_FEATURE_SUPPORT_MINIMUM_LENGTH + \
		PDU_SESSION_STATUS_MINIMUM_LENGTH + \
		PDU_SESSION_REACTIVATION_RESULT_MINIMUM_LENGTH + \
		PDU_SESSION_REACTIVATION_RESULT_ERROR_CAUSE_MINIMUM_LENGTH + \
		LADN_INFORMATION_MINIMUM_LENGTH + \
		MICO_INDICATION_MINIMUM_LENGTH + \
		NETWORK_SLICING_INDICATION_MINIMUM_LENGTH + \
		SERVICE_AREA_LIST_MINIMUM_LENGTH + \
		GPRS_TIMER3_MINIMUM_LENGTH + \
		GPRS_TIMER2_MINIMUM_LENGTH + \
		GPRS_TIMER2_MINIMUM_LENGTH + \
		EMERGENCY_NUMBER_LIST_MINIMUM_LENGTH + \
		EXTENDED_EMERGENCY_NUMBER_LIST_MINIMUM_LENGTH + \
		SOR_TRANSPARENT_CONTAINER_MINIMUM_LENGTH + \
		EAP_MESSAGE_MINIMUM_LENGTH + \
		NSSAI_INCLUSION_MODE_MINIMUM_LENGTH + \
		OPERATOR_DEFINED_ACCESS_CATEGORY_DEFINITIONS_MINIMUM_LENGTH + \
		_5GSDRX_PARAMETERS_MINIMUM_LENGTH + \
0)

/* Maximum length macro. Formed by maximum length of each field */
#define REGISTRATION_ACCEPT_MAXIMUM_LENGTH ( \
		EXTENDED_PROTOCOL_DISCRIMINATOR_MAXIMUM_LENGTH + \
		SECURITY_HEADER_TYPE_MAXIMUM_LENGTH + \
		MESSAGE_TYPE_MAXIMUM_LENGTH + \
		_5GS_REGISTRATION_RESULT_MAXIMUM_LENGTH + \
		_5GS_MOBILE_IDENTITY_MAXIMUM_LENGTH + \
		PLMN_LIST_MAXIMUM_LENGTH + \
		_5GS_TRACKING_AREA_IDENTITY_LIST_MAXIMUM_LENGTH + \
		NSSAI_MAXIMUM_LENGTH + \
		REJECTED_NSSAI_MAXIMUM_LENGTH + \
		NSSAI_MAXIMUM_LENGTH + \
		_5GS_NETWORK_FEATURE_SUPPORT_MAXIMUM_LENGTH + \
		PDU_SESSION_STATUS_MAXIMUM_LENGTH + \
		PDU_SESSION_REACTIVATION_RESULT_MAXIMUM_LENGTH + \
		PDU_SESSION_REACTIVATION_RESULT_ERROR_CAUSE_MAXIMUM_LENGTH + \
		LADN_INFORMATION_MAXIMUM_LENGTH + \
		MICO_INDICATION_MAXIMUM_LENGTH + \
		NETWORK_SLICING_INDICATION_MAXIMUM_LENGTH + \
		SERVICE_AREA_LIST_MAXIMUM_LENGTH + \
		GPRS_TIMER3_MAXIMUM_LENGTH + \
		GPRS_TIMER2_MAXIMUM_LENGTH + \
		GPRS_TIMER2_MAXIMUM_LENGTH + \
		EMERGENCY_NUMBER_LIST_MAXIMUM_LENGTH + \
		EXTENDED_EMERGENCY_NUMBER_LIST_MAXIMUM_LENGTH + \
		SOR_TRANSPARENT_CONTAINER_MAXIMUM_LENGTH + \
		EAP_MESSAGE_MAXIMUM_LENGTH + \
		NSSAI_INCLUSION_MODE_MAXIMUM_LENGTH + \
		OPERATOR_DEFINED_ACCESS_CATEGORY_DEFINITIONS_MAXIMUM_LENGTH + \
		_5GSDRX_PARAMETERS_MAXIMUM_LENGTH + \
0)

typedef struct registration_accept_msg_tag{
	ExtendedProtocolDiscriminator extendedprotocoldiscriminator;
	SecurityHeaderType securityheadertype;
	MessageType messagetype;
	_5GSRegistrationResult _5gsregistrationresult;
	_5GSMobileIdentity _5gsmobileidentity;
	PLMNList plmnlist;
	_5GSTrackingAreaIdentityList _5gstrackingareaidentitylist;
	NSSAI nssai;
	RejectedNSSAI rejectednssai;
	NSSAI nssai;
	_5GSNetworkFeatureSupport _5gsnetworkfeaturesupport;
	PDUSessionStatus pdusessionstatus;
	PDUSessionReactivationResult pdusessionreactivationresult;
	PDUSessionReactivationResultErrorCause pdusessionreactivationresulterrorcause;
	LADNInformation ladninformation;
	MICOIndication micoindication;
	NetworkSlicingIndication networkslicingindication;
	ServiceAreaList servicearealist;
	GPRSTimer3 gprstimer3;
	GPRSTimer2 gprstimer2;
	GPRSTimer2 gprstimer2;
	EmergencyNumberList emergencynumberlist;
	ExtendedEmergencyNumberList extendedemergencynumberlist;
	SORTransparentContainer sortransparentcontainer;
	EAPMessage eapmessage;
	NSSAIInclusionMode nssaiinclusionmode;
	OperatorDefinedAccessCategoryDefinitions operatordefinedaccesscategorydefinitions;
	_5GSDRXParameters _5gsdrxparameters;
}registration_accept_msg;


int decode_registration_accept(registration_accept_msg *registrationaccept, uint8_t *buffer, uint32_t len);
int encode_registration_accept(registration_accept_msg *registrationaccept, uint8_t *buffer, uint32_t len);
