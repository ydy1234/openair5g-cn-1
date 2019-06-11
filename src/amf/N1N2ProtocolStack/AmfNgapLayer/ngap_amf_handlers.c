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
#include "Ngap_NGAP-PDU.h"

extern hash_table_ts_t g_ngap_gnb_coll;
extern uint32_t nb_gnb_associated;
static const char * const ng_gnb_state_str [] = {"NGAP_INIT", "NGAP_RESETTING", "NGAP_READY", "NGAP_SHUTDOWN"};

extern int g_supportTaListCount;

ngap_message_decoded_callback   messages_callback[][3] = {
    {0,0,0}, /*AMFConfigurationUpdate*/
    {0,0,0}, /*AMFStatusIndication*/
    {0,0,0}, /*CellTrafficTrace*/
    {0,0,0}, /*DeactivateTrace*/
    {ngap_amf_handle_ng_downlink_nas_transport,0,0}, /*DownlinkNASTransport*/
    
    {0,0,0}, /*DownlinkNonUEAssociatedNRPPaTransport*/
    {0,0,0}, /*DownlinkRANConfigurationTransfer*/
    {0,0,0}, /*DownlinkRANStatusTransfer*/
    {0,0,0}, /*DownlinkUEAssociatedNRPPaTransport*/
    {0,0,0},//{ngap_amf_handle_error_indication,0,0}, /*ErrorIndication*/
    
    {0,0,0}, /*HandoverCancel*/
    {0,0,0}, /*HandoverNotification*/
    {0,0,0}, /*HandoverPreparation*/
    {0,0,0}, /*HandoverResourceAllocation*/
    {0,0,0},//{
     //0,ngap_amf_handle_initial_context_setup_response,
     //ngap_amf_handle_initial_context_setup_failure}, /*InitialContextSetup*/
    {ngap_amf_handle_ng_initial_ue_message,0,0},//{ngap_amf_handle_initial_ue_message,0,0}, /*InitialUEMessage*/
    {0,0,0}, /*LocationReportingControl*/
    {0,0,0}, /*LocationReportingFailureIndication*/
    {0,0,0}, /*LocationReport*/
    {0,0,0}, /*NASNonDeliveryIndication*/
    
    {0,0,0}, /*NGReset*/
    {ngap_amf_handle_ng_setup_request, ngap_amf_handle_ng_setup_response, ngap_amf_handle_ng_setup_failure}, /*NGSetup*/
    {0,0,0}, /*OverloadStart*/
	{0,0,0}, /*OverloadStop*/
    {0,0,0}, /*Paging*/
    
    {0,0,0},//{ngap_amf_handle_path_switch_request,0,0}, /*PathSwitchRequest*
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
    {0,0,0},//{0,ngap_amf_handle_ue_context_release_complete,0}, /*UEContextRelease*/
    {0,0,0},//{ngap_amf_handle_ue_context_release_request,0,0}, /*UEContextReleaseRequest*/
    {0,0,0}, /*UERadioCapabilityCheck*/
    {0,0,0},//{ngap_amf_handle_ue_radio_cap_indication,0,0}, /*UERadioCapabilityInfoIndication*/

	
    {0,0,0}, /*UETNLABindingRelease*/
    {ngap_amf_handle_ng_uplink_nas_transport,0,0},//{ngap_amf_handle_uplink_nas_transport,0,0}, /*UplinkNASTransport*/
    {0,0,0}, /*UplinkNonUEAssociatedNRPPaTransport*/
    {0,0,0}, /*UplinkRANConfigurationTransfer*/
    {0,0,0}, /*UplinkRANStatusTransfer*/

	
    {0,0,0}, /*UplinkUEAssociatedNRPPaTransport*/
    {0,0,0}, /*WriteReplaceWarning*/
	{0,0,0}, /*WriteReplaceWarning*/
	{0,0,0}, /*WriteReplaceWarning*/
	{0,0,0}, /*WriteReplaceWarning*/
	
	{0,0,0}, /*WriteReplaceWarning*/
	{0,0,0}, /*WriteReplaceWarning*/
	{0,0,0}, /*WriteReplaceWarning*/
	{0,0,0}, /*WriteReplaceWarning*/
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
  
  int procedureCode = 0, present = pdu->present;
  switch(present){
    case Ngap_NGAP_PDU_PR_initiatingMessage:
      procedureCode = pdu->choice.initiatingMessage->procedureCode;
      break;
    case Ngap_NGAP_PDU_PR_successfulOutcome:
      procedureCode = pdu->choice.successfulOutcome->procedureCode;
      break;
    case Ngap_NGAP_PDU_PR_unsuccessfulOutcome:
      procedureCode = pdu->choice.unsuccessfulOutcome->procedureCode;
      break;
	default:
	  printf("ngap_amf_handle_message unknown protocol %d\n",present);
	  return -1;
  }

  printf("ngap_amf_handle_message procedureCode:%d;present:%d\n",pdu->choice.initiatingMessage->procedureCode,pdu->present);
  if ((procedureCode > (sizeof (messages_callback) / (3 * sizeof (ngap_message_decoded_callback)))) || (present > Ngap_NGAP_PDU_PR_unsuccessfulOutcome)) {
    //OAILOG_DEBUG (LOG_NGAP, "[SCTP %d] Either procedureCode %d or direction %d exceed expected\n", assoc_id, (int)pdu->choice.initiatingMessage->procedureCode, (int)pdu->present);
    return -1;  
  }             

  if (messages_callback[procedureCode][present - 1] == NULL) {
    //OAILOG_DEBUG (LOG_NGAP, "[SCTP %d] No handler for procedureCode %d in %s\n", assoc_id, (int)pdu->choice.initiatingMessage->procedureCode, ngap_direction2String[(int)pdu->present]);
    return -2;
  }     
  printf("procedureCode:%d;present:%d\n",pdu->choice.initiatingMessage->procedureCode,pdu->present);    
  return (*messages_callback[procedureCode][present - 1]) (assoc_id, stream, pdu);
 
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

#define MAX_NG_ENG_CONNECTED 2
static unsigned int  g_NG_ENG_CONNECTED  = 0;

extern Ngap_GlobalRANNodeIDList_t   g_glocalRANNodeIDList;
extern Ngap_SupportedTAList_t	    g_SupportedTAList;
extern int g_supportTaListCount ;
int ng_setup_request_find_GlobalRANNodeID(Ngap_GlobalRANNodeID_t *req_glocalRANNodeID)
{
    //not in:add;in:return failure;
    int count = g_glocalRANNodeIDList.list.count;
    
    return 0;
}
int ng_setup_request_to_sendback_response()
{
    printf("ng_setup_request_to_sendback_response-------------\n");
    return 0;
}

int ng_setup_request_to_sendback_failure()
{
    printf("ng_setup_request_to_sendback_failure-------------\n");
    return 0;
}

int ng_setup_request_is_supported_plmn_id(Ngap_SupportedTAList_t	 req_supportedTAList)
{

  

   Ngap_SupportedTAList_t	 reqSupportedTAList;
   memset(&req_supportedTAList, 0,  sizeof(Ngap_SupportedTAList_t));
   req_supportedTAList = req_supportedTAList;
   int  count = reqSupportedTAList.list.count;
   int i  =  0;
   for(; i<count; i++)
   {
       Ngap_SupportedTAItem_t  *req_ta = NULL;
	   memset(req_ta, 0, sizeof(Ngap_SupportedTAItem_t));
	   
       req_ta = req_supportedTAList.list.array[i];
	   if(!req_ta)
	   {
           printf("ta is NULL, continue\n");
		   continue;
	   }
	   Ngap_TAC_t	 req_tAC = req_ta->tAC;
	   Ngap_BroadcastPLMNList_t	 req_broadcastPLMNList  = req_ta->broadcastPLMNList;

	   if(req_tAC.size == 0 || req_broadcastPLMNList.list.count == 0)
	   {
	       printf("req_tAC.size:%d,req_broadcastPLMNList.list.count:%d",
		   req_tAC.size,req_broadcastPLMNList.list.count);
           continue;
	   }
  
	   printf("req_tAC:0x%x,0x%x,0x%x\n",req_tAC.buf[0], req_tAC.buf[1], req_tAC.buf[2]);

	   
	   //Ngap_SupportedTAItem
	   int j  = 0;
	   for(j; j< g_supportTaListCount; j++)
	   {
           Ngap_SupportedTAItem_t *taTmp = NULL;
		   memset(taTmp, 0, sizeof(Ngap_SupportedTAItem_t));
		   taTmp = g_SupportedTAList.list.array[j];
		   if(!taTmp)
		   {
               printf("g_ta is NULL, continue\n");
			   continue;

               //tAC
			   bool tacEqual = 0;
			   Ngap_TAC_t	 tmptAC = req_ta->tAC;
			   if(req_tAC.size == tmptAC.size)
			   {
			       printf("ta size is EQUAL g_tac size:%d\n",req_tAC.size);
                   if(!memcmp(req_tAC.buf, tmptAC.buf, tmptAC.size))
			       {
				      tacEqual = 1;
					  printf("tac is EQUAL, size:%d, buf:0x%x,0x%x,0x%x\n",
					  tmptAC.size,tmptAC.buf[0],tmptAC.buf[1],tmptAC.buf[2]);
			       }
			   }

			   if(!tacEqual)
			   {
			       printf("tac is not EQUAL,continue\n");
                   continue;
			   }

               
               //broadcastPLMNList
			   Ngap_BroadcastPLMNList_t	 tmp_broadcastPLMNList  = taTmp->broadcastPLMNList;
			   int tmp_plmnCount = tmp_broadcastPLMNList.list.count;  
			   if( tmp_plmnCount != 0  && req_broadcastPLMNList.list.count == tmp_plmnCount)
			   {   
                   int req_plmnCount  = req_broadcastPLMNList.list.count;
				   int i = 0 ,j = 0;
				   for(; i< req_plmnCount; i++)
				   {
                        ;

				   }
			   }   
		    };
	    }
 
   }
   return -1;
}

int
ngap_amf_handle_ng_setup_request(
    const sctp_assoc_id_t assoc_id,
    const sctp_stream_id_t stream,
	Ngap_NGAP_PDU_t *pdu){

    #if 0
    if(g_NG_ENG_CONNECTED >= MAX_NG_ENG_CONNECTED)
    {
       ng_setup_request_to_sendback_failure();
	   return -1;
       //return NG SETUP FAILURE;
	}
    g_NG_ENG_CONNECTED++;
	#endif

    //OAILOG_FUNC_IN (LOG_NGAP);
    int rc = RETURNok;
    Ngap_NGSetupRequestIEs_t * ngSetupRequest_p = NULL;
	Ngap_NGSetupRequestIEs_t * ngSetupRequestIEs_p = NULL;
    gnb_description_t   * gnb_association = NULL;
    uint32_t              gnb_id = 0;
    char                 *gnb_name = NULL;
    int				      gnb_name_size = 0;
    int                   ta_ret = 0;
    uint32_t              max_gnb_connected = 0;
    int i = 0;
    Ngap_NGSetupRequest_t                  *container = NULL;
    Ngap_NGSetupRequestIEs_t               *ie = NULL;
    Ngap_NGSetupRequestIEs_t               *ie_gnb_name = NULL;

    printf("ngap_amf_handle_ng_setup_request\n");
    DevAssert (pdu != NULL);
	
    container = &pdu->choice.initiatingMessage->value.choice.NGSetupRequest;
	
	//ngSetupRequestIEs_p = pdu->choice.initiatingMessage->value.choice.NGSetupRequest.protocolIEs;
	
	 for (i = 0; i < container->protocolIEs.list.count; i++)
	 {
        Ngap_NGSetupRequestIEs_t *setupRequestIes_p = NULL;
        setupRequestIes_p = container->protocolIEs.list.array[i];
		if(!setupRequestIes_p)
			continue;
		switch(setupRequestIes_p->id)
	    {
            case Ngap_ProtocolIE_ID_id_GlobalRANNodeID:
			{
				Ngap_GlobalRANNodeID_t *ngap_GlobalRANNodeID = NULL;
	            ngap_GlobalRANNodeID = &setupRequestIes_p->value.choice.GlobalRANNodeID;
				if(!ngap_GlobalRANNodeID)
				    break;
				switch(ngap_GlobalRANNodeID->present)
				{
				    case Ngap_GlobalRANNodeID_PR_NOTHING:
					{
						 printf("Ngap_ProtocolIE_ID_id_GlobalRANNodeID nothing------------\n");
				    }
					break;
				    case Ngap_GlobalRANNodeID_PR_globalGNB_ID:
					{
						 printf("Ngap_GlobalRANNodeID_PR_globalGNB_ID----------\n");

						 #if 0
                         if(ng_setup_request_find_GlobalRANNodeID(ngap_GlobalRANNodeID)  == -1)
                         {
                             //return failure
						 }
                         #endif
						 
						 switch(ngap_GlobalRANNodeID->choice.globalGNB_ID->gNB_ID.present)
						 {
                            case Ngap_GNB_ID_PR_NOTHING:	/* No components present */
							break;
	                        case Ngap_GNB_ID_PR_gNB_ID:
							{
	                            unsigned long  size = ngap_GlobalRANNodeID->choice.globalGNB_ID->gNB_ID.choice.gNB_ID.size;
						        uint8_t gNB_ID[size];
								memcpy(gNB_ID, ngap_GlobalRANNodeID->choice.globalGNB_ID->gNB_ID.choice.gNB_ID.buf, size);
								printf("gNB_ID: 0x%x,0x%x,0x%x,0x%x\n",gNB_ID[0],gNB_ID[1],gNB_ID[2],gNB_ID[3]);
	                        }
							break;
							
	                        case Ngap_GNB_ID_PR_choice_Extensions:
							break;
						 } 
						 
				    }
					break;
	                case Ngap_GlobalRANNodeID_PR_globalNgENB_ID:
						 
					break;
	                case Ngap_GlobalRANNodeID_PR_globalN3IWF_ID:
						
					break;
	                case Ngap_GlobalRANNodeID_PR_choice_Extensions:
						
					break;
					default:
					{
						printf("Ngap_ProtocolIE_ID_id_GlobalRANNodeID,unknown protocol IE id(%d)\n",ngap_GlobalRANNodeID->present);
					}		
                    break;
				}
			}
			break;
            case Ngap_ProtocolIE_ID_id_RANNodeName:
			{
				printf("len:%d,RANNodeName:%s\n",setupRequestIes_p->value.choice.RANNodeName.size, setupRequestIes_p->value.choice.RANNodeName.buf);
            }		
            break;
            case Ngap_ProtocolIE_ID_id_SupportedTAList:
            {  
                Ngap_SupportedTAList_t	 SupportedTAList = setupRequestIes_p->value.choice.SupportedTAList;
				int i = 0;
				for(; i < SupportedTAList.list.count; i++)
				{
				  	Ngap_SupportedTAItem_t  *supportTA = SupportedTAList.list.array[i];
					if(!supportTA)
						continue;
					
				    printf("TAC",supportTA->tAC.buf);

					int j = 0;
					for(; j< supportTA->broadcastPLMNList.list.count; j++)
					{
                         Ngap_BroadcastPLMNItem_t *plmnItem = supportTA->broadcastPLMNList.list.array[j];
                         if(!plmnItem)
							 continue;
						 printf("pLMNIdentity:0x%x,0x%x,0x%x\n", 
						 plmnItem->pLMNIdentity.buf[0], plmnItem->pLMNIdentity.buf[1],plmnItem->pLMNIdentity.buf[2]);

						 int k = 0;
						 for(; k < plmnItem->tAISliceSupportList.list.count; k++)
						 {
                             Ngap_SliceSupportItem_t  *slisupportItem =  plmnItem->tAISliceSupportList.list.array[k];
                             if(!slisupportItem)
							 	continue;
							 	
							 printf("ssT:0x%x,0x%x,0x%x\n",
							 slisupportItem->s_NSSAI.sST.buf[0],slisupportItem->s_NSSAI.sST.buf[1],slisupportItem->s_NSSAI.sST.buf[2]);
							 
                             if(!slisupportItem->s_NSSAI.sD)
                                continue;
                             
							 printf("sd:0x%x,0x%x,0x%x\n",
							 	slisupportItem->s_NSSAI.sD->buf[0],
							 	slisupportItem->s_NSSAI.sD->buf[1],
							 	slisupportItem->s_NSSAI.sD->buf[2]); 
						 }
					}
					
				}
				#if 0
				printf("Ngap_ProtocolIE_ID_id_SupportedTAList\n");
			    if(ng_setup_request_is_supported_plmn_id(setupRequestIes_p->value.choice.SupportedTAList)== 0)
			    {
                    printf("support plmn id\n");
					
				}
				else 
			    {
                    printf("not support plmn id\n");
					//return ng setup failure;
				}
				#endif
			    break;
            }
			break;
            case Ngap_ProtocolIE_ID_id_DefaultPagingDRX:
			{
		        printf("PagingDRX:%ld\n",setupRequestIes_p->value.choice.PagingDRX);
            }
			break;
            default:
			{
		   	    printf("Unknown protocol IE id (%d) for message ngsetup_request_ies\n", (int)setupRequestIes_p->id);
            }
		    break;
		}
	 }
	 return 0;
	//printf("id:%d\n",ngSetupRequestIEs_p->id);
	//printf("criticality:%d\n",ngSetupRequestIEs_p->criticality);
	//printf("value.present:%d\n",ngSetupRequestIEs_p->value.present);
	
/*
    container = &pdu->choice.initiatingMessage->value.choice.NGSetupRequest;

    if (stream != 0) {
    	OAILOG_ERROR (LOG_NGAP, "Received new ng setup request on stream != 0\n");
    	//Send a Ngap setup failure with protocol cause unspecified
    	rc =  ngap_amf_generate_ng_setup_failure (assoc_id, Ngap_Cause_PR_protocol, Ngap_CauseProtocol_unspecified, -1);
    	OAILOG_FUNC_RETURN (LOG_NGAP, rc);
    }

    if ((gnb_association = ngap_is_gnb_assoc_id_in_list(assoc_id)) == NULL) {
      OAILOG_ERROR(LOG_NGAP, "Ignoring ng setup from unknown assoc %u", assoc_id);
      OAILOG_FUNC_RETURN (LOG_NGAP, RETURNok);
    }

    if (gnb_association->ng_state == NGAP_RESETING || gnb_association->ng_state == NGAP_SHUTDOWN) {
      OAILOG_WARNING(LOG_S1AP, "Ignoring s1setup from eNB in state %s on assoc id %u",
      ng_gnb_state_str[gnb_association->ng_state], assoc_id);
      rc = ngap_amf_generate_ng_setup_failure(assoc_id,Ngap_Cause_PR_transport,
                                            Ngap_CauseTransport_transport_resource_unavailable,
                                            Ngap_TimeToWait_v20s);
      OAILOG_FUNC_RETURN (LOG_NGAP, rc);
    }

    log_queue_item_t *context = NULL;
    OAILOG_MESSAGE_START (OAILOG_LEVEL_DEBUG, LOG_NGAP, (&context), "New ng setup request incoming from ");

    NGAP_FIND_PROTOCOLIE_BY_ID(Ngap_NGSetupRequestIEs_t, ie, container,Ngap_ProtocolIE_ID_id_RANNodeName, false);
    if (ie) {
      OAILOG_MESSAGE_ADD (context, "%*s ", (int) ie->value.choice.RANNodeName.size, ie->value.choice.RANNodeName.buf);
      gnb_name = (char *) ie->value.choice.RANNodeName.buf;
      gnb_name_size = (int) ie->value.choice.RANNodeName.size;
    }

    NGAP_FIND_PROTOCOLIE_BY_ID(Ngap_NGSetupRequestIEs_t, ie, container,Ngap_ProtocolIE_ID_id_GlobalRANNodeID, false);
    if (ie){
    	switch(ie->value.choice.GlobalRANNodeID.present){
    	case Ngap_GlobalRANNodeID_PR_globalGNB_ID:
    		//
			break;
    	case Ngap_GlobalRANNodeID_PR_globalNgENB_ID:
    		break;
    	case Ngap_GlobalRANNodeID_PR_globalN3IWF_ID:
    		break;
    	case Ngap_GlobalRANNodeID_PR_choice_Extensions:
    		break;
    	default: //Ngap_GlobalRANNodeID_PR_NOTHING
    		break;
    	}


    }

*/
/*
    if(ngSetupRequest_p->globalRANNodeID.choice.globalGNB_ID.gNB_ID.present == GNB_ID_PR_gNB_ID){  //which gnb id ??
      uint8_t * gnb_id_buf = ngSetupRequest_p->globalRANNodeID.choice.globalGNB_ID.gNB_ID.choice.gNB_ID.buf;
      if(ngSetupRequest_p->globalRANNodeID.choice.globalGNB_ID.gNB_ID.choice.gNB_ID.size != 28){
        //TODO: handle case that size !=28
      }
      gnb_id = (gnb_id_buf[0] << 20) + (gnb_id_buf[1] << 12) + (gnb_id_buf[2] << 4) + ((gnb_id_buf[3] & 0xf0) >> 4);
      OAILOG_MESSAGE_ADD (context, "gNB id: %07x", gnb_id);
    } else {
    }
  */
/*
    OAILOG_MESSAGE_FINISH(context);
    max_gnb_connected = 16;

    if(nb_gnb_associated == max_gnb_connected){
      OAILOG_ERROR (LOG_NGAP, "There is too much eNB connected to MME, rejecting the association\n");
      OAILOG_DEBUG (LOG_NGAP, "Connected = %d, maximum allowed = %d\n", nb_gnb_associated, max_gnb_connected);
      rc = ngap_amf_generate_ng_setup_failure(assoc_id,
                                            Ngap_Cause_PR_misc,
                                            Ngap_CauseMisc_control_processing_overload,
                                            Ngap_TimeToWait_v20s);
      OAILOG_FUNC_RETURN (LOG_NGAP, rc);
    }


    NGAP_FIND_PROTOCOLIE_BY_ID(Ngap_NGSetupRequestIEs_t, ie, container,Ngap_ProtocolIE_ID_id_SupportedTAList, false);
    if (ie){
       ta_ret  = ngap_amf_compare_ta_lists(&ie->value.choice.SupportedTAList);
       if (ta_ret != TA_LIST_RET_OK) {
             OAILOG_ERROR (LOG_NGAP, "No Common PLMN with gNB, generate_ng_setup_failure\n");
             rc = ngap_amf_generate_ng_setup_failure(assoc_id,
                                                     Ngap_Cause_PR_misc,
                                                     Ngap_CauseMisc_unknown_PLMN,
                                                     Ngap_TimeToWait_v20s);
             OAILOG_FUNC_RETURN (LOG_NGAP, rc);
           }
    }

    OAILOG_DEBUG (LOG_NGAP, "Adding gNB to the list of served gNBs\n");

    gnb_association->gnb_id = gnb_id;
    NGAP_FIND_PROTOCOLIE_BY_ID(Ngap_NGSetupRequestIEs_t, ie, container,Ngap_ProtocolIE_ID_id_DefaultPagingDRX, false);
 if (ie) gnb_association->default_paging_drx = ie->value.choice.PagingDRX;

    if (gnb_name != NULL) {
      memcpy(gnb_association->gnb_name, gnb_name,gnb_name_size);
      gnb_association->gnb_name[gnb_name_size] = '\0';
    }

    //ngap_dump_gnb(gnb_association);
    //rc = ngap_generate_ng_setup_response(gnb_association);
    if (rc == RETURNok) {
      //update_amf_app_stats_connected_gnb_add();
    }
    OAILOG_FUNC_RETURN (LOG_NGAP, rc);
*/

}


int ngap_amf_handle_ng_setup_response(const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
		Ngap_NGAP_PDU_t *pdu)
{
    printf("\n\n ngap_amf_handle_ng_setup_response ----------decode\n");

	//OAILOG_FUNC_IN (LOG_NGAP);
    int rc = RETURNok;
    int i = 0;
    Ngap_NGSetupResponse_t                  *container = NULL;
    Ngap_NGSetupResponseIEs_t               *ie = NULL;
    Ngap_NGSetupResponseIEs_t               *ie_gnb_name = NULL;

    DevAssert (pdu != NULL);
	
    container = &pdu->choice.successfulOutcome->value.choice.NGSetupResponse;

	for (i = 0; i < container->protocolIEs.list.count; i++)
	 {
        Ngap_NGSetupResponseIEs_t *setupResponseIes_p = NULL;
        setupResponseIes_p = container->protocolIEs.list.array[i];
		if(!setupResponseIes_p)
			continue;
		switch(setupResponseIes_p->id)
	    {
            case Ngap_ProtocolIE_ID_id_AMFName:
			{
			    printf("AMFName:%s\n", (char *)(setupResponseIes_p->value.choice.AMFName.buf));
			}
			break;
            case Ngap_ProtocolIE_ID_id_RelativeAMFCapacity:
			{
	            printf("RelativeAMFCapacity:%d\n", setupResponseIes_p->value.choice.RelativeAMFCapacity);
			}
			break;	
            case Ngap_ProtocolIE_ID_id_PLMNSupportList:
			{
				Ngap_PLMNSupportList_t	 PLMNSupportList = setupResponseIes_p->value.choice.PLMNSupportList;
				int j  = 0;
				for(; j< PLMNSupportList.list.count; j++)
				{
				    Ngap_PLMNSupportItem_t  *pPlmn = PLMNSupportList.list.array[j];
					if(!pPlmn)
					   continue;
					printf("pLMNIdentity: 0x%x,0x%x,0x%x\n", pPlmn->pLMNIdentity.buf[0],pPlmn->pLMNIdentity.buf[1],pPlmn->pLMNIdentity.buf[2]);

					Ngap_SliceSupportList_t	 sliceSupportList = pPlmn->sliceSupportList;
					int k = 0;
					for(; k < sliceSupportList.list.count; k++)
					{
                        Ngap_SliceSupportItem_t *pSS  = sliceSupportList.list.array[k];
						if(!pSS)
							continue;
						printf("NSSAI_sST:0x%x\n",pSS->s_NSSAI.sST.buf[0]);
					}
				}
				
			}
			break;
		}
	 }
	 
    return 0;
}


//------------------------------------------------------------------------------------------------------------
int
ngap_amf_handle_ng_setup_failure(
    const sctp_assoc_id_t assoc_id,
    const sctp_stream_id_t stream,
	Ngap_NGAP_PDU_t *pdu)

{
    //OAILOG_FUNC_IN (LOG_NGAP);
    int rc = RETURNok;
    Ngap_NGSetupFailureIEs_t * ngSetupFailure_p = NULL;
	Ngap_NGSetupFailureIEs_t * ngSetupFailureIEs_p = NULL;
    gnb_description_t   * gnb_association = NULL;
    uint32_t              gnb_id = 0;
    char                 *gnb_name = NULL;
    int				      gnb_name_size = 0;
    int                   ta_ret = 0;
    uint32_t              max_gnb_connected = 0;
    int i = 0;
    Ngap_NGSetupFailure_t                  *container = NULL;
    Ngap_NGSetupFailureIEs_t               *ie = NULL;
    Ngap_NGSetupFailureIEs_t               *ie_gnb_name = NULL;

    printf("ngap_amf_handle_ng_setup_Failure----------------\n");
    DevAssert (pdu != NULL);
	
    container = &pdu->choice.initiatingMessage->value.choice.NGSetupFailure;
	
	for (i = 0; i < container->protocolIEs.list.count; i++)
	{
        Ngap_NGSetupFailureIEs_t *setupFailureIes_p = NULL;
        setupFailureIes_p = container->protocolIEs.list.array[i];
		if(!setupFailureIes_p)
			continue;
		switch(setupFailureIes_p->id)
	    {
	        case Ngap_ProtocolIE_ID_id_Cause:
			{
			    printf("radioNetwork:0x%x\n", setupFailureIes_p->value.choice.Cause.choice.radioNetwork);
			}
			break;
            case Ngap_ProtocolIE_ID_id_TimeToWait:
			{
		        printf("TimeToWait:0x%x\n", setupFailureIes_p->value.choice.TimeToWait);
			}
			break;
            case Ngap_ProtocolIE_ID_id_CriticalityDiagnostics:
			{
				
            }		
            break;
            default:
			{
		   	    printf("Unknown protocol IE id (%d) for message ngsetup_failure_ies\n", (int)setupFailureIes_p->id);
            }
		    break;
		}
	 }
	 return 0;
}

static int
ngap_generate_ng_setup_response(
  gnb_description_t * gnb_association)
{

  
}

int
ngap_amf_handle_ng_initial_ue_message(
    const sctp_assoc_id_t assoc_id,
    const sctp_stream_id_t stream,
	Ngap_NGAP_PDU_t *pdu)
{
    //OAILOG_FUNC_IN (LOG_NGAP);
    int rc = RETURNok;
    Ngap_NGSetupFailureIEs_t * ngInitialUeMsg = NULL;
	Ngap_NGSetupFailureIEs_t * ngInitialUeMsgIEs_p = NULL;
    gnb_description_t   * gnb_association = NULL;
    uint32_t              gnb_id = 0;
    char                 *gnb_name = NULL;
    int				      gnb_name_size = 0;
    int                   ta_ret = 0;
    uint32_t              max_gnb_connected = 0;
    int i = 0;
    Ngap_InitialUEMessage_t                  *container = NULL;
    Ngap_InitialUEMessage_IEs_t               *ie = NULL;
    Ngap_InitialUEMessage_IEs_t               *ie_gnb_name = NULL;

    printf("ngap_amf_handle_ng_initial_ue_msg\n");
    DevAssert (pdu != NULL);
	
    container = &pdu->choice.initiatingMessage->value.choice.InitialUEMessage;
	
	for (i = 0; i < container->protocolIEs.list.count; i++)
	{
        Ngap_InitialUEMessage_IEs_t *initialUeMsgIEs_p = NULL;
        initialUeMsgIEs_p = container->protocolIEs.list.array[i];
		if(!initialUeMsgIEs_p)
			continue;
		switch(initialUeMsgIEs_p->id)
	    {
	        case Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID:
			{
				printf("RAN_UE_NGAP_ID:0x%x\n",initialUeMsgIEs_p->value.choice.RAN_UE_NGAP_ID); 
			}
			break;
            default:
			{
		   	    printf("Unknown protocol IE id (%d) for message ngsetup_failure_ies\n", (int)initialUeMsgIEs_p->id);
            }
		    break;
		}
	 }
     return 0;
}


int ngap_amf_handle_ng_uplink_nas_transport(const sctp_assoc_id_t assoc_id,
    const sctp_stream_id_t stream,
	Ngap_NGAP_PDU_t *pdu)
{
    printf("ngap_amf_handle_ng_uplink_nas_transport-----\n");
    return  0;
}

int ngap_amf_handle_ng_downlink_nas_transport(const sctp_assoc_id_t assoc_id, const sctp_stream_id_t stream,
								Ngap_NGAP_PDU_t *pdu)
{
    printf("ngap_amf_handle_ng_downlink_nas_transport-----\n");

	 //OAILOG_FUNC_IN (LOG_NGAP);
    int rc = RETURNok;
    
    int i = 0;
    Ngap_DownlinkNASTransport_t                  *container = NULL;
    Ngap_DownlinkNASTransport_IEs_t               *ie = NULL;
    bstring nas_msg;
    DevAssert (pdu != NULL);
	
    container = &pdu->choice.initiatingMessage->value.choice.DownlinkNASTransport;
	
	for (i = 0; i < container->protocolIEs.list.count; i++)
	{
        Ngap_DownlinkNASTransport_IEs_t *downlinkNasTransportIes_p = NULL;
        downlinkNasTransportIes_p = container->protocolIEs.list.array[i];
		if(!downlinkNasTransportIes_p)
			continue;
		switch(downlinkNasTransportIes_p->id)
	    {
	     case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID:
		 {
		 	printf("Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID------\n");
		 }
		 break;
         case Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID:
		 {
		 	printf("Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID------\n");
		 }
		 break;
         case Ngap_ProtocolIE_ID_id_NAS_PDU:
		 {
		 	printf("Ngap_ProtocolIE_ID_id_NAS_PDU------\n");
			nas_msg =  blk2bstr(downlinkNasTransportIes_p->value.choice.NAS_PDU.buf,downlinkNasTransportIes_p->value.choice.NAS_PDU.size);
			printf("nas_msg size:%d\n", nas_msg->slen);
			int i  = 0;
			for(; i < nas_msg->slen; i++)
			{
               printf("0x%x ", nas_msg->data[i]);
			}
		 }
		 break;
		 default:
		 	printf("unknown protocol:%d\n", downlinkNasTransportIes_p->id);
		}
	}
    return 0;
}

int
ngap_handle_new_association (
  sctp_new_peer_t * sctp_new_peer_p)
{


}


