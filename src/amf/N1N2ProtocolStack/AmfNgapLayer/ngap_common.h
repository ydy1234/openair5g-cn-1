#ifndef FILE_NGAP_COMMON_SEEN
#define FILE_NGAP_COMMON_SEEN

#include "common_types.h"

#include "bstrlib.h"

/* Defined in asn_internal.h */
// extern int asn_debug_indent;
extern int asn_debug;

#if defined(EMIT_ASN_DEBUG_EXTERN)
inline void ASN_DEBUG(const char *fmt, ...);
#endif

#include "Ngap_ProtocolIE-Field.h"
#include "Ngap_NGAP-PDU.h"
#include "Ngap_InitiatingMessage.h"
#include "Ngap_SuccessfulOutcome.h"
#include "Ngap_UnsuccessfulOutcome.h"
#include "Ngap_ProtocolIE-Field.h"
#include "Ngap_ProtocolIE-FieldPair.h"
#include "Ngap_ProtocolIE-ContainerPair.h"
#include "Ngap_ProtocolExtensionField.h"
#include "Ngap_ProtocolExtensionContainer.h"
#include "Ngap_asn_constant.h"
#include "Ngap_BroadcastPLMNItem.h"
#include "Ngap_PLMNIdentity.h"
#include "Ngap_PLMNIdentity.h"
#include "Ngap_PLMNSupportItem.h"
#include "Ngap_PLMNSupportList.h"
#include "Ngap_BroadcastPLMNList.h"
#include "Ngap_SupportedTAItem.h"
#include "Ngap_NGSetupRequest.h"
#include "Ngap_NGSetupFailure.h"



typedef int (*ngap_message_decoded_callback)(
    const sctp_assoc_id_t             assoc_id,
    const sctp_stream_id_t            stream,
    struct ngap_message_s *message_p,
	Ngap_NGAP_PDU_t                   *pdu
);

/** \brief Handle criticality
 \param criticality Criticality of the IE
 @returns void
 **/
void ngap_handle_criticality(Ngap_Criticality_t criticality);

#endif
