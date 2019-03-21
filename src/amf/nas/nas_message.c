#include "common_types.h"
#include "nas_message.h"
#include "log.h"
#include "gcc_diag.h"

#include "TLVDecoder.h"
#include "TLVEncoder.h"

#include <stdlib.h>             
#include <string.h>             

//#include "nas_itti_messaging.h"
//#include "secu_defs.h"
#include "mmData.h"
#include "dynamic_memory_check.h"



/****************************************************************************/
/*******************  L O C A L    D E F I N I T I O N S  *******************/
/****************************************************************************/
#define SR_MAC_SIZE_BYTES 2

/* Functions used to decode layer 3 NAS messages */
static int _nas_message_header_decode (
    const unsigned char * const buffer,
    nas_message_security_header_t * const header,
    const size_t length,
    nas_message_decode_status_t * const status,
    bool * const is_sr);

static int _nas_message_plain_decode (
    const unsigned char *buffer,
    const nas_message_security_header_t * header,
    nas_message_plain_t * msg,
    size_t length);

static int _nas_message_protected_decode (
    unsigned char * const buffer,
    nas_message_security_header_t * header,
    nas_message_plain_t * msg,
    size_t length,
    fivegmm_security_context_t * const fivegmm_security_context,
    nas_message_decode_status_t * status);

/* Functions used to encode layer 3 NAS messages */
static int _nas_message_header_encode (
    unsigned char *buffer,
    const nas_message_security_header_t * header,
    size_t length);

static int _nas_message_plain_encode (
    unsigned char *buffer,
    const nas_message_security_header_t * header,
    const nas_message_plain_t * msg,
    size_t length);

static int _nas_message_protected_encode (
    unsigned char *buffer,
    const nas_message_security_protected_t * msg,
    size_t length,
    void *security);

/* Functions used to decrypt and encrypt layer 3 NAS messages */
static int _nas_message_decrypt (
    unsigned char * const dest,
    unsigned char * const src,
    uint8_t type,
    uint32_t code,
    uint8_t seq,
    size_t length,
    fivegmm_security_context_t * const fivegmm_security_context,
    nas_message_decode_status_t * status);

static int _nas_message_encrypt (
    unsigned char *dest,
    const unsigned char *src,
    uint8_t type,
    uint32_t code,
    uint8_t seq,
    int const direction,
    size_t length,
    fivegmm_security_context_t * const fivegmm_security_context);

/* Functions used for integrity protection of layer 3 NAS messages */
static uint32_t _nas_message_get_mac (
    const unsigned char *const buffer,
    size_t const length,
    int const direction,
    fivegmm_security_context_t * const fivegmm_security_context);




/****************************************************************************
 **                                                                        **
 ** Name:  nas_message_encode()                                      **
 **                                                                        **
 ** Description: Encode layer 3 NAS message                                **
 **                                                                        **
 ** Inputs   msg:   L3 NAS message structure to encode         **
 **    length:  Maximal capacity of the output buffer      **
 **    Others:  None                                       **
 **                                                                        **
 ** Outputs:   buffer:  Pointer to the encoded data buffer         **
 **      Return:  The number of bytes in the buffer if the   **
 **       data have been successfully encoded;       **
 **       A negative error code otherwise.           **
 **    Others:  None                                       **
 **                                                                        **
 ***************************************************************************/
int nas_message_encode (
    unsigned char *buffer,
    const nas_message_t * const msg,
    size_t length,
    void *security)
{
  OAILOG_FUNC_IN (LOG_NAS);
  fivegmm_security_context_t                 *fivegmm_security_context = (fivegmm_security_context_t *) security;
  int                                     bytes;

  /*
   * Encode the header
   */
  int                                     size = _nas_message_header_encode (buffer, &msg->header, length);

  if (size < 0) {
    OAILOG_FUNC_RETURN (LOG_NAS, TLV_BUFFER_TOO_SHORT);
  } else if (size > 1) {
    /*
     * Encode security protected NAS message
     */
    bytes = _nas_message_protected_encode (buffer + size, &msg->security_protected, length - size, fivegmm_security_context);

    /*
     * Integrity protect the NAS message
     */
    if (bytes > 0) {
      /*
       * Compute offset of the sequence number field
       */
      int                                     offset = size - sizeof (uint8_t);

      /*
       * Compute the NAS message authentication code
       */
      OAILOG_DEBUG (LOG_NAS, "offset %d = %d - %lu, hdr encode = %d, length = %lu bytes = %d\n", offset, size, sizeof (uint8_t), size, length, bytes);
      uint32_t                                mac = _nas_message_get_mac (buffer + offset,
                                                                          bytes + size - offset,
                                                                          SECU_DIRECTION_DOWNLINK,
                                                                          fivegmm_security_context);

      /*
       * Set the message authentication code of the NAS message
       */
      *(uint32_t *) (buffer + sizeof (uint8_t)) = htonl (mac);

      if (fivegmm_security_context) {
        fivegmm_security_context->dl_count.seq_num += 1;

        if (!fivegmm_security_context->dl_count.seq_num) {
          fivegmm_security_context->dl_count.overflow += 1;
        }

        OAILOG_DEBUG (LOG_NAS, "Incremented fivegmm_security_context.dl_count.seq_num -> %u\n", fivegmm_security_context->dl_count.seq_num);
      } else {
        OAILOG_DEBUG (LOG_NAS, "Did not increment fivegmm_security_context.dl_count.seq_num because no security context\n");
      }
    }
    /*
     * Log message header
     */

  } else {
    /*
     * Encode plain NAS message
     */
    bytes = _nas_message_plain_encode (buffer, &msg->header, &msg->plain, length);
  }

  if (bytes < 0) {
    OAILOG_FUNC_RETURN (LOG_NAS, bytes);
  }

  if (size > 1) {
    OAILOG_FUNC_RETURN (LOG_NAS, size + bytes);
  }

  OAILOG_FUNC_RETURN (LOG_NAS, bytes);
}






