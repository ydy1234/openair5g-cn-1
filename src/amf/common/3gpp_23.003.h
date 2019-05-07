#ifndef FILE_3GPP_23_003_SEEN
#define FILE_3GPP_23_003_SEEN

#include <stdint.h>

typedef struct plmn_s {
  uint8_t mcc_digit2:4;
  uint8_t mcc_digit1:4;
  uint8_t mnc_digit3:4;
  uint8_t mcc_digit3:4;
  uint8_t mnc_digit2:4;
  uint8_t mnc_digit1:4;
} plmn_t;

#define INVALID_TAC_0000                  (uint16_t)0x0000
#define INVALID_TAC_FFFE                  (uint16_t)0xFFFE

typedef uint16_t    tac_t;

typedef struct tai_s {
  plmn_t plmn;                                             /*!< \brief  <MCC> + <MNC>        */
  tac_t  tac;                                              /*!< \brief  Tracking Area Code   */
} tai_t;

typedef struct eci_s {
  uint32_t gnb_id:20; 
  uint32_t cell_id:8;
  uint32_t empty:4;
} ci_t;

typedef struct cgi_s {
  plmn_t   plmn;
  ci_t    cell_identity;           //28 bits 
} cgi_t;



#endif
