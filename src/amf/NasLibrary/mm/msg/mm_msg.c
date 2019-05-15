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
      case AUTHENTICATION_REQUEST:
	  	  encode_result = encode_authentication_request(&msg->authentication_request, buffer, len);
	  break;	  
	  case AUTHENTICATION_RESPONSE:
	      encode_result = encode_authentication_response(&msg->authentication_response, buffer, len);	  
	  break;	  
	  case AUTHENTICATION_REJECT:
	      encode_result = encode_authentication_reject(&msg->authentication_reject, buffer, len);
	  break;
	  case AUTHENTICATION_FAILURE:
	      encode_result = encode_authentication_failure(&msg->authentication_failure, buffer, len);
	  break;
	  case AUTHENTICATION_RESULT:
	      encode_result = encode_authentication_result(&msg->authentication_result, buffer, len);
	  break;
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

int
mm_msg_decode (
  MM_msg * msg,
  uint8_t * buffer,
  uint32_t len)
{
  OAILOG_FUNC_IN (LOG_NAS_EMM);
  int                                     header_result = 0;
  int                                     decode_result = 0;
  uint8_t                                *buffer_log = buffer;
  uint32_t                                len_log = len;
  bool                                    is_down_link = false;

  /*
   * First decode the MM message header
   */
  header_result = _fivegmm_msg_decode_header (&msg->header, buffer, len);

  if (header_result < 0) {
    OAILOG_ERROR (LOG_NAS_EMM, "EMM-MSG   - Failed to decode MM message header " "(%d)\n", header_result);
    OAILOG_FUNC_RETURN (LOG_NAS_EMM, header_result);
  }

  buffer += header_result;
  len -= header_result;
  OAILOG_INFO (LOG_NAS_EMM, "EMM-MSG   - Message Type 0x%02x\n", msg->header.message_type);
  switch (msg->header.message_type) {//plain nas message e.g. registrationrequest message
      case AUTHENTICATION_REQUEST:
	      decode_result = decode_authentication_request(&msg->authentication_request, buffer, len);
	  break;
	  case AUTHENTICATION_RESPONSE:
	      decode_result = decode_authentication_response(&msg->authentication_response, buffer, len);
	  break;
	  case AUTHENTICATION_REJECT:
	  	  decode_result = decode_authentication_reject(&msg->authentication_reject, buffer, len);
	  break;
	  case AUTHENTICATION_FAILURE:
	      decode_result = decode_authentication_failure(&msg->authentication_failure, buffer, len);
	  break;
	  case AUTHENTICATION_RESULT:
	      decode_result = decode_authentication_result(&msg->authentication_result, buffer, len);
	  break;
  }
  if (decode_result < 0) {
    OAILOG_ERROR (LOG_NAS_EMM, "EMM-MSG   - Failed to decode L3 EMM message 0x%x " "(%d)\n", msg->header.message_type, decode_result);
    OAILOG_FUNC_RETURN (LOG_NAS_EMM, decode_result);
  } else {
    /*
     * Message has been decoded and security header removed, handle it has a plain message
     */
    //nas_itti_plain_msg ((char *)buffer_log, (nas_message_t *) msg, len_log, is_down_link);
  }

  OAILOG_FUNC_RETURN (LOG_NAS_EMM, header_result + decode_result);
}

static int
_fivegmm_msg_decode_header (
  mm_msg_header_t * header,
  const uint8_t * buffer,
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
   * Decode the security header type and the protocol discriminator
   */
  //DECODE_U8 (buffer + size, *(uint8_t *) (header), size);
  DECODE_U8(buffer + size, header->extended_protocol_discriminator, size);
  DECODE_U8(buffer + size, header->security_header_type, size);
  /*
   * Decode the message type
   */
  DECODE_U8 (buffer + size, header->message_type, size);

  /*
   * Check the protocol discriminator
   */
  if (header->extended_protocol_discriminator != FIVEGS_MOBILITY_MANAGEMENT_MESSAGES) {
    OAILOG_ERROR (LOG_NAS_EMM, "ESM-MSG   - Unexpected protocol discriminator: 0x%x\n", header->protocol_discriminator);
    return (TLV_PROTOCOL_NOT_SUPPORTED);
  }

  return (size);
}
