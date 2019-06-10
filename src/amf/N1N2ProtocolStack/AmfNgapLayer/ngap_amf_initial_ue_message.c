#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "ngap_amf_initial_ue_message.h"
#include "Ngap_ProtocolIE-Field.h"
#include "Ngap_InitiatingMessage.h"
#include "Ngap_BroadcastPLMNItem.h"
#include "Ngap_GNB-ID.h"
#include "Ngap_GlobalGNB-ID.h"
#include "Ngap_PagingDRX.h"
#include "Ngap_SliceSupportItem.h"
#include "Ngap_SupportedTAItem.h"
#include "Ngap_UnsuccessfulOutcome.h"
#include "Ngap_GlobalRANNodeID.h"

#include "sctp_gNB_defs.h"

#include  "bstrlib.h"
#include  "intertask_interface_types.h"

#include  "Ngap_CriticalityDiagnostics-IE-Item.h"
#include  "Ngap_EUTRACellIdentity.h"
#include  "Ngap_TimeStamp.h"
#include  "Ngap_EUTRA-CGI.h"
#include  "Ngap_UserLocationInformationEUTRA.h"
#include  "Ngap_AllowedNSSAI-Item.h"


//RAN_UE_NGAP_ID
Ngap_InitialUEMessage_IEs_t  *make_RAN_UE_NGAP_ID_ie(Ngap_RAN_UE_NGAP_ID_t  RAN_UE_NGAP_ID)
{
    Ngap_InitialUEMessage_IEs_t *ie = NULL;
    ie = calloc(1, sizeof(Ngap_InitialUEMessage_IEs_t));
	memset(ie, 0, sizeof(Ngap_InitialUEMessage_IEs_t));
	
    ie->id = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID; 
	ie->value.present = Ngap_InitialUEMessage_IEs__value_PR_RAN_UE_NGAP_ID;
	ie->value.choice.RAN_UE_NGAP_ID = 0x80;
	printf("RAN_UE_NGAP_ID:0x%x\n", ie->value.choice.RAN_UE_NGAP_ID);

	return ie;
}

//userLocationInformationEUTRA
//CGI
//pLMNIdentity
//eUTRACellIdentity

void fill_eUTRA_with_CGI_with_pLMNIdentity(Ngap_PLMNIdentity_t *pLMNIdentity)
{
     uint8_t plmn[3] = { 0x02, 0xF8, 0x29 };
	OCTET_STRING_fromBuf(pLMNIdentity, (const char*)plmn, 3);

	printf("pLMNIdentity: 0x%x,0x%x,0x%x\n", pLMNIdentity->buf[0],pLMNIdentity->buf[1],pLMNIdentity->buf[2]);
}
void fill_eUTRA_with_CGI_with_eUTRACellIdentity(Ngap_EUTRACellIdentity_t	 *eUTRACellIdentity, int index)
{
    uint32_t ei = htonl(index);
	eUTRACellIdentity->buf = calloc(4, sizeof(uint8_t));
	eUTRACellIdentity->size = 4;
	memcpy(eUTRACellIdentity->buf, &ei, 4);

    printf("eUTRACellIdentity: 0x%x,0x%x,0x%x,0x%x\n",
	eUTRACellIdentity->buf[0],eUTRACellIdentity->buf[1],
	eUTRACellIdentity->buf[2],eUTRACellIdentity->buf[3]);
}

void fill_userLocationInformationEUTRA_eUTRA_with_CGI(Ngap_EUTRA_CGI_t	 *eUTRA_CGI)
{
    fill_eUTRA_with_CGI_with_pLMNIdentity(&eUTRA_CGI->pLMNIdentity);
	fill_eUTRA_with_CGI_with_eUTRACellIdentity(&eUTRA_CGI->eUTRACellIdentity, 0x79);
    
}

