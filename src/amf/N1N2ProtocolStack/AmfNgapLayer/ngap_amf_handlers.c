#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "ngap_amf_handlers.h"
#include "log.h"
#include "ngap_amf.h"
#include "common_defs.h"
#include "assertions.h"
#include "ngap_amf_itti_messaging.h"
#include "ngap_amf_ta.h"
#include "ngap_common.h"
#include "ngap_amf_nas_procedures.h"


extern hash_table_ts_t g_ngap_gnb_coll;
extern uint32_t nb_gnb_associated;
static const char * const ng_gnb_state_str [] = {"NGAP_INIT", "NGAP_RESETTING", "NGAP_READY", "NGAP_SHUTDOWN"};


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
    {ngap_amf_handle_initial_ue_message,0,0}, /*InitialUEMessage*/
//    {0,0,0}, /*LocationReportingControl*/
//    {0,0,0}, /*LocationReportingFailureIndication*/
//    {0,0,0}, /*LocationReport
//    {0,0,0}, /*NASNonDeliveryIndication*/
//    {0,0,0}, /*NGReset*/
    {ngap_amf_handle_ng_setup_request,0,0}, /*NGSetup*/
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
	Ngap_NGAP_PDU_t *pdu){
  /*
  if ((message->procedureCode > (sizeof (messages_callback) / (3 * sizeof (ngap_message_decoded_callback)))) || (message->direction > NGAP_PDU_PR_unsuccessfulOutcome)) {
    OAILOG_DEBUG (LOG_NGAP, "[SCTP %d] Either procedureCode %d or direction %d exceed expected\n", assoc_id, (int)message->procedureCode, (int)message->direction);
    return -1;  
  }             
        
  if (messages_callback[message->procedureCode][message->direction - 1] == NULL) {
    OAILOG_DEBUG (LOG_NGAP, "[SCTP %d] No handler for procedureCode %d in %s\n", assoc_id, (int)message->procedureCode, ngap_direction2String[(int)message->direction]);
    return -2;
  }     
      
  return (*messages_callback[message->procedureCode][message->direction - 1]) (assoc_id, stream, message);
  */
}


//------------------------------------------------------------------------------
int
ngap_amf_set_cause (
  Ngap_Cause_t * cause_p,
  const Ngap_Cause_PR cause_type,
  const long cause_value)
{
  /*
  DevAssert (cause_p != NULL);
  cause_p->present = cause_type;

  switch (cause_type) {
  case Cause_PR_radioNetwork:
    cause_p->choice.radioNetwork = cause_value;
    break;

  case Cause_PR_transport:
    cause_p->choice.transport = cause_value;
    break;

  case Cause_PR_nas:
    cause_p->choice.nas = cause_value;
    break;

  case Cause_PR_protocol:
    cause_p->choice.protocol = cause_value;
    break;

  case Cause_PR_misc:
    cause_p->choice.misc = cause_value;
    break;

  default:
    return -1;
  }
*/
  return 0;
}

//------------------------------------------------------------------------------
int
ngap_amf_generate_ng_setup_failure (
    const sctp_assoc_id_t assoc_id,
    const Ngap_Cause_PR cause_type,
    const long cause_value,
    const long time_to_wait)
{
	/*
  uint8_t                                *buffer_p = 0;
  uint32_t                                length = 0;
  ngap_message                            message = { 0 };
  NGSetupFailureIEs_t                    *ng_setup_failure_p = NULL;
  int                                     rc = RETURNok;

  OAILOG_FUNC_IN (LOG_NGAP);
  ng_setup_failure_p = &message.msg.ngSetupFailureIEs;
  message.procedureCode = Ngap_ProcedureCode_id_NGSetup;
  message.direction = NGAP_PDU_PR_unsuccessfulOutcome;
  ngap_amf_set_cause (&ng_setup_failure_p->cause, cause_type, cause_value);
*/
  /*
   * Include the optional field time to wait only if the value is > -1
   */
 /*
	if (time_to_wait > -1) {
    ng_setup_failure_p->presenceMask |= NGSETUPFAILUREIES_TIMETOWAIT_PRESENT;
    ng_setup_failure_p->timeToWait = time_to_wait;
  }

  if (ngap_amf_encode_pdu (&message, &buffer_p, &length) < 0) {
    OAILOG_ERROR (LOG_NGAP, "Failed to encode ng setup failure\n");
    OAILOG_FUNC_RETURN (LOG_NGAP, RETURNerror);
  }

  bstring b = blk2bstr(buffer_p, length);
  rc =  ngap_amf_itti_send_sctp_request (&b, assoc_id, 0, INVALID_AMF_UE_NGAP_ID);
  OAILOG_FUNC_RETURN (LOG_NGAP, rc);
  */
}


int
ngap_amf_handle_ng_setup_request(
    const sctp_assoc_id_t assoc_id,
    const sctp_stream_id_t stream,
	Ngap_NGAP_PDU_t *pdu){
    int rc = RETURNok;

}

//------------------------------------------------------------------------------------------------------------
static int
ngap_generate_ng_setup_response(
  gnb_description_t * gnb_association)
{


}



int
ngap_handle_new_association (
  sctp_new_peer_t * sctp_new_peer_p)
{


}


