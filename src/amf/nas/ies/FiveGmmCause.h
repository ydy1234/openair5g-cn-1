#include <stdint.h>

#define FIVEGMM_CAUSE_MINIMUM_LENGTH 1
#define FIVEGMM_CAUSE_MAXIMUM_LENGTH 1 
  
typedef uint8_t FiveGmmCause;          
  
int encode_mm_cause(FiveGmmCause *mmcause, uint8_t iei, uint8_t *buffer, uint32_t len);
  
//void dump_mm_cause_xml(FiveGmmCause *emmcause, uint8_t iei);

int decode_mm_cause(FiveGmmCause *mmcause, uint8_t iei, uint8_t *buffer, uint32_t len);
