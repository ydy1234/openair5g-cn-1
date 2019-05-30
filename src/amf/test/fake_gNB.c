#include <stdio.h>
#include <stdlib.h>



#include "bstrlib.h"
#include "intertask_interface_types.h"
#include "ngap_amf_encoder.h"
#include "sctp_primitives_client.h"
#include "intertask_interface.h"
#include "assertions.h"
#include "intertask_interface_init.h"
#include "sctp_primitives_server.h"
#include "ngap_amf.h"
#include "amf_app.h"
#include "log.h"
#include "ngap_common.h"
#include "bstrlib.h"


void
send_NGAP_SetupRequest()
{

	int assoc[1];
	sctp_data_t * sctp_data_p = NULL;
	char *local_ip_addr[] = {"192.168.2.122"};
	char remote_ip_addr[] = "192.168.2.122";
	//char *local_ip_addr[] = {"127.0.0.1"};
	//char remote_ip_addr[] = "127.0.0.1";

	Ngap_NGAP_PDU_t pdu;

	uint8_t * buffer_p = NULL;
	uint32_t length = 0;

	Ngap_NGSetupRequest_t *ngapSetupRequest;
	Ngap_NGSetupRequestIEs_t *ngapSetupRequestIEs;

	memset(&pdu, 0, sizeof(pdu));
    
	//for NGSetupRequest
	pdu.present = Ngap_NGAP_PDU_PR_initiatingMessage;
	pdu.choice.initiatingMessage = calloc(1, sizeof(Ngap_InitiatingMessage_t));
	pdu.choice.initiatingMessage->procedureCode = Ngap_ProcedureCode_id_NGSetup;
	pdu.choice.initiatingMessage->criticality = Ngap_Criticality_reject;
	pdu.choice.initiatingMessage->value.present = Ngap_InitiatingMessage__value_PR_NGSetupRequest;
	ngapSetupRequest = &(pdu.choice.initiatingMessage->value.choice.NGSetupRequest);

	ngapSetupRequestIEs = calloc(1, sizeof(Ngap_NGSetupRequestIEs_t));
	ngapSetupRequestIEs->id = Ngap_ProtocolIE_ID_id_GlobalRANNodeID;
	ngapSetupRequestIEs->criticality = Ngap_Criticality_reject;
	ngapSetupRequestIEs->value.present = Ngap_NGSetupRequestIEs__value_PR_GlobalRANNodeID;

	Ngap_GlobalRANNodeID_t *ngap_GlobalRANNodeID;
	ngap_GlobalRANNodeID = &ngapSetupRequestIEs->value.choice.GlobalRANNodeID;
	ngap_GlobalRANNodeID->present = Ngap_GlobalRANNodeID_PR_globalGNB_ID;
	ngap_GlobalRANNodeID->choice.globalGNB_ID = calloc(1, sizeof(struct Ngap_GlobalGNB_ID));

	uint8_t plmn[3] = { 0x02, 0xF8, 0x29 };
	OCTET_STRING_fromBuf(&ngap_GlobalRANNodeID->choice.globalGNB_ID->pLMNIdentity, (const char*)plmn, 3);

	ngap_GlobalRANNodeID->choice.globalGNB_ID->gNB_ID.present = Ngap_GNB_ID_PR_gNB_ID;
	uint8_t gNB_ID[4] = { 0x01, 0x02, 0x03, 0x04 };
	ngap_GlobalRANNodeID->choice.globalGNB_ID->gNB_ID.choice.gNB_ID.buf = calloc(4, sizeof(uint8_t));
	ngap_GlobalRANNodeID->choice.globalGNB_ID->gNB_ID.choice.gNB_ID.size = 4;
	memcpy(ngap_GlobalRANNodeID->choice.globalGNB_ID->gNB_ID.choice.gNB_ID.buf, gNB_ID, 4);
	ASN_SEQUENCE_ADD(&ngapSetupRequest->protocolIEs, ngapSetupRequestIEs);


	//RANNodeName
	ngapSetupRequestIEs = calloc(1, sizeof(Ngap_NGSetupRequestIEs_t));
	ngapSetupRequestIEs->id = Ngap_ProtocolIE_ID_id_RANNodeName;
	ngapSetupRequestIEs->criticality = Ngap_Criticality_reject;
	ngapSetupRequestIEs->value.present = Ngap_NGSetupRequestIEs__value_PR_RANNodeName;
	OCTET_STRING_fromBuf (&ngapSetupRequestIEs->value.choice.RANNodeName, "gNB1 Eurecom", strlen ("gNB1 Eurecom"));
	ASN_SEQUENCE_ADD(&ngapSetupRequest->protocolIEs, ngapSetupRequestIEs);

	//supportedTAList
    ngapSetupRequestIEs = calloc(1, sizeof(Ngap_NGSetupRequestIEs_t));
    ngapSetupRequestIEs->id =Ngap_ProtocolIE_ID_id_SupportedTAList;
    ngapSetupRequestIEs->criticality = Ngap_Criticality_reject;
    ngapSetupRequestIEs->value.present = Ngap_NGSetupRequestIEs__value_PR_SupportedTAList;

    Ngap_SupportedTAItem_t *ta;
    ta = (Ngap_SupportedTAItem_t *)calloc(1, sizeof(ta));
    uint8_t tAC[3] = {0x01, 0x02, 0x03};
    OCTET_STRING_fromBuf(&ta->tAC, (const char*)tAC, 3);
    Ngap_BroadcastPLMNItem_t *broadcastPLMNItem;
    Ngap_SliceSupportItem_t *sliceSupportItem;
    sliceSupportItem = calloc (1, sizeof(sliceSupportItem));
    sliceSupportItem->s_NSSAI.sD = calloc(1, sizeof (sliceSupportItem->s_NSSAI.sD));
    uint8_t sST[1] = {0x03};
    uint8_t sD[3] = {0x30, 0x33, 0x01};
    OCTET_STRING_fromBuf(&sliceSupportItem->s_NSSAI.sST, (const char*)sST, 1);
    OCTET_STRING_fromBuf(sliceSupportItem->s_NSSAI.sD, (const char*)sD, 3);
    broadcastPLMNItem = (Ngap_BroadcastPLMNItem_t *)calloc (1, sizeof(broadcastPLMNItem));
    //memset (&broadcastPLMNItem, 0, sizeof(Ngap_BroadcastPLMNItem_t));

    OCTET_STRING_fromBuf(&broadcastPLMNItem->pLMNIdentity, (const char*)plmn, 3);
    ASN_SEQUENCE_ADD (&broadcastPLMNItem->tAISliceSupportList.list, &sliceSupportItem);
    ASN_SEQUENCE_ADD (&ta->broadcastPLMNList.list, broadcastPLMNItem);

    ASN_SEQUENCE_ADD (&ngapSetupRequestIEs->value.choice.SupportedTAList.list, ta);
    //ASN_SEQUENCE_ADD(&ngapSetupRequest->protocolIEs, ngapSetupRequestIEs);


	//PagingDRX
	ngapSetupRequestIEs = calloc(1, sizeof(Ngap_NGSetupRequestIEs_t));
	ngapSetupRequestIEs->id =Ngap_ProtocolIE_ID_id_DefaultPagingDRX;
	ngapSetupRequestIEs->criticality = Ngap_Criticality_reject;
	ngapSetupRequestIEs->value.present = Ngap_NGSetupRequestIEs__value_PR_PagingDRX;
	ngapSetupRequestIEs->value.choice.PagingDRX = Ngap_PagingDRX_v256;
	ASN_SEQUENCE_ADD(&ngapSetupRequest->protocolIEs, ngapSetupRequestIEs);

	int enc_rval = ngap_amf_encode_pdu (&pdu, &buffer_p, &length);

   #if 0
        Ngap_NGAP_PDU_t  decoded_pdu;
        Ngap_NGAP_PDU_t * ppdu = &decoded_pdu;
        asn_dec_rval_t rc = asn_decode(NULL,ATS_ALIGNED_CANONICAL_PER,&asn_DEF_Ngap_NGAP_PDU,(void**)&ppdu,buffer_p,length);
        printf("decode result(%d)\n",rc);
        printf("decoded message present(%d)\n",decoded_pdu.present);
        switch(decoded_pdu.present){
          case Ngap_NGAP_PDU_PR_initiatingMessage:
            printf("precedureCode(%d)\n",decoded_pdu.choice.initiatingMessage->procedureCode);
            printf("message type(%d)\n",decoded_pdu.choice.initiatingMessage->value.present);
        } 
   #endif
 

	 MessagesIds message_id = MESSAGES_ID_MAX;
     Ngap_NGAP_PDU_t decoded_pdu = {0};
     
	 bstring b = blk2bstr(buffer_p, length);
	 
     ngap_amf_decode_pdu(&decoded_pdu, b,  &message_id);
     ngap_amf_handle_message(0,0,&decoded_pdu);
     //printf("decoded_pdu precedureCode(%d)\n",decoded_pdu.choice.initiatingMessage->procedureCode);
     printf("over\n");
    #if 0
	//connect to sctp and send message to AMF
	sctp_data_p = (sctp_data_t *) calloc (1, sizeof(sctp_data_t));
	if (sctp_data_p == NULL)  exit(1);
	assoc[0] = sctp_connect_to_remote_host (local_ip_addr, 1, remote_ip_addr, 36412, SOCK_STREAM, sctp_data_p);
	sctp_send_msg (sctp_data_p, 60, 0, buffer_p,length);
    #endif

}



