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
	char *local_ip_addr[] = {"127.0.0.1"};
	char remote_ip_addr[] = "127.0.0.1";

	uint8_t                                *buffer;
	//uint32_t                                length;
	Ngap_NGAP_PDU_t                         pdu;
	Ngap_SupportedTAItem_t                       ta;
	Ngap_PLMNIdentity_t  plmnIdentity;
	uint8_t * buffer_p = NULL;
	uint32_t length = 0;

	//Ngap_NGSetupRequest_t                       *ngapSetupRequest;
	//Ngap_NGSetupRequestIEs_t					*ngapSetupRequestIEs;
   	Ngap_NGSetupFailure_t                       *ngapSetupFailure;
	memset(&pdu, 0, sizeof(pdu));

	pdu.present = Ngap_NGAP_PDU_PR_unsuccessfulOutcome;
	pdu.choice.unsuccessfulOutcome = calloc(1, sizeof(Ngap_UnsuccessfulOutcome_t));
	pdu.choice.unsuccessfulOutcome->procedureCode = Ngap_ProcedureCode_id_NGSetup;
	pdu.choice.unsuccessfulOutcome->criticality = Ngap_Criticality_reject;
	pdu.choice.unsuccessfulOutcome->value.present = Ngap_UnsuccessfulOutcome__value_PR_NGSetupFailure;
	ngapSetupFailure = &pdu.choice.unsuccessfulOutcome->value.choice.NGSetupFailure;
    int enc_rval = ngap_amf_encode_pdu (&pdu, &buffer_p, &length);

/*
	ngap_message                            mesasge;
	NGAP_PDU_t  							  pdu;

	ngap_message message = {0};
	NGSetupRequestIEs_t * ng_setup_request_p = NULL;


	uint8_t * buffer_p = NULL;
	uint32_t length = 0;

	ng_setup_request_p = &message.msg.ngSetupRequestIEs;
	message.procedureCode = Ngap_ProcedureCode_id_NGSetup;
	message.direction = NGAP_PDU_PR_initiatingMessage;

	ng_setup_request_p->presenceMask |= NGSETUPREQUESTIES_RANNODENAME_PRESENT;
	OCTET_STRING_fromBuf (&ng_setup_request_p->ranNodeName, "gNB1", strlen ("gNB1"));
	ng_setup_request_p->globalRANNodeID.present =  GlobalRANNodeID_PR_globalGNB_ID;

	ng_setup_request_p->globalRANNodeID.choice.globalGNB_ID.gNB_ID.present = GNB_ID_PR_gNB_ID;
	ng_setup_request_p->globalRANNodeID.choice.globalGNB_ID.gNB_ID.choice.gNB_ID.size  = 5;//for testing purpose only
	ng_setup_request_p->globalRANNodeID.choice.globalGNB_ID.gNB_ID.choice.gNB_ID.bits_unused =0;
	ng_setup_request_p->globalRANNodeID.choice.globalGNB_ID.gNB_ID.choice.gNB_ID.buf  = (uint8_t *) calloc (5, sizeof(uint8_t));
	memset(ng_setup_request_p->globalRANNodeID.choice.globalGNB_ID.gNB_ID.choice.gNB_ID.buf, 0, 5);
	//ng_setup_request_p->globalRANNodeID.choice.globalGNB_ID.gNB_ID.choice.gNB_ID.buf[0]  = 0x01;
	//ng_setup_request_p->globalRANNodeID.choice.globalGNB_ID.gNB_ID.choice.gNB_ID.buf[1]  = 0x02;
	//ng_setup_request_p->globalRANNodeID.choice.globalGNB_ID.gNB_ID.choice.gNB_ID.buf[2]  = 0x03;
	//ng_setup_request_p->globalRANNodeID.choice.globalGNB_ID.gNB_ID.choice.gNB_ID.buf[3]  = 0x04;
	//ng_setup_request_p->globalRANNodeID.choice.globalGNB_ID.gNB_ID.choice.gNB_ID.buf[4]  = 0x05;

	OCTET_STRING_fromBuf (&(ng_setup_request_p->globalRANNodeID.choice.globalGNB_ID.pLMNIdentity), "20893", strlen ("20893"));


	ng_setup_request_p->defaultPagingDRX = PagingDRX_v256;

	//supportedTAList
	memset (&ta, 0, sizeof (SupportedTAItem_t));
	OCTET_STRING_fromBuf (&ta.tAC, "1", strlen("1"));
	//memset (&plmnIdentity, 0, sizeof (PLMNIdentity_t));
	//OCTET_STRING_fromBuf (&plmnIdentity,"20893", strlen ("20893"));
	BroadcastPLMNItem_t broadcastPLMNItem;

	SliceSupportItem_t sliceSupportItem;
	memset(&sliceSupportItem, 0, sizeof (SliceSupportItem_t));
	OCTET_STRING_fromBuf (&sliceSupportItem.s_NSSAI.sST, "SST", strlen("SST"));

	memset (&broadcastPLMNItem, 0, sizeof(BroadcastPLMNItem_t));
	OCTET_STRING_fromBuf (&(broadcastPLMNItem.pLMNIdentity),"20893", strlen ("20893"));

	ASN_SEQUENCE_ADD (&(broadcastPLMNItem.tAISliceSupportList), &sliceSupportItem);
	ASN_SEQUENCE_ADD (&(ta.broadcastPLMNList), &broadcastPLMNItem);
	ASN_SEQUENCE_ADD (&ng_setup_request_p->supportedTAList, &ta);

	//encode the message
	ngap_amf_encode_pdu (&message, &buffer_p, &length);
*/
/*	 ngap_message message = {0};
	  NGSetupFailureIEs_t * ng_setup_failure_p = NULL;
	  const Cause_PR cause_type = Cause_PR_transport;
	  const long cause_value = CauseProtocol_unspecified;
	  const long time_to_wait = TimeToWait_v20s;

	  uint8_t * buffer_p = NULL;
	  uint32_t length = 0;

	  ng_setup_failure_p = &message.msg.ngSetupFailureIEs;
	  message.procedureCode = Ngap_ProcedureCode_id_NGSetup;
	  message.direction = NGAP_PDU_PR_unsuccessfulOutcome;
	  ngap_amf_set_cause (&ng_setup_failure_p->cause, cause_type, cause_value);
	  ng_setup_failure_p->presenceMask |= NGSETUPFAILUREIES_TIMETOWAIT_PRESENT;
	  ng_setup_failure_p->timeToWait = time_to_wait;

	  if (ngap_amf_encode_pdu (&message, &buffer_p, &length) < 0) {
	    printf("encode ngap pdu error\n");
	  }
*/


