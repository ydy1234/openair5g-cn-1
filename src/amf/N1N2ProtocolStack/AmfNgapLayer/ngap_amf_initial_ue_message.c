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


#include  "nas_message.h"
#include  "mmData.h"
#include  "common_defs.h"


#define BUFF_LEN  256

#include "conversions.h"

void reg_request(uint8_t *data, uint32_t  bufLen)
{
    
	printf("REGISTRATION_REQUEST------------ start\n");
	int size = NAS_MESSAGE_SECURITY_HEADER_SIZE; 
	int bytes = 0;
	  
	nas_message_t  nas_msg;
	memset (&nas_msg,		0, sizeof (nas_message_t));
	  
	nas_msg.header.extended_protocol_discriminator = FIVEGS_MOBILITY_MANAGEMENT_MESSAGES;
	nas_msg.header.security_header_type = SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED_CYPHERED;
	uint8_t sequencenumber = 0xfe;
	//uint32_t mac = 0xffffeeee;
	uint32_t mac = 0xffee;
	nas_msg.header.sequence_number = sequencenumber;
	nas_msg.header.message_authentication_code= mac;
	  
	nas_msg.security_protected.header = nas_msg.header;
	  
	MM_msg * mm_msg = &nas_msg.plain.mm;
	mm_msg->header.extended_protocol_discriminator = FIVEGS_MOBILITY_MANAGEMENT_MESSAGES;
	mm_msg->header.security_header_type = SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED_CYPHERED;
	mm_msg->header.message_type = REGISTRATION_REQUEST;
	  
	memset (&mm_msg->specific_msg.registration_request, 0, sizeof (registration_request_msg));
	
	mm_msg->specific_msg.registration_request._5gsregistrationtype.is_for = true;
	mm_msg->specific_msg.registration_request._5gsregistrationtype.registration_type = 0x07;
	
		
	mm_msg->specific_msg.registration_request.naskeysetidentifier.tsc = 1;
	mm_msg->specific_msg.registration_request.naskeysetidentifier.naskeysetidentifier = 0b101;
		
	mm_msg->specific_msg.registration_request.presence = 0x07;
		
	mm_msg->specific_msg.registration_request.non_current_native_nas_key_set_identifier.tsc =  1;
	mm_msg->specific_msg.registration_request.non_current_native_nas_key_set_identifier.naskeysetidentifier = 4;
	
	   
	mm_msg->specific_msg.registration_request._5gmmcapability.is_HO_supported =  1;
	mm_msg->specific_msg.registration_request._5gmmcapability.is_LPP_supported = 0;
	
	mm_msg->specific_msg.registration_request._5gmmcapability.is_S1_mode_supported = 1;
	
		
	mm_msg->specific_msg.registration_request.uesecuritycapability.nea = 0x11;
	mm_msg->specific_msg.registration_request.uesecuritycapability.nia = 0x22;
		
	//NSSAI nssai;
	mm_msg->specific_msg.registration_request._5gstrackingareaidentity.mcc = 1;
	mm_msg->specific_msg.registration_request._5gstrackingareaidentity.mnc = 2;
	mm_msg->specific_msg.registration_request._5gstrackingareaidentity.tac = 3;
	
		
	mm_msg->specific_msg.registration_request.s1uenetworkcapability.eea = 1;
	mm_msg->specific_msg.registration_request.s1uenetworkcapability.eia = 2;
	
		
	mm_msg->specific_msg.registration_request.uplinkdatastatus = 0x01;
	mm_msg->specific_msg.registration_request.pdusessionstatus = 0x02;
	mm_msg->specific_msg.registration_request.micoindication.raai = 0x1;
	mm_msg->specific_msg.registration_request.uestatus.n1_mode_reg = 1;
	mm_msg->specific_msg.registration_request.uestatus.s1_mode_reg = 0;
		
	//_5GSMobileIdentity AdditionalGUTI;
	mm_msg->specific_msg.registration_request.allowedpdusessionstatus =  0x01;
	mm_msg->specific_msg.registration_request.uesusagesetting = 0x01;
	mm_msg->specific_msg.registration_request._5gsdrxparameters = 0x02;
	
		
		
	bstring eps = bfromcstralloc(10, "\0");
	uint8_t bitStream_eps = 0b00110100;
	eps->data = (unsigned char *)(&bitStream_eps);
	eps->slen = 1; 
		
	mm_msg->specific_msg.registration_request.epsnasmessagecontainer = eps;
		
	//LADNIndication ladnindication;
	mm_msg->specific_msg.registration_request.payloadcontainertype = 0x01;
	
	bstring pay = bfromcstralloc(10, "\0");
	uint8_t bitStream_pay = 0b00110100;
	pay->data = (unsigned char *)(&bitStream_pay);
	pay->slen = 1; 
		
	mm_msg->specific_msg.registration_request.payloadcontainer = pay;
		
		
	mm_msg->specific_msg.registration_request.networkslicingindication.dcni  = 0;
	mm_msg->specific_msg.registration_request.networkslicingindication.nssci = 1;
	mm_msg->specific_msg.registration_request._5gsupdatetype.ng_ran_rcu = 0x22;
	mm_msg->specific_msg.registration_request._5gsupdatetype.sms_requested = 0x11;
	
	bstring nas = bfromcstralloc(10, "\0");
	uint8_t bitStream_nas = 0b00110100;
	nas->data = (unsigned char *)(&bitStream_nas);
	nas->slen = 1; 
		
	mm_msg->specific_msg.registration_request.nasmessagecontainer = nas;
	   
	size += MESSAGE_TYPE_MAXIMUM_LENGTH;
	  
	nas_msg.security_protected.plain.mm = *mm_msg;
	  
	//complete mm msg content
	
	  
	//construct security context
	fivegmm_security_context_t * security = calloc(1,sizeof(fivegmm_security_context_t));
	security->selected_algorithms.encryption = NAS_SECURITY_ALGORITHMS_NEA1;
	security->dl_count.overflow = 0xffff;
	security->dl_count.seq_num =  0x23;
	security->knas_enc[0] = 0x14;
	security->selected_algorithms.integrity = NAS_SECURITY_ALGORITHMS_NIA1;
	security->knas_int[0] = 0x41;
	//complete sercurity context
	  
	int length = BUFF_LEN;
	//unsigned char *data = calloc(BUFF_LEN, sizeof(unsigned char));
	//memset(data, 0, BUF_LEN *sizeof(unsigned char));
	  
	//bstring  info = bfromcstralloc(length, "\0");//info the nas_message_encode result
	
	#if 0
		printf("1 start nas_message_encode \n");
		printf("security %p\n",security);
		printf("info %p\n",info);
	#endif
	
	printf("encode-------------------------\n");
	printf("nas header encode extended_protocol_discriminator:0x%x\n, security_header_type:0x%x\n,sequence_number:0x%x\n,message_authentication_code:0x%x\n",
	nas_msg.header.extended_protocol_discriminator,
	nas_msg.header.security_header_type,
	nas_msg.header.sequence_number,
	nas_msg.header.message_authentication_code);
	
	printf("message type:0x%x\n",mm_msg->header.message_type);
	printf("_5gsregistrationtype :is_for:0x%x,reg_type:0x%x\n",
	mm_msg->specific_msg.registration_request._5gsregistrationtype.is_for,
	mm_msg->specific_msg.registration_request._5gsregistrationtype.registration_type);
		
	printf("naskeysetidentifier: tsc:0x%x,naskeysetidentifier:0x%x\n",
	mm_msg->specific_msg.registration_request.naskeysetidentifier.tsc,
	mm_msg->specific_msg.registration_request.naskeysetidentifier.naskeysetidentifier);
	
	printf("presence:0x%x\n",mm_msg->specific_msg.registration_request.presence);
	printf("non_current_native_nas_key_set_identifier: tsc:0x%x,naskeysetidentifier:0x%x\n",
	mm_msg->specific_msg.registration_request.non_current_native_nas_key_set_identifier.tsc,
	mm_msg->specific_msg.registration_request.non_current_native_nas_key_set_identifier.naskeysetidentifier);
	
	printf("_5gmmcapability: is_HO_supported:0x%x,is_LPP_supported:0x%x,is_S1_mode_supported:0x%x\n",
	mm_msg->specific_msg.registration_request._5gmmcapability.is_HO_supported,
	mm_msg->specific_msg.registration_request._5gmmcapability.is_LPP_supported,
	mm_msg->specific_msg.registration_request._5gmmcapability.is_S1_mode_supported);
	
	printf("uesecuritycapability nea:0x%x,nia:0x%x\n",
	mm_msg->specific_msg.registration_request.uesecuritycapability.nea,
	mm_msg->specific_msg.registration_request.uesecuritycapability.nia);
		
	//NSSAI nssai;
	printf("_5gstrackingareaidentity mcc:0x%x, mnc:0x%x,tac:0x%x\n",
	mm_msg->specific_msg.registration_request._5gstrackingareaidentity.mcc,
	mm_msg->specific_msg.registration_request._5gstrackingareaidentity.mnc,
	mm_msg->specific_msg.registration_request._5gstrackingareaidentity.tac);
	
	printf("s1uenetworkcapability eea:0x%x, eai:0x%x\n",
	mm_msg->specific_msg.registration_request.s1uenetworkcapability.eea,
	mm_msg->specific_msg.registration_request.s1uenetworkcapability.eia);
	
	printf("uplinkdatastatus:0x%x\n",
	mm_msg->specific_msg.registration_request.uplinkdatastatus);
	printf("pdusessionstatus:0x%x\n",
	mm_msg->specific_msg.registration_request.pdusessionstatus);
		
	printf("micoindication.raai:0x%x\n",
	mm_msg->specific_msg.registration_request.micoindication.raai);
		
	printf("uestatus: n1_mode_reg:0x%x,s1_mode_reg:0x%x\n",
	mm_msg->specific_msg.registration_request.uestatus.n1_mode_reg,
	mm_msg->specific_msg.registration_request.uestatus.s1_mode_reg);
		
	//_5GSMobileIdentity AdditionalGUTI;
	
	printf("allowedpdusessionstatus:0x%x\n",
	mm_msg->specific_msg.registration_request.allowedpdusessionstatus);
	printf("uesusagesetting:0x%x\n",
	mm_msg->specific_msg.registration_request.uesusagesetting);
	printf("_5gsdrxparameters:0x%x\n",
	mm_msg->specific_msg.registration_request._5gsdrxparameters);
	
	printf("eapmessage:0x%x\n",*(unsigned char *)((mm_msg->specific_msg.registration_request.epsnasmessagecontainer)->data));
	//LADNIndication ladnindication;
	printf("payloadcontainertype:0x%x\n",mm_msg->specific_msg.registration_request.payloadcontainertype);
	printf("payloadcontainer:0x%x\n",*(unsigned char *)((mm_msg->specific_msg.registration_request.payloadcontainer)->data));
	   
	printf("networkslicingindication,dcni:0x%x,nssci:0x%x\n",
	mm_msg->specific_msg.registration_request.networkslicingindication.dcni,
	mm_msg->specific_msg.registration_request.networkslicingindication.nssci);
		
	printf("_5gsupdatetype ng_ran_rcu:0x%x, sms_requested:0x%x\n",
	
	mm_msg->specific_msg.registration_request._5gsupdatetype.ng_ran_rcu,
	mm_msg->specific_msg.registration_request._5gsupdatetype.sms_requested);
	
	 
	printf("nasmessagecontainer:0x%x\n",*(unsigned char *)((mm_msg->specific_msg.registration_request.nasmessagecontainer)->data));
	//bytes = nas_message_encode (data, &nas_msg, 60/*don't know the size*/, security);
	bytes = nas_message_encode (data, &nas_msg, bufLen/*don't know the size*/, security);

	int i  = 0;
	for(; i<30; i++)
		printf("encode 0x%x\n", data[i]);

	
	//info->data = data;
	//memcpy(info->data, data, BUFF_LEN * sizeof(unsigned char ) -1);
	//info->slen = BUFF_LEN * sizeof(unsigned char )  -1 ;
}