void fill_tAI_with_pLMNIdentity(Ngap_PLMNIdentity_t *pLMNIdentity)
{
    uint8_t plmn[3] = { 0x02, 0xF8, 0x29 };
	OCTET_STRING_fromBuf(pLMNIdentity, (const char*)plmn, 3);

	printf("pLMNIdentity: 0x%x,0x%x,0x%x\n", pLMNIdentity->buf[0],pLMNIdentity->buf[1],pLMNIdentity->buf[2]);
}
void fill_tAI_with_tAC(Ngap_TAC_t	 *tAC)
{
	uint8_t tac[3] = { 0x01, 0xF8, 0x29 };
	OCTET_STRING_fromBuf(tAC, (const char*)tac, 3);
	
    printf("tAC: 0x%x,0x%x,0x%x\n", tAC->buf[0],tAC->buf[1],tAC->buf[2]);
}
void fill_userLocationInformationEUTRA_eUTRA_with_tAI(Ngap_TAI_t	 *tAI)
{
    fill_tAI_with_pLMNIdentity(&tAI->pLMNIdentity);
	fill_tAI_with_tAC(&tAI->tAC);
}
Ngap_TimeStamp_t	* fill_userLocationInformationEUTRA_eUTRA_with_timeStamp()
{
    
    Ngap_TimeStamp_t	*timeStamp = NULL;
	timeStamp = calloc(1,sizeof(Ngap_TimeStamp_t));
    memset(timeStamp, 0 , sizeof(Ngap_TimeStamp_t));
    
	#if 0
    timeStamp->buf = calloc(4, sizeof(uint8_t));
	
	uint8_t ts[4] = { 0x01, 0xF8, 0x29, 0x06};
	timeStamp.size  = 4;
	memcpy(&timeStamp->buf, ts, 4);
	
	printf("timeStamp:x%x,0x%x,0x%x,0x%x\n", 
	timeStamp->buf[0],timeStamp->buf[1],
	timeStamp->buf[2],timeStamp->buf[3]);
	#endif
   
    uint8_t st[4] = { 0x02, 0xF8, 0x29, 0x06 };
	OCTET_STRING_fromBuf(timeStamp, (const char*)st, 4);

	printf("timeStamp: 0x%x,0x%x,0x%x,0x%x\n", timeStamp->buf[0],timeStamp->buf[1],timeStamp->buf[2],timeStamp->buf[3]);
	
	return timeStamp;
}

Ngap_UserLocationInformationEUTRA_t   * fill_userLocationInformationEUTRA()
{
    Ngap_UserLocationInformationEUTRA_t   *uiEUTRA = NULL;
	uiEUTRA = calloc(1, sizeof(Ngap_UserLocationInformationEUTRA_t));
	memset(uiEUTRA, 0 , sizeof(Ngap_UserLocationInformationEUTRA_t));

	fill_userLocationInformationEUTRA_eUTRA_with_CGI(&uiEUTRA->eUTRA_CGI);
    fill_userLocationInformationEUTRA_eUTRA_with_tAI(&uiEUTRA->tAI);
	uiEUTRA->timeStamp = fill_userLocationInformationEUTRA_eUTRA_with_timeStamp();
	
    return uiEUTRA;
}
//UserLocationInformation
void make_userLocationInformation(Ngap_UserLocationInformation_t	 *UserLocationInformation)
{
    UserLocationInformation->present  = Ngap_UserLocationInformation_PR_userLocationInformationEUTRA;
    UserLocationInformation->choice.userLocationInformationEUTRA  = fill_userLocationInformationEUTRA();
}

//make_userLocationInformationEUTRA()
Ngap_UserLocationInformation_t *make_UserLocationInformation_ie()
{
	Ngap_InitialUEMessage_IEs_t *ie = NULL;
    ie  = calloc(1, sizeof(Ngap_InitialUEMessage_IEs_t));
	memset(ie, 0, sizeof(Ngap_InitialUEMessage_IEs_t));

	ie->id = Ngap_ProtocolIE_ID_id_UserLocationInformation; 
	ie->value.present = Ngap_InitialUEMessage_IEs__value_PR_UserLocationInformation;


    #if 0
    Ngap_PLMNSupportItem_t  *plmn = NULL;
    plmn= make_PLMNSupportItem();

	ASN_SEQUENCE_ADD(&ie->value.choice.PLMNSupportList.list, plmn);
    #endif
	
	make_userLocationInformation(&ie->value.choice.UserLocationInformation);
	
    return ie;
}

Ngap_InitialUEMessage_IEs_t *make_RRCEstablishmentCause_ie(Ngap_RRCEstablishmentCause_t	 RRCEstablishmentCause)
{
    Ngap_InitialUEMessage_IEs_t *ie = NULL;
    ie  = calloc(1, sizeof(Ngap_InitialUEMessage_IEs_t));
	memset(ie, 0, sizeof(Ngap_InitialUEMessage_IEs_t));

	ie->id = Ngap_ProtocolIE_ID_id_RRCEstablishmentCause; 
	ie->value.present = Ngap_InitialUEMessage_IEs__value_PR_RRCEstablishmentCause;
    ie->value.choice.RRCEstablishmentCause = RRCEstablishmentCause;

	return ie;
}
//FiveG_S_TMSI
//aMFSetID
void fill_aMFSetID(Ngap_AMFSetID_t *aMFSetID, uint8_t setid, uint32_t len)
{
    aMFSetID->buf = calloc(len, sizeof(uint8_t));
	memcpy(aMFSetID->buf, &setid, len);
	aMFSetID->size =  len;
}
//aMFPointer
void fill_aMFPointer(Ngap_AMFPointer_t *aMFPointer, uint8_t ap, uint32_t len)
{
	aMFPointer->buf = calloc(len, sizeof(uint8_t));
	memcpy(aMFPointer->buf, &ap, len);
	aMFPointer->size =  len;
}

