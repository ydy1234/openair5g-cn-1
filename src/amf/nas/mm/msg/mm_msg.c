#include "common_types.h"
#include "mm_msg.h"
#include "log.h"
#include "TLVDecoder.h"
#include "TLVEncoder.h"
#include "log.h"
//#include "nas_itti_messaging.h"

/****************************************************************************/
/*******************  L O C A L    D E F I N I T I O N S  *******************/
/****************************************************************************/

static int                              _fivegmm_msg_decode_header (
  mm_msg_header_t * header,
  const uint8_t * buffer,
  uint32_t len);
static int                              _fivegmm_msg_encode_header (
  const mm_msg_header_t * header,
  uint8_t * buffer,
  uint32_t len);


/****************************************************************************
 **                                                                        **
 ** Name:    emm_msg_encode()                                          **
 **                                                                        **
 ** Description: Encode EPS Mobility Management messages                   **
 **                                                                        **
 ** Inputs:  msg:       The EMM message structure to encode        **
 **          length:    Maximal capacity of the output buffer      **
 **      Others:    None                                       **
 **                                                                        **
 ** Outputs:     buffer:    Pointer to the encoded data buffer         **
 **          Return:    The number of bytes in the buffer if data  **
 **             have been successfully encoded;            **
 **             A negative error code otherwise.           **
 **      Others:    None                                       **
 **                                                                        **
 ***************************************************************************/
int
fivegmm_msg_encode (
  MM_msg * msg,
  uint8_t * buffer,
  uint32_t len)
{
  OAILOG_FUNC_IN (LOG_NAS_EMM);
  int                                     header_result;
  int                                     encode_result;
  uint8_t                                *buffer_log = buffer;
  bool                                    is_down_link = true;
  /*
   * First encode the EMM message header
   */
  header_result = _fivegmm_msg_encode_header (&msg->header, buffer, len);

  if (header_result < 0) {
    OAILOG_ERROR (LOG_NAS_EMM, "EMM-MSG   - Failed to encode EMM message header " "(%d)\n", header_result);
    OAILOG_FUNC_RETURN (LOG_NAS_EMM, header_result);
  }

  buffer += header_result;
  len -= header_result;
  switch (msg->header.message_type) { //need more msg encode
    //case AuthenticationRequest:
      //encode_result = encode_authentication_request(&msg->authentication_request,buffer,len);/* msg define in openair5g-cn/src/amf/nas/mm/msg  */
      //break;
  }
  if (encode_result < 0) {
    OAILOG_ERROR (LOG_NAS_EMM, "EMM-MSG   - Failed to encode L3 EMM message 0x%x " "(%d)\n", msg->header.message_type, encode_result);
  } else {
    //nas_itti_plain_msg ((char *)buffer_log, (nas_message_t *) msg, header_result + encode_result, is_down_link);
  }

  OAILOG_FUNC_RETURN (LOG_NAS_EMM, header_result + encode_result);
}


/****************************************************************************
 **                                                                        **
 ** Name:    _emm_msg_encode_header()                                  **
 **                                                                        **
 **      The protocol discriminator and the security header type   **
 **      have already been encoded.                                **
 **                                                                        **
 ** Inputs:  header:    The EMM message header to encode           **
 **          len:       Maximal capacity of the output buffer      **
 **      Others:    None                                       **
 **                                                                        **
 ** Outputs:     buffer:    Pointer to the encoded data buffer         **
 **          Return:    The number of bytes in the buffer if data  **
 **             have been successfully encoded;            **
 **             A negative error code otherwise.           **
 **      Others:    None                                       **
 **                                                                        **
 ***************************************************************************/
static int
_fivegmm_msg_encode_header (
  const mm_msg_header_t * header,
  uint8_t * buffer,
  uint32_t len)
{
  int                                     size = 0;

  /*
   * Check the buffer length
   */
  if (len < sizeof (mm_msg_header_t)) {
    return (TLV_BUFFER_TOO_SHORT);
  }
  /*
   * Check the protocol discriminator
   */
  if (header->extended_protocol_discriminator != FIVEGS_MOBILITY_MANAGEMENT_MESSAGES) {
    OAILOG_ERROR (LOG_NAS_EMM, "ESM-MSG   - Unexpected protocol discriminator: 0x%x\n", header->extended_protocol_discriminator);
    return (TLV_PROTOCOL_NOT_SUPPORTED);
  }

  /*
   * Encode the security header type and the protocol discriminator
   */
  //ENCODE_U8 (buffer + size, *(uint8_t *) (header), size);
  ENCODE_U8 (buffer + size, header->extended_protocol_discriminator , size);
  ENCODE_U8 (buffer + size, header->security_header_type , size);
  /*
   * Encode the message type
   */
  ENCODE_U8 (buffer + size, header->message_type, size);
  return (size);
}
