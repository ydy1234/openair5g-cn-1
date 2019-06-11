#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "ngap_amf_uplink_nas_transport.h"

#include "Ngap_ProtocolIE-Field.h"
#include "Ngap_BroadcastPLMNItem.h"
#include "Ngap_GNB-ID.h"
#include "Ngap_GlobalGNB-ID.h"
#include "Ngap_PagingDRX.h"
#include "Ngap_SliceSupportItem.h"
#include "Ngap_SupportedTAItem.h"
#include "Ngap_GlobalRANNodeID.h"
#include "Ngap_InitiatingMessage.h"


#include "sctp_gNB_defs.h"

#include "bstrlib.h"
#include "intertask_interface_types.h"


#include "Ngap_ProtocolIE-Field.h"
#include "Ngap_InitiatingMessage.h"
#include "Ngap_BroadcastPLMNItem.h"
#include "Ngap_GNB-ID.h"
#include "Ngap_GlobalGNB-ID.h"
#include "Ngap_PagingDRX.h"
#include "Ngap_SliceSupportItem.h"
#include "Ngap_SupportedTAItem.h"



#include "sctp_gNB_defs.h"

#include "bstrlib.h"
#include "intertask_interface_types.h"

#include "Ngap_CriticalityDiagnostics-IE-Item.h"
#include "Ngap_EUTRACellIdentity.h"
#include "Ngap_TimeStamp.h"
#include "Ngap_EUTRA-CGI.h"
#include "Ngap_UserLocationInformationEUTRA.h"




Ngap_UplinkNASTransport_IEs_t * make_AMF_UE_NGAP_ID(unsigned long AMF_UE_NGAP_ID)
{
   	Ngap_UplinkNASTransport_IEs_t *ie;
	ie = calloc(1, sizeof(Ngap_UplinkNASTransport_IEs_t));
	
	ie->id = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
	ie->criticality = Ngap_Criticality_reject;
	ie->value.present = Ngap_UplinkNASTransport_IEs__value_PR_AMF_UE_NGAP_ID;
	
	asn_ulong2INTEGER(&ie->value.choice.AMF_UE_NGAP_ID, AMF_UE_NGAP_ID);
	
	return ie;	
}

//RAN_UE_NGAP_ID

Ngap_UplinkNASTransport_IEs_t * make_RAN_UE_NGAP_ID(unsigned long rAN_UE_NGAP_ID)
{
	Ngap_UplinkNASTransport_IEs_t *ie;
	ie = calloc(1, sizeof(Ngap_UplinkNASTransport_IEs_t));

	ie->id = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
	ie->criticality = Ngap_Criticality_reject;
	ie->value.present = Ngap_UplinkNASTransport_IEs__value_PR_RAN_UE_NGAP_ID;
	ie->value.choice.RAN_UE_NGAP_ID = rAN_UE_NGAP_ID;

	return ie;
}


//userLocationInformationEUTRA
//CGI
//pLMNIdentity
//eUTRACellIdentity

void uplink_fill_eUTRA_with_CGI_with_pLMNIdentity(Ngap_PLMNIdentity_t *pLMNIdentity)
{
    uint8_t plmn[3] = { 0x02, 0xF8, 0x29 };
	OCTET_STRING_fromBuf(pLMNIdentity, (const char*)plmn, 3);

	printf("pLMNIdentity: 0x%x,0x%x,0x%x\n", pLMNIdentity->buf[0],pLMNIdentity->buf[1],pLMNIdentity->buf[2]);
}
void uplink_fill_eUTRA_with_CGI_with_eUTRACellIdentity(Ngap_EUTRACellIdentity_t	 *eUTRACellIdentity, int index)
{
    uint32_t ei = htonl(index);
	eUTRACellIdentity->buf = calloc(4, sizeof(uint8_t));
	eUTRACellIdentity->size = 4;
	memcpy(eUTRACellIdentity->buf, &ei, 4);
	eUTRACellIdentity->bits_unused = 0x04;

    printf("eUTRACellIdentity: 0x%x,0x%x,0x%x,0x%x\n",
	eUTRACellIdentity->buf[0],eUTRACellIdentity->buf[1],
	eUTRACellIdentity->buf[2],eUTRACellIdentity->buf[3]);
}

void uplink_fill_userLocationInformationEUTRA_eUTRA_with_CGI(Ngap_EUTRA_CGI_t	 *eUTRA_CGI)
{
    uplink_fill_eUTRA_with_CGI_with_pLMNIdentity(&eUTRA_CGI->pLMNIdentity);
	uplink_fill_eUTRA_with_CGI_with_eUTRACellIdentity(&eUTRA_CGI->eUTRACellIdentity, 0x79);
    
}

