#include "bstrlib.h"
#include "common_defs.h"
#include "obj_hashtable.h"
#include "hashtable.h"
#include "3gpp_38.401.h"


typedef struct mm_data_context_s{
  amf_ue_ngap_id_t  ue_id;
  

  uint8_t           attach_type;
  uint              num_attach_request;
}mm_data_context_t;
