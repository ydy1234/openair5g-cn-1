#include <stdio.h>
#include <stdlib.h>


#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "bstrlib.h"
#include "intertask_interface_types.h"
#include "ngap_amf_encoder.h"
#include "sctp_primitives_client.h"
#include "intertask_interface.h"
#include "assertions.h"
#include "intertask_interface_init.h"
#include "sctp_primitives_server.h"
#include "Ngap_CriticalityDiagnostics-IE-Item.h"
#include "Ngap_CriticalityDiagnostics-IE-List.h"
#include "Ngap_ProtocolIE-Field.h"
#include "Ngap_UserLocationInformationEUTRA.h"

#include "ngap_amf.h"
#include "amf_app.h"
#include "log.h"
#include "ngap_common.h"
#include "sctp_gNB_defs.h"





Ngap_SupportedTAList_t	     g_SupportedTAList;
Ngap_GlobalRANNodeIDList_t   g_glocalRANNodeIDList;

int g_supportTaListCount = 0;;
int init_ng_setup_request_param()
{
    memset(&g_SupportedTAList,  0, sizeof(g_SupportedTAList));
	memset(&g_glocalRANNodeIDList,  0, sizeof(Ngap_GlobalRANNodeID_t));
    
	
	//Ngap_SupportedTAItem_t: tac
	Ngap_SupportedTAItem_t *ta;
	ta = (Ngap_SupportedTAItem_t *)calloc(1, sizeof(Ngap_SupportedTAItem_t));
	uint8_t tAC[3] = {0x01, 0x02, 0x03};
	OCTET_STRING_fromBuf(&ta->tAC, (const char*)tAC, 3);

	//Ngap_SupportedTAItem_t: broadcastPLMNList
	Ngap_BroadcastPLMNItem_t *broadcastPLMNItem = NULL;
	broadcastPLMNItem = (Ngap_BroadcastPLMNItem_t *)calloc(1, sizeof(Ngap_BroadcastPLMNItem_t));
	

	//broadcastPLMNList: pLMNIdentity;
	uint8_t plmnIdentity[3] = {0x01, 0x02, 0x03};
	OCTET_STRING_fromBuf(&broadcastPLMNItem->pLMNIdentity, (const char*)plmnIdentity, 3);
	
    
	//broadcastPLMNList: tAISliceSupportList
	Ngap_SliceSupportItem_t *sliceSupportItem = NULL;
	sliceSupportItem = calloc (1, sizeof(sliceSupportItem));
	sliceSupportItem->s_NSSAI.sD = calloc(1, sizeof (sliceSupportItem->s_NSSAI.sD));
	uint8_t sST[1] = {0x03};
	uint8_t sD[3] = {0x30, 0x33, 0x01};
	OCTET_STRING_fromBuf(&sliceSupportItem->s_NSSAI.sST, (const char*)sST, 1);
	OCTET_STRING_fromBuf(sliceSupportItem->s_NSSAI.sD, (const char*)sD, 3);
	ASN_SEQUENCE_ADD (&broadcastPLMNItem->tAISliceSupportList.list, &sliceSupportItem);
    ASN_SEQUENCE_ADD(&ta->broadcastPLMNList, &broadcastPLMNItem);
	
    ASN_SEQUENCE_ADD (&g_SupportedTAList.list, ta);

    g_supportTaListCount = g_SupportedTAList.list.count;
	
    return  0;
}