//RAN_UE_NGAP_ID
Ngap_InitialUEMessage_IEs_t  *make_RAN_UE_NGAP_ID_ie(Ngap_RAN_UE_NGAP_ID_t  RAN_UE_NGAP_ID)
{
    Ngap_InitialUEMessage_IEs_t *ie = NULL;
    ie = calloc(1, sizeof(Ngap_InitialUEMessage_IEs_t));
	memset(ie, 0, sizeof(Ngap_InitialUEMessage_IEs_t));
	
    ie->id = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
	ie->criticality = Ngap_Criticality_reject;
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
	eUTRACellIdentity->bits_unused = 0x04;

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
	ie->criticality = Ngap_Criticality_reject;
	ie->value.present = Ngap_InitialUEMessage_IEs__value_PR_UserLocationInformation;


    #if 0
    Ngap_PLMNSupportItem_t  *plmn = NULL;
    plmn= make_PLMNSupportItem();

	ASN_SEQUENCE_ADD(&ie->value.choice.PLMNSupportList.list, plmn);
    #endif
	
	make_userLocationInformation(&ie->value.choice.UserLocationInformation);
	
    return ie;
}

Ngap_InitialUEMessage_IEs_t *make_RRCEstablishmentCause_ie(e_Ngap_RRCEstablishmentCause	 cause)
{
    #if 0
    Ngap_InitialUEMessage_IEs_t *ie = NULL;
    ie  = calloc(1, sizeof(Ngap_InitialUEMessage_IEs_t));
	memset(ie, 0, sizeof(Ngap_InitialUEMessage_IEs_t));

	ie->id = Ngap_ProtocolIE_ID_id_RRCEstablishmentCause;
	ie->criticality = Ngap_Criticality_reject;
	ie->value.present = Ngap_InitialUEMessage_IEs__value_PR_RRCEstablishmentCause;
    ie->value.choice.RRCEstablishmentCause = RRCEstablishmentCause;

    printf("RRCEstablishmentCause:%ld\n", ie->value.choice.RRCEstablishmentCause);
    #endif

    Ngap_InitialUEMessage_IEs_t *ie = NULL;
    ie = calloc(1, sizeof(Ngap_InitialUEMessage_IEs_t));
	memset(ie, 0, sizeof(Ngap_InitialUEMessage_IEs_t));
	
    ie->id = Ngap_ProtocolIE_ID_id_RRCEstablishmentCause;
	ie->criticality = Ngap_Criticality_reject;
	ie->value.present = Ngap_InitialUEMessage_IEs__value_PR_RRCEstablishmentCause;
	ie->value.choice.RRCEstablishmentCause = cause;
	printf("RRCEstablishmentCause:0x%x\n", ie->value.choice.RRCEstablishmentCause);
	
	return ie;
	
}
//FiveG_S_TMSI
//aMFSetID
void fill_aMFSetID(Ngap_AMFSetID_t *aMFSetID, uint8_t setid, uint32_t len)
{
    aMFSetID->buf = calloc(len, sizeof(uint8_t));
	memset(aMFSetID->buf, 0, len );
	memcpy(aMFSetID->buf, &setid, len);
	aMFSetID->size =  len;
	aMFSetID->bits_unused = 0x0E;

	printf("aMFSetID:aMFSetID->size:%d,0x%x,0x%x\n", aMFSetID->size, aMFSetID->buf[0],aMFSetID->buf[1]);
}
//aMFPointer
void fill_aMFPointer(Ngap_AMFPointer_t *aMFPointer, uint8_t ap, uint32_t len)
{
	aMFPointer->buf = calloc(len, sizeof(uint8_t));
	memset(aMFPointer->buf, 0, sizeof(len));
	memcpy(aMFPointer->buf, &ap, len);
	aMFPointer->size =  len;
	aMFPointer->bits_unused = 0x02;
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
	ie->criticality = Ngap_Criticality_reject;
	ie->value.present = Ngap_InitialUEMessage_IEs__value_PR_FiveG_S_TMSI;
    
    uint8_t setid[] = {0x00,0x08};
	uint8_t aMFPointer[] = {0x08};
	fill_aMFSetID(&ie->value.choice.FiveG_S_TMSI.aMFSetID, setid, sizeof(setid)/ sizeof(setid[0]));
    
    fill_aMFPointer(&ie->value.choice.FiveG_S_TMSI.aMFPointer, aMFPointer, sizeof(aMFPointer)/sizeof(aMFPointer[0]));
  
	fill_fiveG_TMSI(&ie->value.choice.FiveG_S_TMSI.fiveG_TMSI, "tmsi");
	return ie;
}

