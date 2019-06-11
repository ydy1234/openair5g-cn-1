#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "ngap_amf_downlink_nas_transport.h"


#include "Ngap_ProtocolIE-Field.h"
#include "Ngap_BroadcastPLMNItem.h"
#include "Ngap_GNB-ID.h"
#include "Ngap_GlobalGNB-ID.h"
#include "Ngap_PagingDRX.h"
#include "Ngap_SliceSupportItem.h"
#include "Ngap_SupportedTAItem.h"
#include "Ngap_GlobalRANNodeID.h"
#include "Ngap_SuccessfulOutcome.h"

#include "sctp_gNB_defs.h"

#include  "bstrlib.h"
#include  "intertask_interface_types.h"

#include  "Ngap_CriticalityDiagnostics-IE-Item.h"
#include  "Ngap_PLMNSupportItem.h"
Ngap_NGAP_PDU_t *make_NGAP_DownlinkNasTransport()
{
    Ngap_NGAP_PDU_t *pdu = NULL;
	pdu = calloc(1, sizeof(Ngap_NGAP_PDU_t));
	memset(pdu, 0, sizeof(Ngap_NGAP_PDU_t));
}
