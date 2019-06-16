#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "ngap_amf_downlink_nas_transport.h"


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

#include  "bstrlib.h"
#include  "intertask_interface_types.h"

#include  "Ngap_CriticalityDiagnostics-IE-Item.h"
#include  "Ngap_PLMNSupportItem.h"

#include "nas_message.h"
//#include "mm_msg.h"
#include "bstrlib.h"
#include "mmData.h"
#include "common_types.h"
#include "common_defs.h"
#include "log.h"


#define DOWNLINK_BUFF_LEN 256
void downlink_nas_transport_with_auth_request(uint8_t *data)
{
    OAILOG_FUNC_IN (LOG_NGAP);
    OAILOG_DEBUG(LOG_NGAP,"encode authentication request dump--------");
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
	mm_msg->header.message_type = AUTHENTICATION_REQUEST;
	  
	memset (&mm_msg->specific_msg.authentication_request,		0, sizeof (authentication_request_msg));
	  
	mm_msg->specific_msg.authentication_request.naskeysetidentifier.tsc = 1;
	mm_msg->specific_msg.authentication_request.naskeysetidentifier.naskeysetidentifier = 0b101;
	  
	bstring abba = bfromcstralloc(10, "\0");
	uint8_t bitStream_abba = 0b00110100;
	abba->data = (unsigned char *)(&bitStream_abba);
	abba->slen = 1; 
	mm_msg->specific_msg.authentication_request.abba = abba;
	  
	bstring rand = bfromcstralloc(10, "\0");
	uint8_t bitStream_rand = 0b00110111;
	rand->data = (unsigned char *)(&bitStream_rand);
	rand->slen = 1;
	  
	mm_msg->specific_msg.authentication_request.presence = 0x07;
	mm_msg->specific_msg.authentication_request.authenticationparameterrand = rand;
	mm_msg->specific_msg.authentication_request.authenticationparameterautn = abba;
	mm_msg->specific_msg.authentication_request.eapmessage = abba;
		
	size += MESSAGE_TYPE_MAXIMUM_LENGTH;
	  
	nas_msg.security_protected.plain.mm = *mm_msg;
	  
		
	  
	//construct security context
	fivegmm_security_context_t * security = calloc(1,sizeof(fivegmm_security_context_t));
	security->selected_algorithms.encryption = NAS_SECURITY_ALGORITHMS_NEA1;
	security->dl_count.overflow = 0xffff;
	security->dl_count.seq_num =  0x23;
	security->knas_enc[0] = 0x14;
	security->selected_algorithms.integrity = NAS_SECURITY_ALGORITHMS_NIA1;
	security->knas_int[0] = 0x41;
	//complete sercurity context
	  
	int length = DOWNLINK_BUFF_LEN;
	//unsigned char data[UPLINK_BUFF_LEN] = {'\0'};
	  
	bstring  info = bfromcstralloc(length, "\0");//info the nas_message_encode result
	
	#if 0
		printf("1 start nas_message_encode \n");
		printf("security %p\n",security);
		printf("info %p\n",info);
	#endif
	
	OAILOG_DEBUG(LOG_NGAP,"nas header encode extended_protocol_discriminator:0x%x, security_header_type:0x%x,sequence_number:0x%x,message_authentication_code:0x%x",
	nas_msg.header.extended_protocol_discriminator,
	nas_msg.header.security_header_type,
	nas_msg.header.sequence_number,
	nas_msg.header.message_authentication_code);
	
	OAILOG_DEBUG(LOG_NGAP,"message type:0x%x",mm_msg->header.message_type);
	OAILOG_DEBUG(LOG_NGAP,"naskey tsc:0x%x",mm_msg->specific_msg.authentication_request.naskeysetidentifier.tsc);
	OAILOG_DEBUG(LOG_NGAP,"naskey tsc:0x%x",mm_msg->specific_msg.authentication_request.naskeysetidentifier.naskeysetidentifier);
	OAILOG_DEBUG(LOG_NGAP,"abba buffer:0x%x",*(unsigned char *)((mm_msg->specific_msg.authentication_request.abba)->data));
	OAILOG_DEBUG(LOG_NGAP,"rand buffer:0x%x",*(unsigned char *)((mm_msg->specific_msg.authentication_request.authenticationparameterrand)->data));
	OAILOG_DEBUG(LOG_NGAP,"autn buffer:0x%x",*(unsigned char *)((mm_msg->specific_msg.authentication_request.authenticationparameterautn)->data));
	OAILOG_DEBUG(LOG_NGAP,"eap message buffer:0x%x",*(unsigned char *)((mm_msg->specific_msg.authentication_request.eapmessage)->data));
	
	//bytes = nas_message_encode (data, &nas_msg, 60/*don't know the size*/, security);
	bytes = nas_message_encode (data, &nas_msg, DOWNLINK_BUFF_LEN/*don't know the size*/, security);

}