void fill_AMFSetID(Ngap_AMFSetID_t *aMFSetID, uint8_t setid, uint32_t len)
{
    aMFSetID->buf = calloc(len, sizeof(uint8_t));
	memset(aMFSetID->buf, 0, len );
	memcpy(aMFSetID->buf, &setid, len);
	aMFSetID->size =  len;
	aMFSetID->bits_unused = 0x0E;

	printf("aMFSetID:aMFSetID->size:%d,0x%x,0x%x\n", aMFSetID->size, aMFSetID->buf[0],aMFSetID->buf[1]);

	#if 0
    //uint32_t amf_ID = htonl(index);
	//uint8_t tmp = 1;
	printf("1111111111111 sizeof(index):%d, 0x%x\n", sizeof(index), index);
	AMFSetID->buf = calloc(2, sizeof(uint8_t));
	memset(AMFSetID->buf,0, 2);
	AMFSetID->size = 2;
	memcpy(AMFSetID->buf, &index, 2);
	AMFSetID->bits_unused = 0x0E;
    #endif
	//printf("1111111111111 sizeof(index):%d, 0x%x\n", sizeof(index), index);
	
    //uint32_t tmp = 0x0400;
    //INT32_TO_BIT_STRING(tmp, AMFSetID);

    //AMFSetID->buf = calloc(len, sizeof(uint8_t));
    //memcpy(AMFSetID->buf, setID, len);
	//AMFSetID->size = len;
	
	//printf("AMFSetID:0x%x,0x%x,0x%x,0x%x\n",AMFSetID->buf[0],AMFSetID->buf[1],AMFSetID->buf[2],AMFSetID->buf[3]);
}
Ngap_InitialUEMessage_IEs_t * make_AMFSetID_ie(uint8_t setid, uint32_t len)
{
	Ngap_InitialUEMessage_IEs_t *ie = NULL;
	ie  = calloc(1, sizeof(Ngap_InitialUEMessage_IEs_t));
	memset(ie, 0, sizeof(Ngap_InitialUEMessage_IEs_t));
	
	ie->id = Ngap_ProtocolIE_ID_id_AMFSetID;
	ie->criticality = Ngap_Criticality_reject;
	ie->value.present = Ngap_InitialUEMessage_IEs__value_PR_AMFSetID;

    fill_AMFSetID(&ie->value.choice.AMFSetID, setid, len);
	
	return ie;
}
Ngap_InitialUEMessage_IEs_t * make_UEContextRequest_ie(e_Ngap_UEContextRequest	 UEContextRequest)
{
	Ngap_InitialUEMessage_IEs_t *ie = NULL;
	ie	= calloc(1, sizeof(Ngap_InitialUEMessage_IEs_t));
	memset(ie, 0, sizeof(Ngap_InitialUEMessage_IEs_t));
		
	ie->id = Ngap_ProtocolIE_ID_id_UEContextRequest; 
	ie->criticality = Ngap_Criticality_reject;
	ie->value.present = Ngap_InitialUEMessage_IEs__value_PR_UEContextRequest;

	ie->value.choice.UEContextRequest = UEContextRequest;
	return ie;
}

 Ngap_AllowedNSSAI_Item_t * fill_AllowedNSSAI_Item()
 {
     uint8_t sst[1] = {0x80};
	 uint8_t sd[3] = {0x01, 0x02,0x03};
	 
     Ngap_AllowedNSSAI_Item_t *item = NULL;
	 item = calloc(1, sizeof(Ngap_AllowedNSSAI_Item_t));

     OCTET_STRING_fromBuf(&item->s_NSSAI.sST, &sst, 1);

	 Ngap_SD_t *sD = calloc(1, sizeof(Ngap_SD_t));
     item->s_NSSAI.sD = sD;
     OCTET_STRING_fromBuf(sD, sd, 3);
   
     return item;   
 }