/****************************************************************************/
/*********************  L O C A L    F U N C T I O N S  *********************/
/****************************************************************************/

/****************************************************************************
 **                                                                        **
 ** Name:  _nas_message_header_encode()                              **
 **                                                                        **
 ** Description: Encode header of a security protected NAS message         **
 **                                                                        **
 ** Inputs   header:  Security header structure to encode        **
 **    length:  Maximal capacity of the output buffer      **
 **    Others:  None                                       **
 **                                                                        **
 ** Outputs:   buffer:  Pointer to the encoded data buffer         **
 **      Return:  The number of bytes in the buffer if the   **
 **       data have been successfully encoded;       **
 **       1, if the header is not a security header  **
 **       (header of plain NAS message);             **
 **       -1 otherwise.                              **
 **    Others:  None                                       **
 **                                                                        **
 ***************************************************************************/
static int _nas_message_header_encode (
    unsigned char *buffer,
    const nas_message_security_header_t * header,
    size_t length)
{
  OAILOG_FUNC_IN (LOG_NAS);
  int                                     size = 0;

  /*
   * Encode the first octet of the header (security header type or EPS bearer
   * * * * identity, and protocol discriminator)
   */
  //ENCODE_U8 (buffer, *(uint8_t *) (header), size);
  ENCODE_U8 (buffer,header->extended_protocol_discriminator,size);
  ENCODE_U8 (buffer+size,header->security_header_type,size);

  if (header->extended_protocol_discriminator == FIVEG_MOBILITY_MANAGEMENT_MESSAGES) {
    if (header->security_header_type != SECURITY_HEADER_TYPE_NOT_PROTECTED) {
      if (length < NAS_MESSAGE_SECURITY_HEADER_SIZE) {
        /*
         * The buffer is not big enough to contain security header
         */
        OAILOG_WARNING(LOG_NAS, "NET-API   - The size of the header (%u) " "exceeds the buffer length (%lu)\n", NAS_MESSAGE_SECURITY_HEADER_SIZE, length);
        OAILOG_FUNC_RETURN (LOG_NAS, RETURNerror);
      }

      /*
       * Encode the message authentication code
       */
      ENCODE_U32 (buffer + size, header->message_authentication_code, size);
      /*
       * Encode the sequence number
       */
      ENCODE_U8 (buffer + size, header->sequence_number, size);
    }
  }

  OAILOG_FUNC_RETURN (LOG_NAS, size);
}

/****************************************************************************
 **                                                                        **
 ** Name:  _nas_message_protected_encode()                               **
 **                                                                        **
 ** Description: Encode security protected NAS message                     **
 **                                                                        **
 ** Inputs    msg:     Security protected NAS message structure            **
 **                    to encode                                           **
 **           length:  Maximal capacity of the output buffer               **
 **           Others:  None                                                **
 **                                                                        **
 ** Outputs:  buffer:  Pointer to the encoded data buffer                  **
 ** Return:   The number of bytes in the buffer if the                     **
 **           data have been successfully encoded;                         **
 **           A negative error code otherwise.                             **
 ** Others:   None                                                         **
 **                                                                        **
 ***************************************************************************/
static int _nas_message_protected_encode (
    unsigned char *buffer,
    const nas_message_security_protected_t * msg,
    size_t length,
    void *security)
{
  OAILOG_FUNC_IN (LOG_NAS);
  fivegmm_security_context_t                 *fivegmm_security_context = (fivegmm_security_context_t *) security;
  int                                     bytes = TLV_BUFFER_TOO_SHORT;
  unsigned char                          *plain_msg = (unsigned char *)calloc (1, length);

  if (plain_msg) {
    /*
     * Encode the security protected NAS message as plain NAS message
     */
    int                                     size = _nas_message_plain_encode (plain_msg, &msg->header,
                                                                              &msg->plain, length);

    if (size > 0) {
      /*
       * Encrypt the encoded plain NAS message
       */
      bytes = _nas_message_encrypt (buffer, plain_msg, msg->header.security_header_type, msg->header.message_authentication_code, msg->header.sequence_number,
                                    SECU_DIRECTION_DOWNLINK,
                                    size, fivegmm_security_context);
    }

    free_wrapper ((void**) &plain_msg);
  }

  OAILOG_FUNC_RETURN (LOG_NAS, bytes);
}

