#include "intertask_interface.h"

#include "ngap_common.h"
#include "ngap_ies_defs.h"
#include "ngap_amf_encoder.h"
#include "assertions.h"

static inline int                       ngap_amf_encode_initial_context_setup_request (
  ngap_message * message_p,
  uint8_t ** buffer,
  uint32_t * length);
static inline int                       ngap_amf_encode_ngsetupresponse (
  ngap_message * message_p,
  uint8_t ** buffer,
  uint32_t * length);
static inline int                       ngap_amf_encode_ngsetupfailure (
  ngap_message * message_p,
  uint8_t ** buffer,
  uint32_t * length);
static inline int                       ngap_amf_encode_ue_context_release_command (
  ngap_message * message_p,
  uint8_t ** buffer,
  uint32_t * length);
static inline int                       ngap_amf_encode_downlink_nas_transport (
  ngap_message * message_p,
  uint8_t ** buffer,
  uint32_t * length);

static inline int                       ngap_amf_encode_initiating (
  ngap_message * message_p,
  uint8_t ** buffer,
  uint32_t * length);
static inline int                       ngap_amf_encode_successfull_outcome (
  ngap_message * message_p,
  uint8_t ** buffer,
  uint32_t * len);
static inline int                       ngap_amf_encode_unsuccessfull_outcome (
  ngap_message * message_p,
  uint8_t ** buffer,
  uint32_t * len);
static inline int                       ngap_amf_encode_ngsetuprequest (
  ngap_message * message_p,
  uint8_t ** buffer,
  uint32_t * length);

int
ngap_amf_encode_pdu (
  ngap_message * message_p, 
  uint8_t ** buffer,
  uint32_t * length)
{
  DevAssert (message_p != NULL);
  DevAssert (buffer != NULL);
  DevAssert (length != NULL);
  
  switch (message_p->direction) {
  case NGAP_PDU_PR_initiatingMessage:
    return ngap_amf_encode_initiating (message_p, buffer, length);

  case NGAP_PDU_PR_successfulOutcome:
    //return ngap_amf_encode_successfull_outcome (message_p, buffer, length);

  case NGAP_PDU_PR_unsuccessfulOutcome:
    return ngap_amf_encode_unsuccessfull_outcome (message_p, buffer, length);
    
  default:
    OAILOG_DEBUG (LOG_S1AP, "Unknown message outcome (%d) or not implemented", (int)message_p->direction);
    break;
  }
  
  return -1;
}

static inline int
ngap_amf_encode_ngsetupfailure (
  ngap_message * message_p,
  uint8_t ** buffer,
  uint32_t * length)
{
  NGSetupFailure_t                   ngSetupFailure;
  NGSetupFailure_t                  *ngSetupFailure_p = &ngSetupFailure;

  memset (ngSetupFailure_p, 0, sizeof (NGSetupFailure_t));

  if (ngap_encode_ngsetupfailureies (ngSetupFailure_p, &message_p->msg.ngSetupFailureIEs) < 0) {
    return -1;
  }

  return ngap_generate_unsuccessfull_outcome (buffer, length, Ngap_ProcedureCode_id_NGSetup, message_p->criticality, &asn_DEF_NGSetupFailure, ngSetupFailure_p);
}

static inline int
ngap_amf_encode_unsuccessfull_outcome (
  ngap_message * message_p,
  uint8_t ** buffer,
  uint32_t * length)
{
  switch (message_p->procedureCode) {
  case Ngap_ProcedureCode_id_NGSetup:
    return ngap_amf_encode_ngsetupfailure (message_p, buffer, length);

  default:
    OAILOG_DEBUG (LOG_S1AP, "Unknown procedure ID (%d) for unsuccessfull outcome message\n", (int)message_p->procedureCode);
    break;
  }

  return -1;
}


static inline int
ngap_amf_encode_initiating (
  ngap_message * message_p,
  uint8_t ** buffer,
  uint32_t * length)
{
  switch (message_p->procedureCode) {
  case Ngap_ProcedureCode_id_NGSetup:
    return ngap_amf_encode_ngsetuprequest (message_p, buffer, length);

  default:
    OAILOG_DEBUG (LOG_S1AP, "Unknown procedure ID (%d) for unsuccessfull outcome message\n", (int)message_p->procedureCode);
    break;
  }

  return -1;
}

static inline int
ngap_amf_encode_ngsetuprequest (
  ngap_message * message_p,
  uint8_t ** buffer,
  uint32_t * length)
{
	NGSetupRequest_t                   ngSetupRequest;
	NGSetupRequest_t                  *ngSetupRequest_p = &ngSetupRequest;

  memset (ngSetupRequest_p, 0, sizeof (NGSetupRequest_t));

  if (ngap_encode_ngsetuprequesties (ngSetupRequest_p, &message_p->msg.ngSetupRequestIEs) < 0) {
    return -1;
  }

  return ngap_generate_initiating_message (buffer, length, Ngap_ProcedureCode_id_NGSetup, message_p->criticality, &asn_DEF_NGSetupRequest, ngSetupRequest_p);
}
/*
Ngap_IE_t                              *
ngap_new_ie (
  Ngap_ProtocolIE_ID_t id,
  Ngap_Criticality_t criticality,
  asn_TYPE_descriptor_t * type,
  void *sptr)
{ 
  Ngap_IE_t                              *buff;
  
  if ((buff = malloc (sizeof (Ngap_IE_t))) == NULL) {
    // Possible error on malloc
    return NULL;
  }
  
  memset ((void *)buff, 0, sizeof (Ngap_IE_t));
  buff->id = id;
  buff->criticality = criticality;
  
  if (ANY_fromType (&buff->value, type, sptr) < 0) {
    OAILOG_ERROR (LOG_S1AP, "Encoding of %s failed\n", type->name);
    free_wrapper ((void**) &buff);
    return NULL;
  }
  
  if (asn1_xer_print)
    if (xer_fprint (stdout, &asn_DEF_S1ap_IE, buff) < 0) {
      free_wrapper ((void**) &buff);
      return NULL;
    }
  
  return buff;
}
*/