void
send_NGAP_SetupFailure()
{

	int assoc[1];
	sctp_data_t * sctp_data_p = NULL;
	char *local_ip_addr[] = {"127.0.0.1"};
	char remote_ip_addr[] = "127.0.0.1";
	Ngap_NGAP_PDU_t                         pdu;
	uint8_t * buffer_p = NULL;
	uint32_t length = 0;

	Ngap_NGSetupFailure_t                       *ngapSetupFailure;
	memset(&pdu, 0, sizeof(pdu));

	//for NGSetupFailure
	pdu.present = Ngap_NGAP_PDU_PR_unsuccessfulOutcome;
	pdu.choice.unsuccessfulOutcome = calloc(1, sizeof(Ngap_UnsuccessfulOutcome_t));
	pdu.choice.unsuccessfulOutcome->procedureCode = Ngap_ProcedureCode_id_NGSetup;
	pdu.choice.unsuccessfulOutcome->criticality = Ngap_Criticality_reject;
	pdu.choice.unsuccessfulOutcome->value.present = Ngap_UnsuccessfulOutcome__value_PR_NGSetupFailure;
	ngapSetupFailure = &pdu.choice.unsuccessfulOutcome->value.choice.NGSetupFailure;
	int enc_rval = ngap_amf_encode_pdu (&pdu, &buffer_p, &length);


	//connect to sctp and send message to AMF
	sctp_data_p = (sctp_data_t *) calloc (1, sizeof(sctp_data_t));
	if (sctp_data_p == NULL)  exit(1);
	assoc[0] = sctp_connect_to_remote_host (local_ip_addr, 1, remote_ip_addr, 36412, SOCK_STREAM, sctp_data_p);
	sctp_send_msg (sctp_data_p, 60, 0, buffer_p,length);


}
int main(
		int argc,
		char * argv[])
{

	/*
	  int nb_gNB = 1;
	  int assoc[1];
	  sctp_data_t * sctp_data_p = NULL;
	  sctp_data_p = (sctp_data_t *) calloc (1, sizeof(sctp_data_t));
	  if (sctp_data_p == NULL)  exit(1);

	  char *local_ip_addr[] = {"127.0.0.1"};
	  char remote_ip_addr[] = "127.0.0.1";

	  //test SCTP
	  //open a socket to AMF's SCTP
	  for (int i = 0; i < nb_gNB; i++) {
	    assoc[i] = sctp_connect_to_remote_host (local_ip_addr, 1, remote_ip_addr, 36412, SOCK_STREAM, sctp_data_p);
	  }
	 */


	//Test NGAP messages
	send_NGAP_SetupRequest();

	//send_NGAP_SetupFailure();
	//test NGAP
	//test gNB
/*
	while (1) {
		sleep (1);
	}

	return (0);
*/
}
