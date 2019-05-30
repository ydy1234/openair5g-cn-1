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
#include "ngap_amf_itti_messaging.h"
#include "ngap_amf_ta.h"
#include "ngap_common.h"
#include "ngap_amf_nas_procedures.h"

extern hash_table_ts_t g_ngap_gnb_coll;
extern uint32_t nb_gnb_associated;
static const char * const ng_gnb_state_str [] = {"NGAP_INIT", "NGAP_RESETTING", "NGAP_READY", "NGAP_SHUTDOWN"};


ngap_message_decoded_callback   messages_callback[][3] = {
    {0,0,0}, /*AMFConfigurationUpdate*/
    {0,0,0}, /*AMFStatusIndication*/
    {0,0,0}, /*CellTrafficTrace*/
    {0,0,0}, /*DeactivateTrace*/
    {0,0,0}, /*DownlinkNASTransport*/
    {0,0,0}, /*DownlinkNonUEAssociatedNRPPaTransport*/
    {0,0,0}, /*DownlinkRANConfigurationTransfer*/
    {0,0,0}, /*DownlinkRANStatusTransfer*/
    {0,0,0}, /*DownlinkUEAssociatedNRPPaTransport*/
    {0,0,0},/*{ngap_amf_handle_error_indication,0,0},*/ /*ErrorIndication*/
    {0,0,0}, /*HandoverCancel*/
    {0,0,0}, /*HandoverNotification*/
    {0,0,0}, /*HandoverPreparation*/
    {0,0,0}, /*HandoverResourceAllocation*/
    {0,0,0},
//    {
//     0,ngap_amf_handle_initial_context_setup_response,
//     ngap_amf_handle_initial_context_setup_failure}, /*InitialContextSetup*/
   {ngap_amf_handle_initial_ue_message,0,0}, /*InitialUEMessage*/
    {0,0,0}, /*LocationReportingControl*/
    {0,0,0}, /*LocationReportingFailureIndication*/
    {0,0,0}, /*LocationReport*/
    {0,0,0}, /*NASNonDeliveryIndication*/
    {0,0,0}, /*NGReset*/
    {ngap_amf_handle_ng_setup_request,0,0}, /*NGSetup*/
    {0,0,0}, /*OverloadStart*/
    {0,0,0}, /*OverloadStop*/
    {0,0,0}, /*Paging*/
    {0,0,0},/*{ngap_amf_handle_path_switch_request,0,0},*/ /*PathSwitchRequest*/
    {0,0,0}, /*PDUSessionResourceModify*/
    {0,0,0}, /*PDUSessionResourceModifyIndication*/
    {0,0,0}, /*PDUSessionResourceRelease*/
    {0,0,0}, /*PDUSessionResourceSetup*/
    {0,0,0}, /*PDUSessionResourceNotify*/
    {0,0,0}, /*PrivateMessage*/
    {0,0,0}, /*PWSCancel*/
    {0,0,0}, /*PWSFailureIndication*/
    {0,0,0}, /*PWSRestartIndication*/
    {0,0,0}, /*RANConfigurationUpdate*/
    {0,0,0}, /*RerouteNASRequest*/
    {0,0,0}, /*RRCInactiveTransitionReport*/
    {0,0,0}, /*TraceFailureIndication*/
    {0,0,0}, /*TraceStart*/
    {0,0,0}, /*UEContextModification*/
    {0,0,0},/*{0,ngap_amf_handle_ue_context_release_complete,0},*/ /*UEContextRelease*/
    {0,0,0},/*{ngap_amf_handle_ue_context_release_request,0,0},*/ /*UEContextReleaseRequest*/
    {0,0,0}, /*UERadioCapabilityCheck*/
    {0,0,0},/*{ngap_amf_handle_ue_radio_cap_indication,0,0},*/ /*UERadioCapabilityInfoIndication*/
    {0,0,0}, /*UETNLABindingRelease*/
    {0,0,0},/*{ngap_amf_handle_uplink_nas_transport,0,0},*/ /*UplinkNASTransport*/
    {0,0,0}, /*UplinkNonUEAssociatedNRPPaTransport*/
    {0,0,0}, /*UplinkRANConfigurationTransfer*/
    {0,0,0}, /*UplinkRANStatusTransfer*/
    {0,0,0}, /*UplinkUEAssociatedNRPPaTransport*/
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
    OAILOG_DEBUG (LOG_NGAP, "[SCTP %d] Either procedureCode %d or direction %d exceed expected\n", assoc_id, (int)message->procedureCode, (int)message->direction);
    return -1;  
  }             
      
  if (messages_callback[message->procedureCode][message->direction - 1] == NULL) {
    OAILOG_DEBUG (LOG_NGAP, "[SCTP %d] No handler for procedureCode %d in %s\n", assoc_id, (int)message->procedureCode, ngap_direction2String[(int)message->direction]);
    return -2;
  }     
      
  return (*messages_callback[message->procedureCode][message->direction - 1]) (assoc_id, stream, message);
}


//------------------------------------------------------------------------------
int
ngap_amf_set_cause (
  Cause_t * cause_p,
  const Cause_PR cause_type,
  const long cause_value)
{
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

  return 0;
}