//fiveG_TMSI
void fill_fiveG_TMSI(Ngap_FiveG_TMSI_t *fiveG_TMSI,const char *tmsi)
{
    OCTET_STRING_fromBuf (fiveG_TMSI, tmsi, strlen (tmsi));  
}
Ngap_InitialUEMessage_IEs_t *make_FiveG_S_TMSI_ie()
{
    Ngap_InitialUEMessage_IEs_t *ie = NULL;
    ie  = calloc(1, sizeof(Ngap_InitialUEMessage_IEs_t));
	memset(ie, 0, sizeof(Ngap_InitialUEMessage_IEs_t));

	ie->id = Ngap_ProtocolIE_ID_id_FiveG_S_TMSI; 
	ie->value.present = Ngap_InitialUEMessage_IEs__value_PR_FiveG_S_TMSI;
    
    uint8_t setid[] = {0x79};
	fill_aMFSetID(&ie->value.choice.FiveG_S_TMSI.aMFSetID, setid, sizeof(setid)/ sizeof(setid[0]));
	uint8_t aMFPointer[] = {0x80};
    fill_aMFPointer(&ie->value.choice.FiveG_S_TMSI.aMFPointer, aMFPointer, sizeof(aMFPointer)/sizeof(aMFPointer[0]));
    fill_fiveG_TMSI(&ie->value.choice.FiveG_S_TMSI.fiveG_TMSI, "test_tmsi");
     
	return ie;
}

void fill_AMFSetID(Ngap_AMFSetID_t *AMFSetID, uint8_t setID, uint32_t len)
{
    AMFSetID->buf = calloc(len, sizeof(uint8_t));
    memcpy(AMFSetID->buf, setID, len);
	AMFSetID->size = len;
}
Ngap_InitialUEMessage_IEs_t * make_AMFSetID_ie(uint8_t setID, uint32_t len)
{
	Ngap_InitialUEMessage_IEs_t *ie = NULL;
	ie  = calloc(1, sizeof(Ngap_InitialUEMessage_IEs_t));
	memset(ie, 0, sizeof(Ngap_InitialUEMessage_IEs_t));
	
	ie->id = Ngap_ProtocolIE_ID_id_AMFSetID; 
	ie->value.present = Ngap_InitialUEMessage_IEs__value_PR_AMFSetID;

    fill_AMFSetID(&ie->value.choice.AMFSetID, setID, len);
	
	return ie;
}
Ngap_InitialUEMessage_IEs_t * make_UEContextRequest_ie(Ngap_UEContextRequest_t	 UEContextRequest)
{
	Ngap_InitialUEMessage_IEs_t *ie = NULL;
	ie	= calloc(1, sizeof(Ngap_InitialUEMessage_IEs_t));
	memset(ie, 0, sizeof(Ngap_InitialUEMessage_IEs_t));
		
	ie->id = Ngap_ProtocolIE_ID_id_UEContextRequest; 
	ie->value.present = Ngap_InitialUEMessage_IEs__value_PR_UEContextRequest;

	ie->value.choice.UEContextRequest = UEContextRequest;
	return ie;
}

 Ngap_AllowedNSSAI_Item_t * fill_AllowedNSSAI_Item()
 {
     uint8_t sST[1] = {0x80};
	 uint8_t sd[3] = {0x01, 0x02,0x03};
	 
     Ngap_AllowedNSSAI_Item_t *item = NULL;
	 item = calloc(1, sizeof(Ngap_AllowedNSSAI_Item_t));
	 
     memcpy(&item->s_NSSAI.sST, sST, 1);

     Ngap_SD_t *ngsD = calloc(1, sizeof(Ngap_SD_t));
     memcpy(ngsD, sd, 3);
	 
	 item->s_NSSAI.sD  = ngsD;
	 
     return item;   
 }

