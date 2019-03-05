#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "ngap_amf_handlers.h"
#include "log.h"
#include "ngap_amf.h"
#include "common_defs.h"
#include "ngap_ies_defs.h"
#include "NGAP-PDU.h"
#include "assertions.h"

ngap_message_decoded_callback   messages_callback[][3] = {
    {0,0,0}, /*AMFConfigurationUpdate*/
//    {0,0,0}, /*AMFStatusIndication*/
//    {0,0,0}, /*CellTrafficTrace*/
//    {0,0,0}, /*DeactivateTrace*/
//    {0,0,0}, /*DownlinkNASTransport*/
//    {0,0,0}, /*DownlinkNonUEAssociatedNRPPaTransport*/
//    {0,0,0}, /*DownlinkRANConfigurationTransfer*/
//    {0,0,0}, /*DownlinkRANStatusTransfer*/
//    {0,0,0}, /*DownlinkUEAssociatedNRPPaTransport*/
//    {ngap_amf_handle_error_indication,0,0}, /*ErrorIndication*/
//    {0,0,0}, /*HandoverCancel*/
//    {0,0,0}, /*HandoverNotification*/
//    {0,0,0}, /*HandoverPreparation*/
//    {0,0,0}, /*HandoverResourceAllocation*/
//    {
//     0,ngap_amf_handle_initial_context_setup_response,
//     ngap_amf_handle_initial_context_setup_failure}, /*InitialContextSetup*/
//    {ngap_amf_handle_initial_ue_message,0,0}, /*InitialUEMessage*/
//    {0,0,0}, /*LocationReportingControl*/
//    {0,0,0}, /*LocationReportingFailureIndication*/
//    {0,0,0}, /*LocationReportNASNonDeliveryIndication*/
//    {0,0,0}, /*NGReset*/
//    {ngap_amf_handle_ng_setup_request,0,0}, /*NGSetup*/
//    {0,0,0}, /*OverloadStart*/
//    {0,0,0}, /*OverloadStop*/
//    {0,0,0}, /*Paging*/
//    {ngap_amf_handle_path_switch_request,0,0}, /*PathSwitchRequest*/
//    {0,0,0}, /*PDUSessionResourceModify*/
//    {0,0,0}, /*PDUSessionResourceModifyIndication*/
//   {0,0,0}, /*PDUSessionResourceRelease*/
//    {0,0,0}, /*PDUSessionResourceSetup*/
//    {0,0,0}, /*PDUSessionResourceNotify*/
//    {0,0,0}, /*PrivateMessage*/
//    {0,0,0}, /*PWSCancel*/
//    {0,0,0}, /*PWSFailureIndication*/
//    {0,0,0}, /*PWSRestartIndication*/
//    {0,0,0}, /*RANConfigurationUpdate*/
//    {0,0,0}, /*RerouteNASRequest*/
//    {0,0,0}, /*RRCInactiveTransitionReport*/
//    {0,0,0}, /*TraceFailureIndication*/
//    {0,0,0}, /*TraceStart*/
//    {0,0,0}, /*UEContextModification*/
//    {0,ngap_amf_handle_ue_context_release_complete,0}, /*UEContextRelease*/
//    {ngap_amf_handle_ue_context_release_request,0,0}, /*UEContextReleaseRequest*/
//    {0,0,0}, /*UERadioCapabilityCheck*/
//    {ngap_amf_handle_ue_radio_cap_indication,0,0}, /*UERadioCapabilityInfoIndication*/
//    {0,0,0}, /*UETNLABindingRelease*/
//    {ngap_amf_handle_uplink_nas_transport,0,0}, /*UplinkNASTransport*/
//    {0,0,0}, /*UplinkNonUEAssociatedNRPPaTransport*/
//    {0,0,0}, /*UplinkRANConfigurationTransfer*/
//    {0,0,0}, /*UplinkRANStatusTransfer*/
//    {0,0,0}, /*UplinkUEAssociatedNRPPaTransport*/
    {0,0,0} /*WriteReplaceWarning*/
};

const char                             *ngap_direction2String[] = {
  "",                           /* Nothing */
  "Originating message",        /* originating message */
  "Successfull outcome",        /* successfull outcome */
  "UnSuccessfull outcome",      /* successfull outcome */
};