//------------------------------------------------------------------------------
int
ngap_amf_generate_ng_setup_failure (
    const sctp_assoc_id_t assoc_id,
    const Cause_PR cause_type,
    const long cause_value,
    const long time_to_wait)
{
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

  /*
   * Include the optional field time to wait only if the value is > -1
   */
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

    OAILOG_FUNC_IN(LOG_NGAP); 

    DevAssert(message != NULL);
    ngSetupRequest_p = &message->msg.ngSetupRequestIEs;

    if (stream != 0) {
      OAILOG_ERROR (LOG_NGAP, "Received new ng setup request on stream != 0\n");
      rc = ngap_amf_generate_ng_setup_failure(assoc_id,Cause_PR_protocol,CauseProtocol_unspecified,-1);
      OAILOG_FUNC_RETURN (LOG_NGAP, rc);
    }

    if ((gnb_association = ngap_is_gnb_assoc_id_in_list(assoc_id)) == NULL) {
      OAILOG_ERROR(LOG_NGAP, "Ignoring ng setup from unknown assoc %u", assoc_id);
      OAILOG_FUNC_RETURN (LOG_NGAP, RETURNok);
    }

    if (gnb_association->ng_state == NGAP_RESETING || gnb_association->ng_state == NGAP_SHUTDOWN) {
      OAILOG_WARNING(LOG_NGAP, "Ignoring s1setup from eNB in state %s on assoc id %u",
      ng_gnb_state_str[gnb_association->ng_state], assoc_id);
      rc = ngap_amf_generate_ng_setup_failure(assoc_id,Cause_PR_transport,
                                            CauseTransport_transport_resource_unavailable,
                                            TimeToWait_v20s);
      OAILOG_FUNC_RETURN (LOG_NGAP, rc);
    }

    log_queue_item_t *context = NULL;         
    OAILOG_MESSAGE_START (OAILOG_LEVEL_DEBUG, LOG_NGAP, (&context), "New ng setup request incoming from ");

    if (ngSetupRequest_p->presenceMask & NGSETUPREQUESTIES_RANNODENAME_PRESENT) {
      OAILOG_MESSAGE_ADD (context, "%*s ", ngSetupRequest_p->ranNodeName.size, ngSetupRequest_p->ranNodeName.buf);
      gnb_name = (char *) ngSetupRequest_p->ranNodeName.buf;
    }
  
    if(ngSetupRequest_p->globalRANNodeID.choice.globalGNB_ID.gNB_ID.present == GNB_ID_PR_gNB_ID){  //which gnb id ??
      uint8_t * gnb_id_buf = ngSetupRequest_p->globalRANNodeID.choice.globalGNB_ID.gNB_ID.choice.gNB_ID.buf;
      if(ngSetupRequest_p->globalRANNodeID.choice.globalGNB_ID.gNB_ID.choice.gNB_ID.size != 28){
        //TODO: handle case that size !=28
      }
      gnb_id = (gnb_id_buf[0] << 20) + (gnb_id_buf[1] << 12) + (gnb_id_buf[2] << 4) + ((gnb_id_buf[3] & 0xf0) >> 4);
      OAILOG_MESSAGE_ADD (context, "gNB id: %07x", gnb_id);
    } else {
    }
    OAILOG_MESSAGE_FINISH(context);
    max_gnb_connected = 16;

    if(nb_gnb_associated == max_gnb_connected){
      OAILOG_ERROR (LOG_NGAP, "There is too much eNB connected to MME, rejecting the association\n");
      OAILOG_DEBUG (LOG_NGAP, "Connected = %d, maximum allowed = %d\n", nb_gnb_associated, max_gnb_connected);
      rc = ngap_amf_generate_ng_setup_failure(assoc_id,
                                            Cause_PR_misc,
                                            CauseMisc_control_processing_overload,
                                            TimeToWait_v20s);
      OAILOG_FUNC_RETURN (LOG_NGAP, rc);
    }

    ta_ret = ngap_amf_compare_ta_lists(&ngSetupRequest_p->supportedTAList);
    
    if (ta_ret != TA_LIST_RET_OK) {
      OAILOG_ERROR (LOG_NGAP, "No Common PLMN with gNB, generate_ng_setup_failure\n");
      rc = ngap_amf_generate_ng_setup_failure(assoc_id,
                                              Cause_PR_misc,
                                              CauseMisc_unknown_PLMN,
                                              TimeToWait_v20s);
      OAILOG_FUNC_RETURN (LOG_NGAP, rc);
    }
    
    OAILOG_DEBUG (LOG_NGAP, "Adding gNB to the list of served gNBs\n");

    gnb_association->gnb_id = gnb_id;
    gnb_association->default_paging_drx = ngSetupRequest_p->defaultPagingDRX;
    
    if (gnb_name != NULL) {
      memcpy(gnb_association->gnb_name, ngSetupRequest_p->ranNodeName.buf,ngSetupRequest_p->ranNodeName.size);
      gnb_association->gnb_name[ngSetupRequest_p->ranNodeName.size] = '\0';
    }

    //ngap_dump_gnb(gnb_association);
    //rc = ngap_generate_ng_setup_response(gnb_association);
    if (rc == RETURNok) {
      //update_amf_app_stats_connected_gnb_add();
    }
    OAILOG_FUNC_RETURN (LOG_NGAP, rc);
}