void
send_NGAP_SetupRequest()
{
    printf("NGAP_SetupRequest-------------encode\n");
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

    printf("ngap_amf_handle_message procedureCode:%d;present:%d\n",pdu.choice.initiatingMessage->procedureCode,pdu.present);
    printf("procedureCode:%d;present:%d\n",pdu.choice.initiatingMessage->procedureCode,pdu.present);    

	ngapSetupRequestIEs = calloc(1, sizeof(Ngap_NGSetupRequestIEs_t));
	ngapSetupRequestIEs->id = Ngap_ProtocolIE_ID_id_GlobalRANNodeID;
	ngapSetupRequestIEs->criticality = Ngap_Criticality_reject;
	ngapSetupRequestIEs->value.present = Ngap_NGSetupRequestIEs__value_PR_GlobalRANNodeID;

	Ngap_GlobalRANNodeID_t *ngap_GlobalRANNodeID = NULL;
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
    printf("gNB_ID: 0x%x,0x%x,0x%x,0x%x\n",gNB_ID[0],gNB_ID[1],gNB_ID[2],gNB_ID[3]);

	//RANNodeName
	ngapSetupRequestIEs = calloc(1, sizeof(Ngap_NGSetupRequestIEs_t));
	ngapSetupRequestIEs->id = Ngap_ProtocolIE_ID_id_RANNodeName;
	ngapSetupRequestIEs->criticality = Ngap_Criticality_reject;
	ngapSetupRequestIEs->value.present = Ngap_NGSetupRequestIEs__value_PR_RANNodeName;
	OCTET_STRING_fromBuf (&ngapSetupRequestIEs->value.choice.RANNodeName, "gNB1 Eurecom", strlen ("gNB1 Eurecom"));
	ASN_SEQUENCE_ADD(&ngapSetupRequest->protocolIEs, ngapSetupRequestIEs);
	printf("len:%d,RANNodeName:%s\n",ngapSetupRequestIEs->value.choice.RANNodeName.size, ngapSetupRequestIEs->value.choice.RANNodeName.buf);

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
    memset (broadcastPLMNItem, 0, sizeof(Ngap_BroadcastPLMNItem_t));

    OCTET_STRING_fromBuf(&broadcastPLMNItem->pLMNIdentity, (const char*)plmn, 3);
    ASN_SEQUENCE_ADD (&broadcastPLMNItem->tAISliceSupportList.list, &sliceSupportItem);
    ASN_SEQUENCE_ADD (&ta->broadcastPLMNList.list, broadcastPLMNItem);

    ASN_SEQUENCE_ADD (&ngapSetupRequestIEs->value.choice.SupportedTAList.list, ta);
    ASN_SEQUENCE_ADD(&ngapSetupRequest->protocolIEs, ngapSetupRequestIEs);


	//PagingDRX
	ngapSetupRequestIEs = calloc(1, sizeof(Ngap_NGSetupRequestIEs_t));
	ngapSetupRequestIEs->id =Ngap_ProtocolIE_ID_id_DefaultPagingDRX;
	ngapSetupRequestIEs->criticality = Ngap_Criticality_reject;
	ngapSetupRequestIEs->value.present = Ngap_NGSetupRequestIEs__value_PR_PagingDRX;
	ngapSetupRequestIEs->value.choice.PagingDRX = Ngap_PagingDRX_v256;
	ASN_SEQUENCE_ADD(&ngapSetupRequest->protocolIEs, ngapSetupRequestIEs);
    printf("PagingDRX:%ld\n",ngapSetupRequestIEs->value.choice.PagingDRX);
	printf("0000000000000000000000\n");
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

	 #if 0
	 bstring b = blk2bstr(buffer_p, length);
	 
	 printf("NGAP_SetupRequest111111111111111111-------------decode\n");
     ngap_amf_decode_pdu(&decoded_pdu, b,  &message_id);
     printf("error!!!!\n");
     ngap_amf_handle_message(0,0,&decoded_pdu);

     #endif
	
  	 sctp_data_p = (sctp_data_t *) calloc (1, sizeof(sctp_data_t));
  	 if (sctp_data_p == NULL)  exit(1);
  	 assoc[0] = sctp_connect_to_remote_host (local_ip_addr, 1, remote_ip_addr, 36412, SOCK_STREAM, sctp_data_p);
  	 sctp_send_msg (sctp_data_p, 60, 0, buffer_p,length);
  
     
     int                                     flags = 0, n = 0;
     #define SCTP_RECV_BUFFER_SIZE  1024
     //socklen_t								from_len = 0;
     int								from_len = 0;
     struct sctp_sndrcvinfo					sinfo = {0};
     struct sockaddr_in 					    addr = {0};
     char 								    buffer[SCTP_RECV_BUFFER_SIZE] = {0};
     int sd = sctp_data_p->sd;
   	 while(1)
  	 {
          memset ((void *)&addr, 0, sizeof (struct sockaddr_in));
          from_len = (socklen_t) sizeof (struct sockaddr_in);
          memset ((void *)&sinfo, 0, sizeof (struct sctp_sndrcvinfo));
          n = sctp_recvmsg (sd, (void *)buffer, SCTP_RECV_BUFFER_SIZE, (struct sockaddr *)&addr, &from_len, &sinfo, &flags);
         
          if (n < 0) {
             OAILOG_DEBUG (LOG_SCTP, "An error occured during read\n");
             OAILOG_ERROR (LOG_SCTP, "sctp_recvmsg: %s:%d\n", strerror (errno), errno);
             continue;
          }
          if (flags & MSG_NOTIFICATION)
  		  {
             union sctp_notification                *snp = (union sctp_notification *)buffer;
         
             switch (snp->sn_header.sn_type) {
             case SCTP_SHUTDOWN_EVENT: {
               OAILOG_DEBUG (LOG_SCTP, "SCTP_SHUTDOWN_EVENT received\n");
               //return sctp_handle_com_down((sctp_assoc_id_t) snp->sn_shutdown_event.sse_assoc_id);
             }
             case SCTP_ASSOC_CHANGE: {
               OAILOG_DEBUG(LOG_SCTP, "SCTP association change event received\n");
               //return handle_assoc_change(sd, ppid, &snp->sn_assoc_change);
             }
             default: {
               OAILOG_WARNING(LOG_SCTP, "Unhandled notification type %u\n", snp->sn_header.sn_type);
               break;
             }
             }
           }
           else
           {
          	   MessagesIds message_id = MESSAGES_ID_MAX;
               Ngap_NGAP_PDU_t decoded_pdu = {0};
               uint8_t * buffer = NULL;
               bstring b = blk2bstr(buffer, n);
          
          	 
               printf("NGAP_SetupRequest-------------decode, length:%d\n", n);
               ngap_amf_decode_pdu(&decoded_pdu, b,  &message_id);
               ngap_amf_handle_message(0,0,&decoded_pdu);
   			   break;
           }
      }
   
  
   #if 0
        //connect to sctp and send message to AMF
        printf("sctp client send msg buffer(%x) length(%d)\n",buffer_p,length);
	sctp_data_p = (sctp_data_t *) calloc (1, sizeof(sctp_data_t));
	if (sctp_data_p == NULL)  exit(1);
	assoc[0] = sctp_connect_to_remote_host (local_ip_addr, 1, remote_ip_addr, 36412, SOCK_STREAM, sctp_data_p);
	sctp_send_msg (sctp_data_p, 60, 0, buffer_p,length);
    #endif
}

