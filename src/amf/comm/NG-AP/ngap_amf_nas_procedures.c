#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "assertions.h"
#include "hashtable.h"
#include "log.h"
#include "conversions.h"
#include "intertask_interface.h"
#include "asn1_conversions.h"
#include "ngap_common.h"
#include "ngap_ies_defs.h"
#include "ngap_amf_encoder.h"
#include "ngap_amf_itti_messaging.h"
#include "ngap_amf.h"
#include "dynamic_memory_check.h"



int
ngap_amf_handle_initial_ue_message (
  const sctp_assoc_id_t assoc_id,
  const sctp_stream_id_t stream,
  struct ngap_message_s *message)
{
  InitialUEMessage_IEs_t    *initialUEMessage_p = NULL;
  ue_description_t          *ue_ref = NULL;
  gnb_description_t         *gnb_ref = NULL;
  ran_ue_ngap_id_t           ran_ue_ngap_id = 0;

  OAILOG_FUNC_IN(LOG_S1AP);

  initialUEMessage_p = &message->msg.initialUEMessage_IEs;
  OAILOG_INFO (LOG_S1AP, "Received S1AP INITIAL_UE_MESSAGE RAN_UE_NGAP_ID " RAN_UE_NGAP_ID_FMT "\n", (ran_ue_ngap_id_t)initialUEMessage_p->ran_ue_ngap_id);

  if((gnb_ref = ngap_is_gnb_assoc_id_in_list(assoc_id)) == NULL){
    OAILOG_ERROR (LOG_S1AP, "Unknown gNB on assoc_id %d\n", assoc_id);
    OAILOG_FUNC_RETURN (LOG_S1AP, RETURNerror);
  }
  ran_ue_ngap_id = (ran_ue_ngap_id_t)(initialUEMessage_p->ran_ue_ngap_id & 0x00ffffff);
  OAILOG_INFO (LOG_S1AP, "New Initial UE message received with RAN UE NGAP ID: " RAN_UE_NGAP_ID_FMT "\n", ran_ue_ngap_id);

  ue_ref = ngap_is_ue_gnb_id_in_list (gnb_ref, ran_ue_ngap_id);
  if(ue_ref == NULL){
   //IDs ???
    if((ue_ref = ngap_new_ue(assoc_id,ran_ue_ngap_id)) == NULL){
      OAILOG_ERROR (LOG_S1AP, "NGAP:Initial UE Message- Failed to allocate NGAP UE Context, ran_ue_ngap_id:" RAN_UE_NGAP_ID_FMT "\n", ran_ue_ngap_id);
      OAILOG_FUNC_RETURN (LOG_S1AP, RETURNerror);
    }
    ue_ref->ng_ue_state = NGAP_UE_WAITING_CSR;
    ue_ref->ran_ue_ngap_id = ran_ue_ngap_id;
    ue_ref->amf_ue_ngap_id = INVALID_AMF_UE_NGAP_ID;
    ue_ref->ngap_ue_context_rel_timer.id = NGAP_TIMER_INACTIVE_ID;
    ue_ref->ngap_ue_context_rel_timer.sec = NGAP_UE_CONTEXT_REL_COMP_TIMER;
    ue_ref->sctp_stream_recv = stream;
    ue_ref->sctp_stream_send = ue_ref->gnb->next_sctp_stream;
    ue_ref->gnb->next_sctp_stream += 1;
    if(ue_ref->gnb->next_sctp_stream >= ue_ref->gnb->instreams){
      ue_ref->gnb->next_sctp_stream = 1;
    }
    //ngap_dump_gnb(ue_ref->gnb);
    if(initialUEMessage_p->presenceMask & INITIALUEMESSAGE_IES_FIVEG_S_TMSI_PRESENT){} 
    if(initialUEMessage_p->presenceMask & INITIALUEMESSAGE_IES_AMFSETID_PRESENT){} 
    if(initialUEMessage_p->presenceMask & INITIALUEMESSAGE_IES_UECONTEXTREQUEST_PRESENT){} 
    if(initialUEMessage_p->presenceMask & INITIALUEMESSAGE_IES_ALLOWEDNSSAI_PRESENT){}
    ngap_amf_itti_amf_app_initial_ue_message(
      assoc_id,
      ue_ref->gnb->gnb_id,
      ue_ref->ran_ue_ngap_id,
      ue_ref->amf_ue_ngap_id,
      initialUEMessage_p->nas_pdu.buf,
      initialUEMessage_p->nas_pdu.size,
      initialUEMessage_p->rrcEstablishmentCause,
      NULL,
      NULL,
      NULL,
      NULL
      ); 
  }

}