Ngap_InitialUEMessage_IEs_t *  make_AllowedNSSAI_ie()
{
	Ngap_InitialUEMessage_IEs_t *ie = NULL;
	ie	= calloc(1, sizeof(Ngap_InitialUEMessage_IEs_t));
	memset(ie, 0, sizeof(Ngap_InitialUEMessage_IEs_t));
			
	ie->id = Ngap_ProtocolIE_ID_id_AllowedNSSAI; 
	ie->criticality = Ngap_Criticality_reject;
	ie->value.present = Ngap_InitialUEMessage_IEs__value_PR_AllowedNSSAI;

	Ngap_AllowedNSSAI_Item_t *item1;
	
    item1 = fill_AllowedNSSAI_Item();

	ASN_SEQUENCE_ADD(&ie->value.choice.AllowedNSSAI.list, item1);

	return ie;
}
Ngap_InitialUEMessage_IEs_t * make_NAS_PDU_ie()
{
	Ngap_InitialUEMessage_IEs_t *ie = NULL;
	ie	= calloc(1, sizeof(Ngap_InitialUEMessage_IEs_t));
	memset(ie, 0, sizeof(Ngap_InitialUEMessage_IEs_t));
			
	ie->id = Ngap_ProtocolIE_ID_id_NAS_PDU;
	ie->criticality = Ngap_Criticality_reject;
	ie->value.present = Ngap_InitialUEMessage_IEs__value_PR_NAS_PDU;

	//ie->value.choice.NAS_PDU = bfromcstralloc(BUFF_LEN, "\0");

	uint8_t  *data = calloc(BUFF_LEN, sizeof(uint8_t ));
	memset(data, 0, BUFF_LEN );
	reg_request(data, BUFF_LEN );
	
	OCTET_STRING_fromBuf (&ie->value.choice.NAS_PDU, data,  BUFF_LEN); 
	
    return ie;
}