//AMF_UE_NGAP_ID
Ngap_DownlinkNASTransport_IEs_t * downlink_make_AMF_UE_NGAP_ID(unsigned long AMF_UE_NGAP_ID)
{
   	Ngap_DownlinkNASTransport_IEs_t *ie;
	ie = calloc(1, sizeof(Ngap_DownlinkNASTransport_IEs_t));
	
	ie->id = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
	ie->criticality = Ngap_Criticality_reject;
	ie->value.present = Ngap_DownlinkNASTransport_IEs__value_PR_AMF_UE_NGAP_ID;
	
	asn_ulong2INTEGER(&ie->value.choice.AMF_UE_NGAP_ID, AMF_UE_NGAP_ID);
	
	OAILOG_DEBUG(LOG_NGAP,"AMF_UE_NGAP_ID:");
	size_t i  = 0;
	for(i ; i<ie->value.choice.AMF_UE_NGAP_ID.size;i++)
	{
	    OAILOG_DEBUG(LOG_NGAP,"0x%x",ie->value.choice.AMF_UE_NGAP_ID.buf[i]);
	}
	return ie;	
}

//RAN_UE_NGAP_ID

Ngap_DownlinkNASTransport_IEs_t * downlink_make_RAN_UE_NGAP_ID(unsigned long rAN_UE_NGAP_ID)
{
	Ngap_DownlinkNASTransport_IEs_t *ie;
	ie = calloc(1, sizeof(Ngap_DownlinkNASTransport_IEs_t));

	ie->id = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
	ie->criticality = Ngap_Criticality_reject;
	ie->value.present = Ngap_DownlinkNASTransport_IEs__value_PR_RAN_UE_NGAP_ID;
	ie->value.choice.RAN_UE_NGAP_ID = rAN_UE_NGAP_ID;

	OAILOG_DEBUG(LOG_NGAP,"RAN_UE_NGAP_ID:%lu",ie->value.choice.RAN_UE_NGAP_ID);
	

	return ie;
}
//NAS_PDU
Ngap_DownlinkNASTransport_IEs_t *downlink_make_NAS_PDU()
{   
	Ngap_DownlinkNASTransport_IEs_t *ie = NULL;
	ie  = calloc(1, sizeof(Ngap_DownlinkNASTransport_IEs_t));
	memset(ie, 0, sizeof(Ngap_DownlinkNASTransport_IEs_t));
			  
	ie->id = Ngap_ProtocolIE_ID_id_NAS_PDU;
	ie->criticality = Ngap_Criticality_reject;
	ie->value.present = Ngap_DownlinkNASTransport_IEs__value_PR_NAS_PDU;
	  
	uint8_t  *data = calloc(DOWNLINK_BUFF_LEN, sizeof(uint8_t ));
	memset(data, 0, DOWNLINK_BUFF_LEN );
	downlink_nas_transport_with_auth_request(data);
	  
    OCTET_STRING_fromBuf (&ie->value.choice.NAS_PDU, data,  DOWNLINK_BUFF_LEN); 
	return ie;
}

void add_NGDownlinkNasTransport_ie(Ngap_DownlinkNASTransport_t *ngapDownlinkNasTransport, Ngap_DownlinkNASTransport_IEs_t *ie) 
{
    int ret;
	ret = ASN_SEQUENCE_ADD(&ngapDownlinkNasTransport->protocolIEs.list, ie);
    if ( ret != 0 ) {
        fprintf(stderr, "Failed to add ie\n");
    }
}

Ngap_NGAP_PDU_t *make_NGAP_DownlinkNasTransport()
{
	Ngap_NGAP_PDU_t *pdu = NULL;
	pdu = calloc(1, sizeof(Ngap_NGAP_PDU_t));
	memset(pdu,  0 , sizeof(Ngap_NGAP_PDU_t));
		
	pdu->present = Ngap_NGAP_PDU_PR_initiatingMessage;
	pdu->choice.initiatingMessage = calloc(1, sizeof(Ngap_InitiatingMessage_t));
	pdu->choice.initiatingMessage->procedureCode = Ngap_ProcedureCode_id_DownlinkNASTransport;
	pdu->choice.initiatingMessage->criticality = Ngap_Criticality_reject;
	pdu->choice.initiatingMessage->value.present = Ngap_InitiatingMessage__value_PR_DownlinkNASTransport;
		
	Ngap_DownlinkNASTransport_t *ngapDownLinkNasTransport = NULL;
	ngapDownLinkNasTransport = &pdu->choice.initiatingMessage->value.choice.DownlinkNASTransport;
		
	// Make Ngap_downlinkNASTransport_t IEs and add it to message
	Ngap_DownlinkNASTransport_IEs_t *ie;

	//Ngap_AMF_UE_NGAP_ID_t	 AMF_UE_NGAP_ID;
	unsigned long AMF_UE_NGAP_ID = 0x80;
    ie  =  downlink_make_AMF_UE_NGAP_ID(AMF_UE_NGAP_ID);
	add_NGDownlinkNasTransport_ie(ngapDownLinkNasTransport, ie);
	
	
	//Ngap_RAN_UE_NGAP_ID_t	 RAN_UE_NGAP_ID;
	unsigned long RAN_UE_NGAP_ID = 0x81;
	ie  =  downlink_make_RAN_UE_NGAP_ID(RAN_UE_NGAP_ID);
	add_NGDownlinkNasTransport_ie(ngapDownLinkNasTransport, ie);
  
	
	//Ngap_NAS_PDU_t NAS_PDU;//identity response
    //Ngap_ProtocolIE_ID_id_NAS_PDU
    ie  = downlink_make_NAS_PDU();
	add_NGDownlinkNasTransport_ie(ngapDownLinkNasTransport, ie);

	return pdu;
}
