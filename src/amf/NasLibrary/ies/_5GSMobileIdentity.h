#ifndef _5GS_MOBILE_IDENTITY_H
#define _5GS_MOBILE_IDENTITY_H

#include <stdint.h>
#include "bstrlib.h"

#define _5GS_MOBILE_IDENTITY_MINIMUM_LENGTH 4
#define _5GS_MOBILE_IDENTITY_MAXIMUM_LENGTH 9999

typedef struct{
  uint8_t IdentityType;
  uint8_t odd_even_indication;
  union {
    struct{
      uint8_t mcc_digit_3;
      uint8_t mcc_digit_2;
      uint8_t mcc_digit_1;
      uint8_t mnc_digit_3;
      uint8_t mnc_digit_2;
      uint8_t mnc_digit_1;
      uint16_t amfRegionID;
      uint8_t amfSetID;
      uint8_t amfPointer;
      uint32_t _5g_tmsi;
    }_5g_guti;
 
    struct{
      
    }suci_imei_imeisv;
  }choice;
} _5GSMobileIdentity;

int encode__5gs_mobile_identity ( _5GSMobileIdentity _5gsmobileidentity, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;
int decode__5gs_mobile_identity ( _5GSMobileIdentity * _5gsmobileidentity, uint8_t iei, uint8_t * buffer, uint32_t len  ) ;


#endif