Ngap_InitialUEMessage_IEs_t *  make_AllowedNSSAI_ie()
{
	Ngap_InitialUEMessage_IEs_t *ie = NULL;
	ie	= calloc(1, sizeof(Ngap_InitialUEMessage_IEs_t));
	memset(ie, 0, sizeof(Ngap_InitialUEMessage_IEs_t));
			
	ie->id = Ngap_ProtocolIE_ID_id_AllowedNSSAI; 
	ie->value.present = Ngap_InitialUEMessage_IEs__value_PR_AllowedNSSAI;

	Ngap_AllowedNSSAI_Item_t *item1;
	
    item1 = fill_AllowedNSSAI_Item();

	ASN_SEQUENCE_ADD(&ie->value.choice.AllowedNSSAI.list, item1);
	
}

void add_NGInitialUeMessage_ie(Ngap_NGSetupResponse_t *ngapSetupResponse, Ngap_NGSetupResponseIEs_t *ie)
	{
    int ret;
	ret = ASN_SEQUENCE_ADD(&ngapSetupResponse->protocolIEs.list, ie);
    if ( ret != 0 ) {
        fprintf(stderr, "Failed to add ie\n");
    }
}

Ngap_NGAP_PDU_t *make_NGAP_InitialUEMessage()
{
    printf("\n\nNGAP_Initial_UE_Message-------------encode\n");

	Ngap_NGAP_PDU_t              *pdu = NULL;
	sctp_data_t * sctp_data_p = NULL;
	uint8_t * buffer_p = NULL;
	uint32_t length = 0;
   
	Ngap_InitialUEMessage_t						*ngapInitialUeMsg= NULL;
	Ngap_InitialUEMessage_IEs_t					*ie = NULL;

	pdu = calloc(1, sizeof(Ngap_NGAP_PDU_t));
	memset(pdu, 0, sizeof(Ngap_NGAP_PDU_t));
	
	//for NG_initial_ue_message
	pdu->present = Ngap_NGAP_PDU_PR_initiatingMessage;
	pdu->choice.initiatingMessage = calloc(1, sizeof(Ngap_InitiatingMessage_t));
	pdu->choice.initiatingMessage->procedureCode = Ngap_ProcedureCode_id_InitialUEMessage;
	pdu->choice.initiatingMessage->criticality = Ngap_Criticality_reject;
	pdu->choice.initiatingMessage->value.present = Ngap_InitiatingMessage__value_PR_InitialUEMessage;
	ngapInitialUeMsg = &pdu->choice.initiatingMessage->value.choice.InitialUEMessage;


	//RAN_UE_NGAP_ID
	ie  = make_RAN_UE_NGAP_ID_ie(0x80);
	add_NGInitialUeMessage_ie(ngapInitialUeMsg, ie);

	//Ngap_NAS_PDU_t	 NAS_PDU
	
	//Ngap_UserLocationInformation_t	 UserLocationInformation;
    ie = make_UserLocationInformation_ie();
    add_NGInitialUeMessage_ie(ngapInitialUeMsg, ie);

	
	//Ngap_RRCEstablishmentCause_t	 RRCEstablishmentCause;
	ie  = make_RRCEstablishmentCause_ie(0x79);
	add_NGInitialUeMessage_ie(ngapInitialUeMsg, ie);
	
	//Ngap_FiveG_S_TMSI_t	 FiveG_S_TMSI;
	ie = make_FiveG_S_TMSI_ie();
	add_NGInitialUeMessage_ie(ngapInitialUeMsg, ie);
	
	//Ngap_AMFSetID_t	 AMFSetID;
	uint8_t amfsetid[] = {0x81};
	ie = make_AMFSetID_ie(amfsetid, sizeof(amfsetid)/sizeof(amfsetid));
	add_NGInitialUeMessage_ie(ngapInitialUeMsg, ie);
	
	//Ngap_UEContextRequest_t	 UEContextRequest;
	ie = make_UEContextRequest_ie(0x82);
	add_NGInitialUeMessage_ie(ngapInitialUeMsg, ie);
	
	//Ngap_AllowedNSSAI_t	 AllowedNSSAI;
	ie = make_AllowedNSSAI_ie();
	add_NGInitialUeMessage_ie(ngapInitialUeMsg, ie);

    #if 0
	int enc_rval = ngap_amf_encode_pdu (pdu, &buffer_p, &length);

	MessagesIds message_id = MESSAGES_ID_MAX;
    Ngap_NGAP_PDU_t decoded_pdu = {0};
     
	bstring b = blk2bstr(buffer_p, length);
	 
	printf("NGAP_Initial_UE_Message-------------decode\n");
	
    ngap_amf_decode_pdu(&decoded_pdu, b,  &message_id);
    ngap_amf_handle_message(0,0,&decoded_pdu);
    #endif

    return  pdu;   
}