/*
    Ngap_IE_t *ie;


    if ((ie = ngap_new_ie(Ngap_ProtocolIE_ID_id_GlobalRANNodeID,
                        Ngap_Criticality_reject,
                        &asn_DEF_GlobalRANNodeID,
                          &ngSetupRequestIEs.globalRANNodeID)) == NULL) {
    	printf("REturn Null");
        //return -1;
    }
*/


/*    //decode for testing purpose
	bstring b = blk2bstr(buffer_p, length);

	MessagesIds message_id = MESSAGES_ID_MAX;
	ngap_message decoded_message = {0};
	ngap_amf_decode_pdu(&decoded_message,b,&message_id);
	printf("defaultPagingDRX: %d\n",decoded_message.msg.ngSetupRequestIEs.defaultPagingDRX);
	printf("procedureCode: %d\n",decoded_message.procedureCode);
	printf("direction: %d\n",decoded_message.direction);
	printf("pLMNIdentity: %d\n",decoded_message.msg.ngSetupRequestIEs.supportedTAList.list.array[0]->broadcastPLMNList.list.array[0]->pLMNIdentity.size);
	printf("pLMNIdentity: %s", decoded_message.msg.ngSetupRequestIEs.supportedTAList.list.array[0]->broadcastPLMNList.list.array[0]->pLMNIdentity.buf);
    printf("MSG content: \n");
	for (int j=0; j< length; j++)
	 printf("%2x ",((unsigned char *)(buffer_p))[j]);
	printf("\n");
	*/
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


	//send SCTP association request to AMF using sctp_send_msg
	send_NGAP_SetupRequest();
	//test NGAP
	//test gNB

	while (1) {
		sleep (1);
	}

	return (0);

}