void
send_NGAP_SetupFailure()
{
    printf("\n\nNGAP_SetupFailure-------------encode\n");
	
	int assoc[1];
	sctp_data_t * sctp_data_p = NULL;
	char *local_ip_addr[] = {"127.0.0.1"};
	char remote_ip_addr[] = "127.0.0.1";
	Ngap_NGAP_PDU_t                         pdu;
	uint8_t * buffer_p = NULL;
	uint32_t length = 0;

	Ngap_NGSetupFailure_t                       *ngapSetupFailure = NULL;
	Ngap_NGSetupFailureIEs_t                    *ngapSetupFailureIEs = NULL;
	memset(&pdu, 0, sizeof(pdu));

	//for NGSetupFailure
	pdu.present = Ngap_NGAP_PDU_PR_unsuccessfulOutcome;
	pdu.choice.unsuccessfulOutcome = calloc(1, sizeof(Ngap_UnsuccessfulOutcome_t));
	pdu.choice.unsuccessfulOutcome->procedureCode = Ngap_ProcedureCode_id_NGSetup;
	pdu.choice.unsuccessfulOutcome->criticality = Ngap_Criticality_reject;
	pdu.choice.unsuccessfulOutcome->value.present = Ngap_UnsuccessfulOutcome__value_PR_NGSetupFailure;
	ngapSetupFailure = &pdu.choice.unsuccessfulOutcome->value.choice.NGSetupFailure;

    
	printf("ngap_amf_handle_message procedureCode:%d;present:%d\n",pdu.choice.initiatingMessage->procedureCode,pdu.present);
	printf("procedureCode:%d;present:%d\n",pdu.choice.initiatingMessage->procedureCode,pdu.present);	  

    //cause: radioNetwork
    ngapSetupFailureIEs = calloc(1, sizeof(Ngap_NGSetupFailureIEs_t));
	ngapSetupFailureIEs->id = Ngap_ProtocolIE_ID_id_Cause; 
	ngapSetupFailureIEs->value.present = Ngap_NGSetupFailureIEs__value_PR_Cause;
    ngapSetupFailureIEs->value.choice.Cause.present =  Ngap_Cause_PR_radioNetwork;
	ngapSetupFailureIEs->value.choice.Cause.choice.radioNetwork = 0x80;
    ASN_SEQUENCE_ADD(&ngapSetupFailure->protocolIEs, ngapSetupFailureIEs);
	printf("radioNetwork:0x%x\n", ngapSetupFailureIEs->value.choice.Cause.choice.radioNetwork);

   
	//timetowait
	ngapSetupFailureIEs = calloc(1, sizeof(Ngap_NGSetupFailureIEs_t));
	ngapSetupFailureIEs->id = Ngap_ProtocolIE_ID_id_TimeToWait; 
	ngapSetupFailureIEs->value.present = Ngap_NGSetupFailureIEs__value_PR_TimeToWait;
    ngapSetupFailureIEs->value.choice.TimeToWait =  0x81;
    ASN_SEQUENCE_ADD(&ngapSetupFailure->protocolIEs, ngapSetupFailureIEs);

	printf("TimeToWait:0x%x\n", ngapSetupFailureIEs->value.choice.TimeToWait);

	
    //CriticalityDiagnostics
    ngapSetupFailureIEs = calloc(1, sizeof(Ngap_NGSetupFailureIEs_t));
	ngapSetupFailureIEs->id = Ngap_ProtocolIE_ID_id_CriticalityDiagnostics; 
	ngapSetupFailureIEs->value.present = Ngap_NGSetupFailureIEs__value_PR_CriticalityDiagnostics;

    //Ngap_CriticalityDiagnostics_t	 criticalityDiagnostics;
	//memset(&criticalityDiagnostics, 0, sizeof(Ngap_CriticalityDiagnostics_t));
	
	Ngap_ProcedureCode_t  *procedureCode = calloc(1, sizeof(Ngap_ProcedureCode_t));
	*procedureCode = 0x81;
    ngapSetupFailureIEs->value.choice.CriticalityDiagnostics.procedureCode  = procedureCode;

	Ngap_TriggeringMessage_t  *triggeringMessage = calloc(1, sizeof(Ngap_TriggeringMessage_t));
	*triggeringMessage = 0x82;
    ngapSetupFailureIEs->value.choice.CriticalityDiagnostics.triggeringMessage = triggeringMessage;

	Ngap_Criticality_t  *procedureCriticality = calloc(1, sizeof(Ngap_Criticality_t));
	*procedureCriticality = 0x83;
	ngapSetupFailureIEs->value.choice.CriticalityDiagnostics.procedureCriticality = procedureCriticality;

    Ngap_CriticalityDiagnostics_IE_Item_t  *criticalityDiagnosticsIEs = calloc(1, sizeof(Ngap_CriticalityDiagnostics_IE_Item_t));
	criticalityDiagnosticsIEs->iECriticality = 0x85;
	criticalityDiagnosticsIEs->iE_ID = 0x86;
	criticalityDiagnosticsIEs->typeOfError = 0x86;

    ASN_SEQUENCE_ADD(&ngapSetupFailureIEs->value.choice.CriticalityDiagnostics.iEsCriticalityDiagnostics->list, &criticalityDiagnosticsIEs);
	ASN_SEQUENCE_ADD(&ngapSetupFailure->protocolIEs, ngapSetupFailureIEs);


	
	int enc_rval = ngap_amf_encode_pdu (&pdu, &buffer_p, &length);
	
	MessagesIds message_id = MESSAGES_ID_MAX;
    Ngap_NGAP_PDU_t decoded_pdu = {0};
     
	bstring b = blk2bstr(buffer_p, length);
	 
	printf("NGAP_SetupFailure-------------decode, length:%d\n", length);
    ngap_amf_decode_pdu(&decoded_pdu, b,  &message_id);
    ngap_amf_handle_message(0,0,&decoded_pdu);

    #if  0
	//connect to sctp and send message to AMF
	sctp_data_p = (sctp_data_t *) calloc (1, sizeof(sctp_data_t));
	if (sctp_data_p == NULL)  exit(1);
	assoc[0] = sctp_connect_to_remote_host (local_ip_addr, 1, remote_ip_addr, 36412, SOCK_STREAM, sctp_data_p);
	sctp_send_msg (sctp_data_p, 60, 0, buffer_p,length);
    #endif

}
#if 0
int send_NGAP_Initial_UE_Message()
{
    printf("\n\nNGAP_Initial_UE_Message-------------encode\n");
		
	int assoc[1];
	sctp_data_t * sctp_data_p = NULL;
	char *local_ip_addr[] = {"127.0.0.1"};
	char remote_ip_addr[] = "127.0.0.1";
	Ngap_NGAP_PDU_t 						pdu;
	uint8_t * buffer_p = NULL;
	uint32_t length = 0;

	Ngap_InitialUEMessage_t						*ngapInitialUeMsg= NULL;
	Ngap_InitialUEMessage_IEs_t					*ngapInitialUeMsgIEs = NULL;
	memset(&pdu, 0, sizeof(pdu));
	
	//for NGSetupFailure
	pdu.present = Ngap_NGAP_PDU_PR_initiatingMessage;
	pdu.choice.initiatingMessage = calloc(1, sizeof(Ngap_InitiatingMessage_t));
	pdu.choice.initiatingMessage->procedureCode = Ngap_ProcedureCode_id_InitialUEMessage;
	pdu.choice.initiatingMessage->criticality = Ngap_Criticality_reject;
	pdu.choice.initiatingMessage->value.present = Ngap_InitiatingMessage__value_PR_InitialUEMessage;
	ngapInitialUeMsg = &pdu.choice.initiatingMessage->value.choice.InitialUEMessage;
	
	printf("ngap_amf_handle_message procedureCode:%d;present:%d\n",pdu.choice.initiatingMessage->procedureCode,pdu.present);
	printf("procedureCode:%d;present:%d\n",pdu.choice.initiatingMessage->procedureCode,pdu.present);	  

	ngapInitialUeMsgIEs = calloc(1, sizeof(Ngap_InitialUEMessage_IEs_t));
	ngapInitialUeMsgIEs->id = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID; 
	ngapInitialUeMsgIEs->value.present = Ngap_InitialUEMessage_IEs__value_PR_RAN_UE_NGAP_ID;
	ngapInitialUeMsgIEs->value.choice.RAN_UE_NGAP_ID = 0x80;
    ASN_SEQUENCE_ADD(&ngapInitialUeMsg->protocolIEs, ngapInitialUeMsgIEs);
	printf("RAN_UE_NGAP_ID:0x%x\n", ngapInitialUeMsgIEs->value.choice.RAN_UE_NGAP_ID);

	int enc_rval = ngap_amf_encode_pdu (&pdu, &buffer_p, &length);

	MessagesIds message_id = MESSAGES_ID_MAX;
    Ngap_NGAP_PDU_t decoded_pdu = {0};
     
	bstring b = blk2bstr(buffer_p, length);
	 
	printf("NGAP_Initial_UE_Message-------------decode\n");
	
    ngap_amf_decode_pdu(&decoded_pdu, b,  &message_id);
    ngap_amf_handle_message(0,0,&decoded_pdu);
	
    return  0;
}
#endif