/****************************************************************************
 **                                                                        **
 ** Name:  _nas_message_plain_encode()                               **
 **                                                                        **
 ** Description: Encode plain NAS message                                  **
 **                                                                        **
 ** Inputs:  pd:    Protocol discriminator of the NAS message  **
 **       to encode                                  **
 **      msg:   Plain NAS message structure to encode      **
 **    length:  Maximal capacity of the output buffer      **
 **    Others:  None                                       **
 **                                                                        **
 ** Outputs:   buffer:  Pointer to the encoded data buffer         **
 **      Return:  The number of bytes in the buffer if the   **
 **       data have been successfully encoded;       **
 **       A negative error code otherwise.           **
 **    Others:  None                                       **
 **                                                                        **
 ***************************************************************************/
static int _nas_message_plain_encode (
    unsigned char *buffer,
    const nas_message_security_header_t * header,
    const nas_message_plain_t * msg,
    size_t length)
{
  OAILOG_FUNC_IN (LOG_NAS);
  int                                     bytes = TLV_PROTOCOL_NOT_SUPPORTED;

  if (header->extended_protocol_discriminator == FIVEGS_MOBILITY_MANAGEMENT_MESSAGES) {
    /*
     * Encode EPS Mobility Management L3 message
     */
    bytes = fivegmm_msg_encode ((MM_msg *) (&msg->mm), (uint8_t *) buffer, length);
  } else if (header->protocol_discriminator == FIVEGS_SESSION_MANAGEMENT_MESSAGES) {
    /*
     * Encode EPS Session Management L3 message
     */
    bytes = fivegsm_msg_encode ((SM_msg *) (&msg->sm), (uint8_t *) buffer, length);
  } else {
    /*
     * Discard L3 messages with not supported protocol discriminator
     */
    OAILOG_WARNING(LOG_NAS, "NET-API   - Protocol discriminator 0x%x is " "not supported\n", header->protocol_discriminator);
  }

  OAILOG_FUNC_RETURN (LOG_NAS, bytes);
}


/****************************************************************************
 **                                                                        **
 ** Name:  _nas_message_encrypt()                                    **
 **                                                                        **
 ** Description: Encrypt plain NAS message                                 **
 **                                                                        **
 ** Inputs   src:   Pointer to the decrypted data buffer       **
 **    security_header_type:    The security header type                   **
 **    code:    The message authentication code            **
 **    seq:   The sequence number                        **
 **    direction: The sequence number                        **
 **    length:  Maximal capacity of the output buffer      **
 **    Others:  None                                       **
 **                                                                        **
 ** Outputs:   dest:    Pointer to the encrypted data buffer       **
 **      Return:  The number of bytes in the output buffer   **
 **       if data have been successfully encrypted;  **
 **       RETURNerror otherwise.                     **
 **    Others:  None                                       **
 **                                                                        **
 ***************************************************************************/
static int _nas_message_encrypt (
    unsigned char *dest,
    const unsigned char *src,
    uint8_t security_header_type,
    uint32_t code,
    uint8_t seq,
    int const direction,
    size_t length,
    fivegmm_security_context_t * const fivegmm_security_context)
{
  nas_stream_cipher_t                     stream_cipher = {0};
  uint32_t                                count = 0;

  OAILOG_FUNC_IN (LOG_NAS);

  if (!emm_security_context) {
    OAILOG_ERROR(LOG_NAS, "No security context set for encryption protection algorithm\n");
    OAILOG_FUNC_RETURN (LOG_NAS, 0);
  }
  switch (security_header_type) {
    
  }
  OAILOG_FUNC_RETURN (LOG_NAS, length);
}


/****************************************************************************
 **                                                                        **
 ** Name:  _nas_message_get_mac()                                        **
 **                                                                        **
 ** Description: Run integrity algorithm onto cyphered or uncyphered NAS   **
 **    message encoded in the input buffer and return the compu-         **
 **    ted message authentication code                                   **
 **                                                                        **
 ** Inputs   buffer:  Pointer to the integrity protected data            **
 **       buffer                                                     **
 **    count:   Value of the uplink NAS counter                        **
 **    length:  Length of the input buffer                             **
 **      direction                                                         **
 **    Others:  None                                                   **
 **                                                                        **
 ** Outputs:   None                                                      **
 **      Return:  The message authentication code                    **
 **    Others:  None                                                   **
 **                                                                        **
 ***************************************************************************/
static uint32_t _nas_message_get_mac (
    const unsigned char *const buffer,
    size_t const length,
    int const direction,
    fivegmm_security_context_t * const fivegmm_security_context)
{
  OAILOG_FUNC_IN (LOG_NAS);

  if (!fivegmm_security_context) {
    OAILOG_DEBUG (LOG_NAS, "No security context set for integrity protection algorithm\n");
    OAILOG_FUNC_RETURN (LOG_NAS, 0);
  }

  switch (fivegmm_security_context->selected_algorithms.integrity) {
  }
  OAILOG_FUNC_RETURN (LOG_NAS, 0);
}
