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


#include "nas_message.h"
//#include "mm_msg.h"
#include "bstrlib.h"
#include "mmData.h"
#include "common_types.h"
#include "common_defs.h"


#define UPLINK_BUFF_LEN 256
void uplink_nas_transport_with_auth_response(uint8_t *data)
{
    printf("AUTHENTICATION_RESPONSE------------ start\n");
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
	mm_msg->header.message_type = AUTHENTICATION_RESPONSE;
  
	memset (&mm_msg->specific_msg.authentication_response,		0, sizeof (authentication_response_msg));
  
   
   
	bstring param = bfromcstralloc(10, "\0");
	uint8_t bitStream_rand = 0b00110110;
	param->data = (unsigned char *)(&bitStream_rand);
	param->slen = 1;

	bstring eapmsg = bfromcstralloc(10, "\0");
	uint8_t bitStream_eap = 0b00110101;
	eapmsg->data = (unsigned char *)(&bitStream_eap);
	eapmsg->slen = 1; 
	
  
	mm_msg->specific_msg.authentication_response.presence = 0x07;
	mm_msg->specific_msg.authentication_response.authenticationresponseparameter = param;
	mm_msg->specific_msg.authentication_response.eapmessage = eapmsg;
	
	size += MESSAGE_TYPE_MAXIMUM_LENGTH;
  
	nas_msg.security_protected.plain.mm = *mm_msg;
  
	//complete mm msg content
	if(size <= 0){
	  return -1;
	}
  
	//construct security context
	fivegmm_security_context_t * security = calloc(1,sizeof(fivegmm_security_context_t));
	security->selected_algorithms.encryption = NAS_SECURITY_ALGORITHMS_NEA1;
	security->dl_count.overflow = 0xffff;
	security->dl_count.seq_num =  0x23;
	security->knas_enc[0] = 0x14;
	security->selected_algorithms.integrity = NAS_SECURITY_ALGORITHMS_NIA1;
	security->knas_int[0] = 0x41;
	//complete sercurity context
  
	int length = UPLINK_BUFF_LEN;
	//unsigned char data[BUF_LEN] = {'\0'};
  
	//bstring  info = bfromcstralloc(length, "\0");//info the nas_message_encode result

	#if 0
	printf("1 start nas_message_encode \n");
	printf("security %p\n",security);
	printf("info %p\n",info);
	#endif

	printf("encode-----------------\n");
	printf("nas header encode extended_protocol_discriminator:0x%x,\nsecurity_header_type:0x%x,\nsequence_number:0x%x,\nmessage_authentication_code:0x%x,\n",
	nas_msg.header.extended_protocol_discriminator,
	nas_msg.header.security_header_type,
	nas_msg.header.sequence_number,
	nas_msg.header.message_authentication_code);

	printf("message type:0x%x\n",mm_msg->header.message_type);
	printf("presence:0x%x\n",mm_msg->specific_msg.authentication_response.presence);
	printf("param:0x%x\n",*(unsigned char *)((mm_msg->specific_msg.authentication_response.authenticationresponseparameter)->data));
	printf("eap message buffer:0x%x\n",*(unsigned char *)((mm_msg->specific_msg.authentication_response.eapmessage)->data));

	//bytes = nas_message_encode (data, &nas_msg, 60/*don't know the size*/, security);
	bytes = nas_message_encode (data, &nas_msg, UPLINK_BUFF_LEN/*don't know the size*/, security);

}
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

Ngap_UplinkNASTransport_IEs_t *make_NAS_PDU()
{
   Ngap_InitialUEMessage_IEs_t *ie = NULL;
	ie	= calloc(1, sizeof(Ngap_InitialUEMessage_IEs_t));
	memset(ie, 0, sizeof(Ngap_InitialUEMessage_IEs_t));
			
	ie->id = Ngap_ProtocolIE_ID_id_NAS_PDU;
	ie->criticality = Ngap_Criticality_reject;
	ie->value.present = Ngap_UplinkNASTransport_IEs__value_PR_NAS_PDU;
    
	uint8_t  *data = calloc(UPLINK_BUFF_LEN, sizeof(uint8_t ));
	memset(data, 0, UPLINK_BUFF_LEN );
	uplink_nas_transport_with_auth_response(data);
	
	OCTET_STRING_fromBuf (&ie->value.choice.NAS_PDU, data,  UPLINK_BUFF_LEN); 

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
    //Ngap_ProtocolIE_ID_id_NAS_PDU
    ie  = make_NAS_PDU();
	add_NGUplinkNasTransport_ie(ngapUplinkNasTransport, ie);
	
	//Ngap_UserLocationInformation_t	 UserLocationInformation;
	ie = uplink_make_UserLocationInformation_ie();
    add_NGUplinkNasTransport_ie(ngapUplinkNasTransport, ie);

	return pdu;
}
