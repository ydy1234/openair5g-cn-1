#include <stdint.h>
    
#include "ngap_common.h"
#include "dynamic_memory_check.h"
#include "log.h"

int                                     asn_debug = 0;
int                                     asn1_xer_print = 0;

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
    if (xer_fprint (stdout, &asn_DEF_Ngap_IE, buff) < 0) {
      free_wrapper ((void**) &buff);
      return NULL;
    }
  
  return buff;
}