int send_NGAP_Uplink_Nas_Transport()
{
    printf("\n\nNGAP_Uplink_Nas_Transport-------------encode\n");
		
	int assoc[1];
	sctp_data_t * sctp_data_p = NULL;
	char *local_ip_addr[] = {"127.0.0.1"};
	char remote_ip_addr[] = "127.0.0.1";
	Ngap_NGAP_PDU_t 						pdu;
	uint8_t * buffer_p = NULL;
	uint32_t length = 0;

	
	Ngap_UplinkNASTransport_t						*ngapUplinkNasTransport = NULL;
	Ngap_UplinkNASTransport_IEs_t					*ngapUplinkNasTransportIEs = NULL;
	memset(&pdu, 0, sizeof(pdu));
	
	//for NGuplinkNasTransport
	pdu.present = Ngap_NGAP_PDU_PR_initiatingMessage;
	pdu.choice.initiatingMessage = calloc(1, sizeof(Ngap_InitiatingMessage_t));
	pdu.choice.initiatingMessage->procedureCode = Ngap_ProcedureCode_id_UplinkNASTransport;
	pdu.choice.initiatingMessage->criticality = Ngap_Criticality_reject;
	pdu.choice.initiatingMessage->value.present = Ngap_InitiatingMessage__value_PR_UplinkNASTransport;
	ngapUplinkNasTransport = &pdu.choice.initiatingMessage->value.choice.UplinkNASTransport;

	printf("ngap_amf_handle_message procedureCode:%d;present:%d\n",pdu.choice.initiatingMessage->procedureCode,pdu.present);
	printf("procedureCode:%d;present:%d\n",pdu.choice.initiatingMessage->procedureCode,pdu.present);	  
  
	//AMF_UE_NGAP_ID
    ngapUplinkNasTransportIEs = calloc(1, sizeof(Ngap_UplinkNASTransport_IEs_t));
	ngapUplinkNasTransportIEs->id = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID; 
	ngapUplinkNasTransportIEs->value.present = Ngap_UplinkNASTransport_IEs__value_PR_AMF_UE_NGAP_ID;
  

	uint8_t amf_UN[2] = {0x01, 0x02};
	Ngap_AMF_UE_NGAP_ID_t amf_uni;
	memset(&amf_uni, 0 , sizeof(Ngap_AMF_UE_NGAP_ID_t));
	amf_uni.buf = amf_UN;
	amf_uni.size  =  2;
    //asn_ulong2INTEGER(&ngapUplinkNasTransportIEs->value.choice.AMF_UE_NGAP_ID, 0x80);
	ngapUplinkNasTransportIEs->value.choice.AMF_UE_NGAP_ID = amf_uni;
    ASN_SEQUENCE_ADD(&ngapUplinkNasTransport->protocolIEs, ngapUplinkNasTransport);
	printf("AMF_UE_NGAP_ID, size:%d,0x%x,0x%x\n", 
	ngapUplinkNasTransportIEs->value.choice.AMF_UE_NGAP_ID.size,
	ngapUplinkNasTransportIEs->value.choice.AMF_UE_NGAP_ID.buf[0],
	ngapUplinkNasTransportIEs->value.choice.AMF_UE_NGAP_ID.buf[1]);

   
    //RAN_UE_NGAP_ID
	ngapUplinkNasTransportIEs = calloc(1, sizeof(Ngap_UplinkNASTransport_IEs_t));
	ngapUplinkNasTransportIEs->id = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;; 
	ngapUplinkNasTransportIEs->value.present = Ngap_UplinkNASTransport_IEs__value_PR_RAN_UE_NGAP_ID;
	ngapUplinkNasTransportIEs->value.choice.RAN_UE_NGAP_ID = 0x80;
    ASN_SEQUENCE_ADD(&ngapUplinkNasTransport->protocolIEs, ngapUplinkNasTransport);
	printf("RAN_UE_NGAP_ID:0x%x\n", ngapUplinkNasTransportIEs->value.choice.RAN_UE_NGAP_ID);


	//NAS_PDU
	ngapUplinkNasTransportIEs = calloc(1, sizeof(Ngap_UplinkNASTransport_IEs_t));
	ngapUplinkNasTransportIEs->id = Ngap_ProtocolIE_ID_id_NAS_PDU;; 
	ngapUplinkNasTransportIEs->value.present = Ngap_UplinkNASTransport_IEs__value_PR_NAS_PDU;
	uint8_t amf_nas_pdu[3] = { 0x02, 0xF8, 0x29 };
	OCTET_STRING_fromBuf(&ngapUplinkNasTransportIEs->value.choice.NAS_PDU, (const char*)amf_nas_pdu, 3);
    ASN_SEQUENCE_ADD(&ngapUplinkNasTransport->protocolIEs, ngapUplinkNasTransport);
	printf("NAS_PDU:0x%x,0x%x,0x%x\n", 
	ngapUplinkNasTransportIEs->value.choice.NAS_PDU.buf[0],
	ngapUplinkNasTransportIEs->value.choice.NAS_PDU.buf[1],
	ngapUplinkNasTransportIEs->value.choice.NAS_PDU.buf[2]);

    //UserLocationInformation
    ngapUplinkNasTransportIEs = calloc(1, sizeof(Ngap_UplinkNASTransport_IEs_t));
	ngapUplinkNasTransportIEs->id = Ngap_ProtocolIE_ID_id_UserLocationInformation;; 
	ngapUplinkNasTransportIEs->value.present = Ngap_UplinkNASTransport_IEs__value_PR_UserLocationInformation;
    
    Ngap_UserLocationInformation_t	 UserLocationInformation;
	memset(&UserLocationInformation, 0, sizeof(Ngap_UserLocationInformation_t));
	UserLocationInformation.present =  Ngap_UserLocationInformation_PR_userLocationInformationEUTRA;

	Ngap_UserLocationInformationEUTRA_t	*userLocationInformationEUTRA = calloc(1, sizeof(Ngap_UserLocationInformationEUTRA_t));

	UserLocationInformation.choice.userLocationInformationEUTRA = userLocationInformationEUTRA; 

    //eUTRA_CGI: pLMNIdentity,eUTRACellIdentity.

    uint8_t plmni[3] = { 0x02, 0xF8, 0x29 };
	OCTET_STRING_fromBuf(&userLocationInformationEUTRA->eUTRA_CGI.pLMNIdentity, (const char*)plmni, 3);

	uint8_t UTRACellI[3] = { 0x02, 0xF8, 0x29 };
    Ngap_EUTRACellIdentity_t	eUTRACellIdentity;
	memset(&eUTRACellIdentity, 0, sizeof(eUTRACellIdentity));
	eUTRACellIdentity.buf = UTRACellI;
	eUTRACellIdentity.size = 3;

    userLocationInformationEUTRA->eUTRA_CGI.eUTRACellIdentity = eUTRACellIdentity;
	
	
	//tai: pLMNIdentity,tAC;
	//Ngap_PLMNIdentity_t	 pLMNIdentity;
	//Ngap_TAC_t	 tAC;
	uint8_t plmnit[3] = { 0x02, 0xF8, 0x29 };
	OCTET_STRING_fromBuf(&userLocationInformationEUTRA->tAI.pLMNIdentity, (const char*)plmnit, 3);
	uint8_t tACt[3] = { 0x02, 0xF8, 0x29 };
	OCTET_STRING_fromBuf(&userLocationInformationEUTRA->tAI.tAC, (const char*)tACt, 3);
    

    //timestamp
    uint8_t timestamp[3] = { 0x02, 0xF8, 0x29 };
	OCTET_STRING_fromBuf(userLocationInformationEUTRA->timeStamp, (const char*)timestamp, 3);
    
    ngapUplinkNasTransportIEs->value.choice.UserLocationInformation =  UserLocationInformation;
	
    ASN_SEQUENCE_ADD(&ngapUplinkNasTransport->protocolIEs, ngapUplinkNasTransport);
	
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
	 
	printf("NGAP_Uplink_Nas_Transport-------------decode, length:%d\n", length);
    ngap_amf_decode_pdu(&decoded_pdu, b,  &message_id);
    ngap_amf_handle_message(0,0,&decoded_pdu);

    return  0;
}