int
ngap_amf_handle_message(
    const sctp_assoc_id_t assoc_id,
    const sctp_stream_id_t stream,
    struct ngap_message_s *message){
  
  if ((message->procedureCode > (sizeof (messages_callback) / (3 * sizeof (ngap_message_decoded_callback)))) || (message->direction > NGAP_PDU_PR_unsuccessfulOutcome)) {
    OAILOG_DEBUG (LOG_S1AP, "[SCTP %d] Either procedureCode %d or direction %d exceed expected\n", assoc_id, (int)message->procedureCode, (int)message->direction);
    return -1;  
  }             
        
  if (messages_callback[message->procedureCode][message->direction - 1] == NULL) {
    OAILOG_DEBUG (LOG_S1AP, "[SCTP %d] No handler for procedureCode %d in %s\n", assoc_id, (int)message->procedureCode, ngap_direction2String[(int)message->direction]);
    return -2;
  }     
      
  return (*messages_callback[message->procedureCode][message->direction - 1]) (assoc_id, stream, message);
}



////////////////////////////////////////////////////////////////////////////////////////////////////////
//**************************** Callback Functions (EP) ***********************************************//
////////////////////////////////////////////////////////////////////////////////////////////////////////
int 
ngap_amf_handle_ng_setup_request(
    const sctp_assoc_id_t assoc_id,
    const sctp_stream_id_t stream,
    struct ngap_message_s *message){
    int rc = RETURNok;
    NGSetupRequestIEs_t * ngSetupRequest_p = NULL;
    gnb_description_t   * gnb_association = NULL; 
    uint32_t              gnb_id = 0;
    char                 *gnb_name = NULL;
    int                   ta_ret = 0;
    uint32_t              max_gnb_connected = 0;

    OAILOG_FUNC_IN(LOG_S1AP); 

    DevAssert(message != NULL);
    ngSetupRequest_p = &message->msg.ngSetupRequestIEs;
}


int     
ngap_handle_new_association (
  sctp_new_peer_t * sctp_new_peer_p)
{
//  printf("enter func ngap_handle_new_association\n"); 
    
  gnb_description_t                      *gnb_association = NULL;
    
  OAILOG_FUNC_IN (LOG_S1AP);
/*
  DevAssert (sctp_new_peer_p != NULL);
  if ((enb_association = s1ap_is_enb_assoc_id_in_list (sctp_new_peer_p->assoc_id)) == NULL) {
    OAILOG_DEBUG (LOG_S1AP, "Create eNB context for assoc_id: %d\n", sctp_new_peer_p->assoc_id);
    enb_association = s1ap_new_enb ();
    if (enb_association == NULL) {
      OAILOG_ERROR (LOG_S1AP, "Failed to allocate eNB context for assoc_id: %d\n", sctp_new_peer_p->assoc_id);
      OAILOG_FUNC_RETURN(LOG_S1AP, RETURNok);
    }
    enb_association->sctp_assoc_id = sctp_new_peer_p->assoc_id;
    hashtable_rc_t  hash_rc = hashtable_ts_insert (&g_s1ap_enb_coll, (const hash_key_t)enb_association->sctp_assoc_id, (void *)enb_association);
    if (HASH_TABLE_OK != hash_rc) {
      OAILOG_FUNC_RETURN (LOG_S1AP, RETURNerror);
    }
  } else if ((enb_association->s1_state == S1AP_SHUTDOWN) || (enb_association->s1_state == S1AP_RESETING)) {
    OAILOG_WARNING(LOG_S1AP, "Received new association request on an association that is being %s, ignoring",
                   s1_enb_state_str[enb_association->s1_state]);
    OAILOG_FUNC_RETURN(LOG_S1AP, RETURNerror);
  } else {
    OAILOG_DEBUG (LOG_S1AP, "eNB context already exists for assoc_id: %d, update it\n", sctp_new_peer_p->assoc_id);
  }

  enb_association->sctp_assoc_id = sctp_new_peer_p->assoc_id;
  enb_association->instreams = (sctp_stream_id_t) sctp_new_peer_p->instreams;
  enb_association->outstreams = (sctp_stream_id_t) sctp_new_peer_p->outstreams;
  enb_association->next_sctp_stream = 1;
  enb_association->s1_state = S1AP_INIT;
  MSC_LOG_EVENT (MSC_S1AP_MME, "0 Event SCTP_NEW_ASSOCIATION assoc_id: %d", enb_association->sctp_assoc_id);
  OAILOG_FUNC_RETURN (LOG_S1AP, RETURNok);
*/
}


/*
int
ngap_amf_handle_error_ind_message (const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream, struct ngap_message_s *message)
{
  OAILOG_FUNC_IN (LOG_S1AP);
  OAILOG_DEBUG (LOG_S1AP, "*****ERROR IND is not supported*****\n");
  OAILOG_FUNC_RETURN (LOG_S1AP, RETURNok);
}
*/
/*
int
ngap_amf_handle_path_switch_request (
    __attribute__((unused)) const sctp_assoc_id_t assoc_id,
    __attribute__((unused)) const sctp_stream_id_t stream,
    struct ngap_message_s *message)
{

}
*/
