#include <stdio.h>
#include <stdlib.h>

#include "nas_message.h"
//#include "mm_msg.h"
#include "bstrlib.h"
#include "mmData.h"
#include "common_types.h"
#include "common_defs.h"
int main(){

  int size = NAS_MESSAGE_SECURITY_HEADER_SIZE; 
  int bytes = 0;

  nas_message_t  nas_msg;
  memset (&nas_msg,       0, sizeof (nas_message_t));

  nas_msg.header.extended_protocol_discriminator = FIVEGS_MOBILITY_MANAGEMENT_MESSAGES;
  nas_msg.header.security_header_type = SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED_CYPHERED;
  printf("security header type: %x\n",(&nas_msg)->header.security_header_type);
  uint8_t sequencenumber = 0xfe;
  uint32_t mac = 0xffffeeee;
  nas_msg.header.sequence_number = sequencenumber;
  nas_msg.header.message_authentication_code= mac;

  nas_msg.security_protected.header = nas_msg.header;

  MM_msg * mm_msg = &nas_msg.plain.mm;
  mm_msg->header.extended_protocol_discriminator = FIVEGS_MOBILITY_MANAGEMENT_MESSAGES;
  mm_msg->header.security_header_type = SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED_CYPHERED;
  mm_msg->header.message_type = AUTHENTICATION_REQUEST;

  memset (&mm_msg->specific_msg.authentication_request,       0, sizeof (authentication_request_msg));

  mm_msg->specific_msg.authentication_request.naskeysetidentifier.tsc = 1;
  mm_msg->specific_msg.authentication_request.naskeysetidentifier.naskeysetidentifier = 0b101;

  bstring abba = bfromcstralloc(10, "\0");;
  uint8_t bitStream = 0b00110100;
  abba->data = (unsigned char *)(&bitStream);
  abba->slen = 1; 
  mm_msg->specific_msg.authentication_request.abba = abba;

  //mm_msg->specific_msg.authentication_request.messagetype = AUTHENTICATION_REQUEST;
  size += MESSAGE_TYPE_MAXIMUM_LENGTH;

  nas_msg.security_protected.plain.mm = *mm_msg;

  //complete mm msg content
  if(size <= 0){
    return -1;
  }

  //construct security context
  fivegmm_security_context_t * security = calloc(1,sizeof(fivegmm_security_context_t));
  security->selected_algorithms.encryption = NAS_SECURITY_ALGORITHMS_NEA1;
  security->dl_count.overflow = 0xffff;
  security->dl_count.seq_num =  0x23;
  security->knas_enc[0] = 0x14;
  security->selected_algorithms.integrity = NAS_SECURITY_ALGORITHMS_NIA1;
  security->knas_int[0] = 0x41;
  //complete sercurity context

  int length = 60;
  unsigned char data[60];

  bstring  info = bfromcstralloc(length, "\0");//info the nas_message_encode result
  printf("1 start nas_message_encode \n");
  printf("security %p\n",security);
  printf("info %p\n",info);
  bytes = nas_message_encode (data, &nas_msg, 60/*don't know the size*/, security);
  printf("2 nas_message_encode over\n");
  int i = 0;
  for(;i<11;i++)
    printf("nas msg byte test bype[%d] = 0x%x\n",i,data[i]);
  info->data = data;
  info->slen = bytes;
 

/*************************************************************************************************************************/
/*********     NAS DECODE     ***********************/
/************************************************************************************************************************/
  
  printf("start nas_message_decode\n");
  bstring plain_msg = bstrcpy(info); 
  nas_message_security_header_t header = {0};
  //fivegmm_security_context_t  * security = NULL;
  nas_message_decode_status_t   decode_status = {0};

//  int bytes = nas_message_decrypt((*info)->data,plain_msg->data,&header,blength(*info),security,&decode_status);


  nas_message_t  decoded_nas_msg; 
  memset (&decoded_nas_msg,       0, sizeof (nas_message_t));

  int decoder_rc = RETURNok;
  printf("calling nas_message_decode\n");
  decoder_rc = nas_message_decode (plain_msg->data, &decoded_nas_msg, 60/*blength(info)*/, security, &decode_status);
  MM_msg * decoded_mm_msg = &decoded_nas_msg.plain.mm;
  printf("message type:0x%x\n",decoded_mm_msg->header.message_type);
  printf("naskey tsc:0x%x\n",decoded_mm_msg->specific_msg.authentication_request.naskeysetidentifier.tsc);
  printf("naskey tsc:0x%x\n",decoded_mm_msg->specific_msg.authentication_request.naskeysetidentifier.naskeysetidentifier);
  printf("abba buffer:0x%x\n",*(unsigned char *)((decoded_mm_msg->specific_msg.authentication_request.abba)->data));



 
  return 0;
}