void add_NGInitialUeMessage_ie(Ngap_InitialUEMessage_t *ngapInitialUeMsg, Ngap_InitialUEMessage_t *ie)
	{
    int ret;
	ret = ASN_SEQUENCE_ADD(&ngapInitialUeMsg->protocolIEs.list, ie);
    if ( ret != 0 ) {
        fprintf(stderr, "Failed to add ie\n");
    }
}

Ngap_NGAP_PDU_t *make_NGAP_InitialUEMessage()
{
    printf("\n\nNGAP_Initial_UE_Message encode----------- start\n");

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
	//unsigned char data[BUFF_LEN] = {0};

    //bstring  info;// = bfromcstralloc(BUFF_LEN, "\0");//info the nas_message_encode result
	
	ie  = make_NAS_PDU_ie();
	add_NGInitialUeMessage_ie(ngapInitialUeMsg, ie);
	
	//Ngap_UserLocationInformation_t	 UserLocationInformation;
    ie = make_UserLocationInformation_ie();
    add_NGInitialUeMessage_ie(ngapInitialUeMsg, ie);

	
	//Ngap_RRCEstablishmentCause_t	 RRCEstablishmentCause;
	ie  = make_RRCEstablishmentCause_ie(Ngap_RRCEstablishmentCause_emergency);
	add_NGInitialUeMessage_ie(ngapInitialUeMsg, ie);

	
	//Ngap_FiveG_S_TMSI_t	 FiveG_S_TMSI;
	ie = make_FiveG_S_TMSI_ie();
	add_NGInitialUeMessage_ie(ngapInitialUeMsg, ie);

	
	//Ngap_AMFSetID_t	 AMFSetID;
	
	uint8_t setid[] = {0x00,0x08};
	ie = make_AMFSetID_ie(setid, sizeof(setid)/sizeof(setid[0]));
	add_NGInitialUeMessage_ie(ngapInitialUeMsg, ie);

   
	//Ngap_UEContextRequest_t	 UEContextRequest;
	ie = make_UEContextRequest_ie(Ngap_UEContextRequest_requested);
	add_NGInitialUeMessage_ie(ngapInitialUeMsg, ie);
	
	//Ngap_AllowedNSSAI_t	 AllowedNSSAI;
	ie = make_AllowedNSSAI_ie();
	add_NGInitialUeMessage_ie(ngapInitialUeMsg, ie);

    #if 0
    int i  =  0;
	for(; i<30; i++)
		printf("test 0x%x\n", info->data[i]);
   
   
	MessagesIds message_id = MESSAGES_ID_MAX;
    Ngap_NGAP_PDU_t decoded_pdu = {0};
     
	bstring b = blk2bstr(data, sizeof(data));
	 
	printf("NGAP_Initial_UE_Message-------------decode\n");

    
	
    //ngap_amf_decode_pdu(&decoded_pdu, b,  &message_id);
    //ngap_amf_handle_message(0,0,&decoded_pdu);
    #endif

	printf("\n\nNGAP_Initial_UE_Message encode----------- end\n");
    return  pdu;   
}

