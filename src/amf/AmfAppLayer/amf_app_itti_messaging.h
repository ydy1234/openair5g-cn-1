#include <stdint.h>
#include <string.h>

#include "bstrlib.h"
#include "assertions.h"
#include "common_defs.h"
#include "log.h"
#include "intertask_interface.h"
#include "common_types.h"


int amf_app_itti_send_ngap_dl_nas_transport_request(amf_ue_ngap_id_t  amf_ue_ngap_id, ran_ue_ngap_id_t  ran_ue_ngap_id, bstring nas_msg);
