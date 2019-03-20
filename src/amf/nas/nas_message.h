#include "mm_msg.h"



/*********************************************************/
/************************* 9.1 24501**********************/

/* Structure of security protected header */
typedef struct nas_message_security_header_s {
#ifdef __LITTLE_ENDIAN_BITFIELD
  extended_protocol_discriminator_t    extended_protocol_discriminator;//3gpp_24.501.h
  uint8_t                         security_header_type;
#endif
#ifdef __BIG_ENDIAN_BITFIELD
  uint8_t security_header_type;
  uint8_t extended_protocol_discriminator;
#endif
  uint32_t message_authentication_code;
  uint8_t sequence_number;
} nas_message_security_header_t;

/* Structure of plain NAS message */
typedef union {
  MM_msg mm;    /* NAS MM */
  SM_msg sm;    /* NAS SM  */
} nas_message_plain_t;


/* Structure of security protected NAS message */
typedef struct nas_message_security_protected_s {
  nas_message_security_header_t header;
  nas_message_plain_t plain;
} nas_message_security_protected_t;

/*
 * Structure of a layer 3 NAS message
 */
typedef union {
  nas_message_security_header_t header;
  nas_message_security_protected_t security_protected;
  nas_message_plain_t plain;
} nas_message_t;
