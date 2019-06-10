#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include  "ngap_amf_uplink_nas_transport.h"

#include "Ngap_ProtocolIE-Field.h"
#include "Ngap_BroadcastPLMNItem.h"
#include "Ngap_GNB-ID.h"
#include "Ngap_GlobalGNB-ID.h"
#include "Ngap_PagingDRX.h"
#include "Ngap_SliceSupportItem.h"
#include "Ngap_SupportedTAItem.h"
#include "Ngap_GlobalRANNodeID.h"
#include "Ngap_SuccessfulOutcome.h"

#include "sctp_gNB_defs.h"

#include  "bstrlib.h"
#include  "intertask_interface_types.h"


Ngap_UplinkNASTransport_IEs_t * make_AMF_UE_NGAP_ID(unsigned int AMF_UE_NGAP_ID)
{
   	Ngap_UplinkNASTransport_IEs_t *ie;
	ie = calloc(1, sizeof(Ngap_UplinkNASTransport_IEs_t));

    Ngap_UplinkNASTransport_IEs__value_PR_AMF_UE_NGAP_ID
	ie->id = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
	ie->criticality = Ngap_Criticality_reject;
	ie->value.present = Ngap_UplinkNASTransport_IEs__value_PR_AMF_UE_NGAP_ID;
	
    ie->value.choice.AMF_UE_NGAP_ID.buf  = calloc(sizeof(AMF_UE_NGAP_ID), sizeof(uint8_t));
	memcpy(ie->value.choice.AMF_UE_NGAP_ID, &AMF_UE_NGAP_ID, sizeof(AMF_UE_NGAP_ID));
	ie->value.choice.AMF_UE_NGAP_ID.size = sizeof(AMF_UE_NGAP_ID);


	return ie;
	
}
void add_NGUplinkNasTransport_ie(Ngap_UplinkNASTransport_t *ngapUplinkNasTransport, Ngap_NGSetupRequestIEs_t *ie) 
{
    int ret;
	ret = ASN_SEQUENCE_ADD(&ngapUplinkNasTransport->protocolIEs.list, ie);
    if ( ret != 0 ) {
        fprintf(stderr, "Failed to add ie\n");
    }
}
void  make_AMF_UE_NGAP_ID

Ngap_NGAP_PDU_t *make_NGAP_UplinkNasTransport()
{
	Ngap_NGAP_PDU_t *pdu;
	pdu = calloc(1, sizeof(Ngap_NGAP_PDU_t));
	
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
	unsigned int AMF_UE_NGAP_ID = 0x80;
    ie  =  make_AMF_UE_NGAP_ID(AMF_UE_NGAP_ID);
	add_NGUplinkNasTransport_ie(ngapUplinkNasTransport, ie);
	
	//Ngap_RAN_UE_NGAP_ID_t	 RAN_UE_NGAP_ID;
	unsigned int RAN_UE_NGAP_ID = 0x81;
	//ie  =  make_RAN_UE_NGAP_ID(RAN_UE_NGAP_ID);
	add_NGUplinkNasTransport_ie(ngapUplinkNasTransport, ie);
	//Ngap_NAS_PDU_t	 NAS_PDU;
	//Ngap_UserLocationInformation_t	 UserLocationInformation;
		
	

	#if 0
    ie = make_GlobalRANNodeID_ie();
    add_NGSetupRequest_ie(ngapSetupRequest, ie);

    ie = make_RANNodeName_ie("test gNB");
    add_NGSetupRequest_ie(ngapSetupRequest, ie);

    ie = make_DefaultPagingDRX_ie(Ngap_PagingDRX_v128);
    add_NGSetupRequest_ie(ngapSetupRequest, ie);

    ie = make_supportedTAList();
    add_NGSetupRequest_ie(ngapSetupRequest, ie);
    #endif
}