//------------------------------------------------------------------------------------------------------------
static int
ngap_generate_ng_setup_response(
  gnb_description_t * gnb_association)
{
  int i,j;
  int enc_rval;
  NGSetupResponseIEs_t * ng_setup_response_p = NULL;
  ServedGUAMIList_t               *servedGUAMIList = NULL;
  ServedGUAMIItem_t               *Item = NULL;
  ngap_message message = {0};
  uint8_t * buffer = NULL;
  uint32_t  length = 0;
  int rc = RETURNok;
/*
  OAILOG_FUNC_IN(LOG_NGAP);
  DevAssert(gnb_association != NULL);
  servedGUAMIList = calloc(1,sizeof (*servedGUAMIList));
  Item = calloc(1,sizeof(*Item));
  ng_setup_response_p = &message.msg.ngSetupResponseIEs;
  ng_setup_response_p->relativeAMFCapacity = 100;
  for(i=0; i< 1;i++){
    bool plmn_added = false;
    for(j=0;j<i;j++){
      
    }
    if(false == plmn_added){
      PLMNIdentity_t plmn;
      MCC_MNC_TO_PLMNID(208,93,4,&plmn);
      Item->gUAMI.pLMNIdentity = plmn;
      ASN_SEQUENCE_ADD(&servedGUAMIList->list,Item);
    }
  }

  for (i = 0; i < mme_config.gummei.nb; i++) {
    S1ap_MME_Group_ID_t                    *mme_gid = NULL;
    S1ap_MME_Code_t                        *mmec = NULL;

    mme_gid = calloc (1, sizeof (*mme_gid));
    INT16_TO_OCTET_STRING (mme_config.gummei.gummei[i].mme_gid, mme_gid);
    ASN_SEQUENCE_ADD (&servedGUMMEI->servedGroupIDs.list, mme_gid);

    mmec = calloc (1, sizeof (*mmec));
    INT8_TO_OCTET_STRING (mme_config.gummei.gummei[i].mme_code, mmec);
    ASN_SEQUENCE_ADD (&servedGUMMEI->servedMMECs.list, mmec);
  }
*/ 
}



int     
ngap_handle_new_association (
  sctp_new_peer_t * sctp_new_peer_p)
{
  gnb_description_t                      *gnb_association = NULL;
  OAILOG_FUNC_IN (LOG_NGAP);
  DevAssert (sctp_new_peer_p != NULL);

  if ((gnb_association = ngap_is_gnb_assoc_id_in_list (sctp_new_peer_p->assoc_id)) == NULL) {
    OAILOG_DEBUG (LOG_NGAP, "Create eNB context for assoc_id: %d\n", sctp_new_peer_p->assoc_id);
    gnb_association = ngap_new_gnb ();
    if (gnb_association == NULL) {
      OAILOG_ERROR (LOG_NGAP, "Failed to allocate gNB context for assoc_id: %d\n", sctp_new_peer_p->assoc_id);
      OAILOG_FUNC_RETURN(LOG_NGAP, RETURNerror);
    }
    gnb_association->sctp_assoc_id = sctp_new_peer_p->assoc_id;
    hashtable_rc_t  hash_rc = hashtable_ts_insert (&g_ngap_gnb_coll, (const hash_key_t)gnb_association->sctp_assoc_id, (void *)gnb_association);
    if (HASH_TABLE_OK != hash_rc) {
      OAILOG_FUNC_RETURN (LOG_NGAP, RETURNerror);
    }
  } else if ((gnb_association->ng_state == NGAP_SHUTDOWN) || (gnb_association->ng_state == NGAP_RESETING)) {
    OAILOG_WARNING(LOG_NGAP, "Received new association request on an association that is being %s, ignoring",
                   ng_gnb_state_str[gnb_association->ng_state]);
    OAILOG_FUNC_RETURN(LOG_NGAP, RETURNerror);
  } else {
    OAILOG_DEBUG (LOG_NGAP, "eNB context already exists for assoc_id: %d, update it\n", sctp_new_peer_p->assoc_id);
  }

  gnb_association->sctp_assoc_id = sctp_new_peer_p->assoc_id;
  gnb_association->instreams = (sctp_stream_id_t) sctp_new_peer_p->instreams;
  gnb_association->outstreams = (sctp_stream_id_t) sctp_new_peer_p->outstreams;
  gnb_association->next_sctp_stream = 1;
  gnb_association->ng_state = NGAP_INIT;
  OAILOG_FUNC_RETURN (LOG_NGAP, RETURNok);

}


/*
int
ngap_amf_handle_error_ind_message (const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream, struct ngap_message_s *message)
{
  OAILOG_FUNC_IN (LOG_NGAP);
  OAILOG_DEBUG (LOG_NGAP, "*****ERROR IND is not supported*****\n");
  OAILOG_FUNC_RETURN (LOG_NGAP, RETURNok);
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