void uplink_fill_tAI_with_pLMNIdentity(Ngap_PLMNIdentity_t *pLMNIdentity)
{
    uint8_t plmn[3] = { 0x02, 0xF8, 0x29 };
	OCTET_STRING_fromBuf(pLMNIdentity, (const char*)plmn, 3);

	printf("pLMNIdentity: 0x%x,0x%x,0x%x\n", pLMNIdentity->buf[0],pLMNIdentity->buf[1],pLMNIdentity->buf[2]);
}
void uplink_fill_tAI_with_tAC(Ngap_TAC_t	 *tAC)
{
	uint8_t tac[3] = { 0x01, 0xF8, 0x29 };
	OCTET_STRING_fromBuf(tAC, (const char*)tac, 3);
	
    printf("tAC: 0x%x,0x%x,0x%x\n", tAC->buf[0],tAC->buf[1],tAC->buf[2]);
}
void uplink_fill_userLocationInformationEUTRA_eUTRA_with_tAI(Ngap_TAI_t	 *tAI)
{
    uplink_fill_tAI_with_pLMNIdentity(&tAI->pLMNIdentity);
	uplink_fill_tAI_with_tAC(&tAI->tAC);
}
Ngap_TimeStamp_t	* uplink_fill_userLocationInformationEUTRA_eUTRA_with_timeStamp()
{
    
    Ngap_TimeStamp_t	*timeStamp = NULL;
	timeStamp = calloc(1,sizeof(Ngap_TimeStamp_t));
    memset(timeStamp, 0 , sizeof(Ngap_TimeStamp_t));
    
	
    uint8_t st[4] = { 0x02, 0xF8, 0x29, 0x06 };
	OCTET_STRING_fromBuf(timeStamp, (const char*)st, 4);

	printf("timeStamp: 0x%x,0x%x,0x%x,0x%x\n", timeStamp->buf[0],timeStamp->buf[1],timeStamp->buf[2],timeStamp->buf[3]);
	
	return timeStamp;
}

Ngap_UserLocationInformationEUTRA_t   * uplink_fill_userLocationInformationEUTRA()
{
    Ngap_UserLocationInformationEUTRA_t   *uiEUTRA = NULL;
	uiEUTRA = calloc(1, sizeof(Ngap_UserLocationInformationEUTRA_t));
	memset(uiEUTRA, 0 , sizeof(Ngap_UserLocationInformationEUTRA_t));

	uplink_fill_userLocationInformationEUTRA_eUTRA_with_CGI(&uiEUTRA->eUTRA_CGI);
    uplink_fill_userLocationInformationEUTRA_eUTRA_with_tAI(&uiEUTRA->tAI);
	uiEUTRA->timeStamp = uplink_fill_userLocationInformationEUTRA_eUTRA_with_timeStamp();
	
    return uiEUTRA;
}
//UserLocationInformation
void uplink_make_userLocationInformation(Ngap_UserLocationInformation_t	 *UserLocationInformation)
{
    UserLocationInformation->present  = Ngap_UserLocationInformation_PR_userLocationInformationEUTRA;
    UserLocationInformation->choice.userLocationInformationEUTRA  = uplink_fill_userLocationInformationEUTRA();
    
}

//make_userLocationInformationEUTRA()
Ngap_UserLocationInformation_t *uplink_make_UserLocationInformation_ie()
{
	Ngap_UplinkNASTransport_IEs_t *ie;
	ie = calloc(1, sizeof(Ngap_UplinkNASTransport_IEs_t));

	ie->id = Ngap_ProtocolIE_ID_id_UserLocationInformation;
	ie->criticality = Ngap_Criticality_reject;
	ie->value.present = Ngap_UplinkNASTransport_IEs__value_PR_UserLocationInformation;

	uplink_make_userLocationInformation(&ie->value.choice.UserLocationInformation);
	
    return ie;
}


void add_NGUplinkNasTransport_ie(Ngap_UplinkNASTransport_t *ngapUplinkNasTransport, Ngap_UplinkNASTransport_IEs_t *ie) 
{
    int ret;
	ret = ASN_SEQUENCE_ADD(&ngapUplinkNasTransport->protocolIEs.list, ie);
    if ( ret != 0 ) {
        fprintf(stderr, "Failed to add ie\n");
    }
}
Ngap_NGAP_PDU_t *make_NGAP_UplinkNasTransport()
{
	Ngap_NGAP_PDU_t *pdu = NULL;
	pdu = calloc(1, sizeof(Ngap_NGAP_PDU_t));
	memset(pdu,  0 , sizeof(Ngap_NGAP_PDU_t));
	
	pdu->present = Ngap_NGAP_PDU_PR_initiatingMessage;
	pdu->choice.initiatingMessage = calloc(1, sizeof(Ngap_InitiatingMessage_t));
	pdu->choice.initiatingMessage->procedureCode = Ngap_ProcedureCode_id_UplinkNASTransport;
	pdu->choice.initiatingMessage->criticality = Ngap_Criticality_reject;
	pdu->choice.initiatingMessage->value.present = Ngap_InitiatingMessage__value_PR_UplinkNASTransport;
	
	Ngap_UplinkNASTransport_t *ngapUplinkNasTransport;
	ngapUplinkNasTransport = &pdu->choice.initiatingMessage->value.choice.UplinkNASTransport;
	
    // Make Ngap_UplinkNASTransport_t IEs and add it to message
	Ngap_UplinkNASTransport_IEs_t *ie;

	//Ngap_AMF_UE_NGAP_ID_t	 AMF_UE_NGAP_ID;
	unsigned long AMF_UE_NGAP_ID = 0x80;
    ie  =  make_AMF_UE_NGAP_ID(AMF_UE_NGAP_ID);
	add_NGUplinkNasTransport_ie(ngapUplinkNasTransport, ie);
	
	
	//Ngap_RAN_UE_NGAP_ID_t	 RAN_UE_NGAP_ID;
	unsigned long RAN_UE_NGAP_ID = 0x81;
	ie  =  make_RAN_UE_NGAP_ID(RAN_UE_NGAP_ID);
	add_NGUplinkNasTransport_ie(ngapUplinkNasTransport, ie);
  
	
	//Ngap_NAS_PDU_t NAS_PDU;//identity response

	
	//Ngap_UserLocationInformation_t	 UserLocationInformation;
	ie = uplink_make_UserLocationInformation_ie();
    add_NGInitialUeMessage_ie(ngapUplinkNasTransport, ie);

	return pdu;
}
