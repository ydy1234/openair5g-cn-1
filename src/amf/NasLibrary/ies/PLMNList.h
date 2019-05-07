#include <stdint.h>
#include "bstrlib.h"

#define PLMN_LIST_MINIMUM_LENGTH 5
#define PLMN_LIST_MAXIMUM_LENGTH 47

typedef bstring PLMNList;

int encode_plmn_list ( PLMNList plmnlist, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;
int decode_plmn_list ( PLMNList * plmnlist, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;

