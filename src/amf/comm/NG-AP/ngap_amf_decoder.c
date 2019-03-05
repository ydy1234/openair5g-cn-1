#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "assertions.h"
#include "ngap_common.h"
#include "ngap_ies_defs.h"
#include "ngap_amf_handlers.h"
#include "dynamic_memory_check.h"
#include "ngap_amf_decoder.h"
#include "common_defs.h"


static int
ngap_amf_decode_initiating (
    ngap_message * message,
    InitiatingMessage_t * initiating_p,
    MessagesIds *message_id){
    int ret = RETURNerror;
    MessageDef * message_p = NULL;
    char *message_string = NULL;
    size_t message_string_size;

    OAILOG_FUNC_IN (LOG_S1AP);
    
    DevAssert (initiating_p != NULL);
    message_string = calloc(10000,sizeof(char));
    ngap_string_total_size = 0;
    message->procedureCode = initiating_p->procedureCode;
    message->criticality = initiating_p->criticality;
    
//    switch(initiating_p->procedureCode){
//      case 
//    }


    //message_string_size = strlen(message_string); 
    //message_p = itti_alloc_new_message_sized(TASK_NGAP, *message_id, message_string_size + sizeof(IttiMsgText));
    
}

static int 
ngap_amf_decode_successful_outcome(
    ngap_message *message,
    SuccessfulOutcome_t * successfulOutcome_p,
    MessagesIds * message_id ){
    int ret = RETURNerror;
    MessageDef * message_p = NULL;
    char *message_string = NULL;
    size_t message_string_size;
                
    OAILOG_FUNC_IN (LOG_S1AP);
                
    DevAssert (successfulOutcome_p != NULL);
    message_string = calloc(10000,sizeof(char));
    ngap_string_total_size = 0;
    message->procedureCode = successfulOutcome_p->procedureCode;
    message->criticality = successfulOutcome_p->criticality;
}

static int 
ngap_amf_decode_unsuccessful_outcome(
    ngap_message *message,
    UnsuccessfulOutcome_t * unSuccessfulOutcome_p,
    MessagesIds * message_id ){
    int ret = RETURNerror;
    MessageDef * message_p = NULL;
    char *message_string = NULL;
    size_t message_string_size;

    OAILOG_FUNC_IN (LOG_S1AP);

    DevAssert (unSuccessfulOutcome_p != NULL);
    message_string = calloc(10000,sizeof(char));
    ngap_string_total_size = 0;
    message->procedureCode = unSuccessfulOutcome_p->procedureCode;
    message->criticality = unSuccessfulOutcome_p->criticality;
}

int ngap_free_amf_decode_pdu(      
    ngap_message *message, MessagesIds message_id) {
/*
  switch(message_id) {
  case S1AP_UPLINK_NAS_LOG:
    return free_s1ap_uplinknastransport(&message->msg.s1ap_UplinkNASTransportIEs);
  case S1AP_S1_SETUP_LOG:
    return free_s1ap_s1setuprequest(&message->msg.s1ap_S1SetupRequestIEs);
  case S1AP_INITIAL_UE_MESSAGE_LOG:
    return free_s1ap_initialuemessage(&message->msg.s1ap_InitialUEMessageIEs);
  case S1AP_UE_CONTEXT_RELEASE_REQ_LOG:
    return free_s1ap_uecontextreleaserequest(&message->msg.s1ap_UEContextReleaseRequestIEs);
  case S1AP_UE_CAPABILITY_IND_LOG:
    return free_s1ap_uecapabilityinfoindication(&message->msg.s1ap_UECapabilityInfoIndicationIEs);
  case S1AP_NAS_NON_DELIVERY_IND_LOG:
    return free_s1ap_nasnondeliveryindication_(&message->msg.s1ap_NASNonDeliveryIndication_IEs);
  case S1AP_UE_CONTEXT_RELEASE_LOG:
    return free_s1ap_uecontextreleasecomplete(&message->msg.s1ap_UEContextReleaseCompleteIEs);
  case S1AP_INITIAL_CONTEXT_SETUP_LOG:
    if (message->direction == S1AP_PDU_PR_successfulOutcome) {
      return free_s1ap_initialcontextsetupresponse(&message->msg.s1ap_InitialContextSetupResponseIEs);
    } else {
      return free_s1ap_initialcontextsetupfailure(&message->msg.s1ap_InitialContextSetupFailureIEs);
    }   
  default:
    DevAssert(false);                
      
  }
*/   
} 


int ngap_amf_decode_pdu(
  ngap_message *message,
  const_bstring const raw,
  MessagesIds *message_id) {
  NGAP_PDU_t                              pdu = {(NGAP_PDU_PR_NOTHING)};
  NGAP_PDU_t                             *pdu_p = &pdu;
  asn_dec_rval_t                          dec_ret = {(RC_OK)};
  DevAssert (raw != NULL);
  memset ((void *)pdu_p, 0, sizeof (NGAP_PDU_t));
  dec_ret = uper_decode (NULL, &asn_DEF_NGAP_PDU, (void **)&pdu_p, bdata(raw), blength(raw), 0, 0);
        
  if (dec_ret.code != RC_OK) {
    OAILOG_ERROR (LOG_S1AP, "Failed to decode PDU\n");
    return -1;
  }
        
  message->direction = pdu_p->present;
      
  switch (pdu_p->present) {
    case NGAP_PDU_PR_initiatingMessage:
      return ngap_amf_decode_initiating (message, &pdu_p->choice.initiatingMessage, message_id);
      
    case NGAP_PDU_PR_successfulOutcome:
//      return ngap_amf_decode_successfull_outcome (message, &pdu_p->choice.successfulOutcome, message_id);

    case NGAP_PDU_PR_unsuccessfulOutcome:
//      return ngap_amf_decode_unsuccessfull_outcome (message, &pdu_p->choice.unsuccessfulOutcome, message_id);
        
    default:
      OAILOG_ERROR (LOG_S1AP, "Unknown message outcome (%d) or not implemented", (int)pdu_p->present);
      break;
  }
  ASN_STRUCT_FREE(asn_DEF_NGAP_PDU, pdu_p);

  return -1;

}

