#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "assertions.h"
//#include "ngap_common.h"
//#include "ngap_ies_defs.h"
#include "ngap_amf_handlers.h"
#include "dynamic_memory_check.h"

/*
int ngap_free_amf_decode_pdu(      
    ngap_message *message, MessagesIds message_id) {
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
} 
*/
/*
int ngap_amf_decode_pdu(
  ngap_message *message,
  const_bstring const raw,
  MessagesIds *message_id) {
  S1AP_PDU_t                              pdu = {(S1AP_PDU_PR_NOTHING)};
  S1AP_PDU_t                             *pdu_p = &pdu;
  asn_dec_rval_t                          dec_ret = {(RC_OK)};
  DevAssert (raw != NULL);
  memset ((void *)pdu_p, 0, sizeof (S1AP_PDU_t));
  dec_ret = uper_decode (NULL, &asn_DEF_S1AP_PDU, (void **)&pdu_p, bdata(raw), blength(raw), 0, 0);
        
  if (dec_ret.code != RC_OK) {
    OAILOG_ERROR (LOG_S1AP, "Failed to decode PDU\n");
    return -1;
  }
        
  message->direction = pdu_p->present;
      
  switch (pdu_p->present) {
    case S1AP_PDU_PR_initiatingMessage:
      return s1ap_mme_decode_initiating (message, &pdu_p->choice.initiatingMessage, message_id);
      
    case S1AP_PDU_PR_successfulOutcome:
      return s1ap_mme_decode_successfull_outcome (message, &pdu_p->choice.successfulOutcome, message_id);

    case S1AP_PDU_PR_unsuccessfulOutcome:
      return s1ap_mme_decode_unsuccessfull_outcome (message, &pdu_p->choice.unsuccessfulOutcome, message_id);
        
    default:
      OAILOG_ERROR (LOG_S1AP, "Unknown message outcome (%d) or not implemented", (int)pdu_p->present);
      break;
  }
  ASN_STRUCT_FREE(asn_DEF_S1AP_PDU, pdu_p);

  return -1;

}
*/