int send_NGAP_Downlink_Nas_Transport()
{
    printf("\n\nNGAP_Downlink_Nas_Transport-------------encode\n");
		
	int assoc[1];
	sctp_data_t * sctp_data_p = NULL;
	char *local_ip_addr[] = {"127.0.0.1"};
	char remote_ip_addr[] = "127.0.0.1";
	Ngap_NGAP_PDU_t 						pdu;
	uint8_t * buffer_p = NULL;
	uint32_t length = 0;

	Ngap_DownlinkNASTransport_t	     *ngapDownlinkNASTransport     = NULL;
	Ngap_DownlinkNASTransport_IEs_t  *ngapDownlinkNASTransportIEs  = NULL;
	
	memset(&pdu, 0, sizeof(pdu));
	
	//for NGDownLinkNasTransport
	pdu.present = Ngap_NGAP_PDU_PR_initiatingMessage;
	pdu.choice.initiatingMessage = calloc(1, sizeof(Ngap_InitiatingMessage_t));
	pdu.choice.initiatingMessage->procedureCode = Ngap_ProcedureCode_id_DownlinkNASTransport;
	pdu.choice.initiatingMessage->criticality = Ngap_Criticality_reject;
	pdu.choice.initiatingMessage->value.present = Ngap_InitiatingMessage__value_PR_DownlinkNASTransport;
	ngapDownlinkNASTransport = &pdu.choice.initiatingMessage->value.choice.DownlinkNASTransport;

	printf("ngap_amf_handle_message procedureCode:%d;present:%d\n",pdu.choice.initiatingMessage->procedureCode,pdu.present);
	printf("procedureCode:%d;present:%d\n",pdu.choice.initiatingMessage->procedureCode,pdu.present);	  


    //AMF_UE_NGAP_ID
    ngapDownlinkNASTransportIEs = calloc(1, sizeof(Ngap_DownlinkNASTransport_IEs_t));
	ngapDownlinkNASTransportIEs->id = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID; 
	ngapDownlinkNASTransportIEs->value.present = Ngap_DownlinkNASTransport_IEs__value_PR_AMF_UE_NGAP_ID;
  

	uint8_t amf_UN[2] = {0x01, 0x02};
	Ngap_AMF_UE_NGAP_ID_t amf_uni;
	memset(&amf_uni, 0 , sizeof(Ngap_AMF_UE_NGAP_ID_t));
	amf_uni.buf = amf_UN;
	amf_uni.size  =  2;
    //asn_ulong2INTEGER(&ngapUplinkNasTransportIEs->value.choice.AMF_UE_NGAP_ID, 0x80);
	ngapDownlinkNASTransportIEs->value.choice.AMF_UE_NGAP_ID = amf_uni;
    ASN_SEQUENCE_ADD(&ngapDownlinkNASTransport->protocolIEs, ngapDownlinkNASTransportIEs);
	
	printf("AMF_UE_NGAP_ID, size:%d,0x%x,0x%x\n", 
	ngapDownlinkNASTransportIEs->value.choice.AMF_UE_NGAP_ID.size,
	ngapDownlinkNASTransportIEs->value.choice.AMF_UE_NGAP_ID.buf[0],
	ngapDownlinkNASTransportIEs->value.choice.AMF_UE_NGAP_ID.buf[1]);


    //RAN_UE_NGAP_ID
    ngapDownlinkNASTransportIEs = calloc(1, sizeof(Ngap_DownlinkNASTransport_IEs_t));
	ngapDownlinkNASTransportIEs->id = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID; 
	ngapDownlinkNASTransportIEs->value.present = Ngap_DownlinkNASTransport_IEs__value_PR_RAN_UE_NGAP_ID;
  

	uint8_t amf_RU[2] = {0x03, 0x04};
	Ngap_RAN_UE_NGAP_ID_t amf_ru;
	memset(&amf_ru, 0 , sizeof(Ngap_RAN_UE_NGAP_ID_t));
	amf_ru =  0x79;
    //asn_ulong2INTEGER(&ngapUplinkNasTransportIEs->value.choice.AMF_UE_NGAP_ID, 0x80);
	ngapDownlinkNASTransportIEs->value.choice.RAN_UE_NGAP_ID = amf_ru;
    ASN_SEQUENCE_ADD(&ngapDownlinkNASTransport->protocolIEs, ngapDownlinkNASTransportIEs);
	
	printf("RAN_UE_NGAP_ID, size:%d,0x%x\n",ngapDownlinkNASTransportIEs->value.choice.RAN_UE_NGAP_ID);


	//Ngap_AMFName_t
	ngapDownlinkNASTransportIEs = calloc(1, sizeof(Ngap_DownlinkNASTransport_IEs_t));
	ngapDownlinkNASTransportIEs->id = Ngap_ProtocolIE_ID_id_AMFName; 
	ngapDownlinkNASTransportIEs->value.present = Ngap_DownlinkNASTransport_IEs__value_PR_AMFName;
	OCTET_STRING_fromBuf(&ngapDownlinkNASTransportIEs->value.choice.AMFName, "amfName", strlen("amfName"));
	ASN_SEQUENCE_ADD(&ngapDownlinkNASTransport->protocolIEs, ngapDownlinkNASTransportIEs);
	   
	printf("AMFName:%s\n",ngapDownlinkNASTransportIEs->value.choice.AMFName.buf);
    
    int enc_rval = ngap_amf_encode_pdu (&pdu, &buffer_p, &length);
    printf("decode --------------------\n");
	MessagesIds message_id = MESSAGES_ID_MAX;
    Ngap_NGAP_PDU_t decoded_pdu = {0};
     
	bstring b = blk2bstr(buffer_p, length);
	 
	printf("NGAP_downlink_Nas_Transport-------------decode, length:%d\n", length);
    ngap_amf_decode_pdu(&decoded_pdu, b,  &message_id);
    ngap_amf_handle_message(0,0,&decoded_pdu);
	
    return 0;
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
	//init_ng_setup_request_param();
	send_NGAP_SetupRequest();
	//send_NGAP_SetupFailure();
	//send_NGAP_Initial_UE_Message();
	//send_NGAP_Uplink_Nas_Transport();
	//send_NGAP_Downlink_Nas_Transport();
	//test NGAP
	//test gNB
/*
	while (1) {
		sleep (1);
	}

	return (0);
*/
}
