#include <stdio.h>
#include <stdlib.h>

#include "nas_message.h"
//#include "mm_msg.h"
#include "bstrlib.h"
#include "mmData.h"
#include "common_types.h"
#include "common_defs.h"
//add-test
#define BUF_LEN 1024
int auth_request()
{
     printf("AUTHENTICATION_REQUEST------------ start\n");
     int size = NAS_MESSAGE_SECURITY_HEADER_SIZE; 
	 int bytes = 0;
   
	 nas_message_t	nas_msg;
	 memset (&nas_msg,		 0, sizeof (nas_message_t));
   
	 nas_msg.header.extended_protocol_discriminator = FIVEGS_MOBILITY_MANAGEMENT_MESSAGES;
	 nas_msg.header.security_header_type = SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED_CYPHERED;
	 uint8_t sequencenumber = 0xfe;
	 //uint32_t mac = 0xffffeeee;
	 uint32_t mac = 0xffee;
	 nas_msg.header.sequence_number = sequencenumber;
	 nas_msg.header.message_authentication_code= mac;
   
	 nas_msg.security_protected.header = nas_msg.header;
   
	 MM_msg * mm_msg = &nas_msg.plain.mm;
	 mm_msg->header.extended_protocol_discriminator = FIVEGS_MOBILITY_MANAGEMENT_MESSAGES;
	 mm_msg->header.security_header_type = SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED_CYPHERED;
	 mm_msg->header.message_type = AUTHENTICATION_REQUEST;
   
	 memset (&mm_msg->specific_msg.authentication_request,		 0, sizeof (authentication_request_msg));
   
	 mm_msg->specific_msg.authentication_request.naskeysetidentifier.tsc = 1;
	 mm_msg->specific_msg.authentication_request.naskeysetidentifier.naskeysetidentifier = 0b101;
   
	 bstring abba = bfromcstralloc(10, "\0");
	 uint8_t bitStream_abba = 0b00110100;
	 abba->data = (unsigned char *)(&bitStream_abba);
	 abba->slen = 1; 
	 mm_msg->specific_msg.authentication_request.abba = abba;
   
	 bstring rand = bfromcstralloc(10, "\0");
	 uint8_t bitStream_rand = 0b00110111;
	 rand->data = (unsigned char *)(&bitStream_rand);
	 rand->slen = 1;
   
	 mm_msg->specific_msg.authentication_request.presence = 0x07;
	 mm_msg->specific_msg.authentication_request.authenticationparameterrand = rand;
	 mm_msg->specific_msg.authentication_request.authenticationparameterautn = abba;
	 mm_msg->specific_msg.authentication_request.eapmessage = abba;
	 
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
   
	 int length = BUF_LEN;
	 unsigned char data[BUF_LEN];
   
	 bstring  info = bfromcstralloc(length, "\0");//info the nas_message_encode result

	 #if 0
	 printf("1 start nas_message_encode \n");
	 printf("security %p\n",security);
	 printf("info %p\n",info);
	 #endif

	 printf("nas header encode extended_protocol_discriminator:0x%x\n, security_header_type:0x%x\n,sequence_number:0x%x\n,message_authentication_code:0x%x\n",
	 nas_msg.header.extended_protocol_discriminator,
	 nas_msg.header.security_header_type,
	 nas_msg.header.sequence_number,
	 nas_msg.header.message_authentication_code);

	 printf("message type:0x%x\n",mm_msg->header.message_type);
	 printf("naskey tsc:0x%x\n",mm_msg->specific_msg.authentication_request.naskeysetidentifier.tsc);
	 printf("naskey tsc:0x%x\n",mm_msg->specific_msg.authentication_request.naskeysetidentifier.naskeysetidentifier);
	 printf("abba buffer:0x%x\n",*(unsigned char *)((mm_msg->specific_msg.authentication_request.abba)->data));
	 printf("rand buffer:0x%x\n",*(unsigned char *)((mm_msg->specific_msg.authentication_request.authenticationparameterrand)->data));
	 printf("autn buffer:0x%x\n",*(unsigned char *)((mm_msg->specific_msg.authentication_request.authenticationparameterautn)->data));
	 printf("eap message buffer:0x%x\n",*(unsigned char *)((mm_msg->specific_msg.authentication_request.eapmessage)->data));

	 //bytes = nas_message_encode (data, &nas_msg, 60/*don't know the size*/, security);
	 bytes = nas_message_encode (data, &nas_msg, sizeof(data)/*don't know the size*/, security);

	
	 //printf("2 nas_message_encode over\n");
	
	 int i = 0;
	 
	 #if 0
	 for(;i<30;i++)
	   printf("nas msg byte test bype[%d] = 0x%x\n",i,data[i]);
	 #endif
	 
	 info->data = data;
	 info->slen = bytes;
	
   
   /*************************************************************************************************************************/
   /*********	  NAS DECODE	 ***********************/
   /************************************************************************************************************************/
	 
	 printf("start nas_message_decode bytes:%d\n", bytes);
	 bstring plain_msg = bstrcpy(info); 
	 nas_message_security_header_t header = {0};
	 //fivegmm_security_context_t  * security = NULL;
	 nas_message_decode_status_t   decode_status = {0};
   
   //  int bytes = nas_message_decrypt((*info)->data,plain_msg->data,&header,blength(*info),security,&decode_status);
   
   
	 nas_message_t	decoded_nas_msg; 
	 memset (&decoded_nas_msg,		 0, sizeof (nas_message_t));
   
	 int decoder_rc = RETURNok;
	 printf("calling nas_message_decode-----------\n");
	 //decoder_rc = nas_message_decode (plain_msg->data, &decoded_nas_msg, 60/*blength(info)*/, security, &decode_status);
	 decoder_rc = nas_message_decode (data, &decoded_nas_msg, sizeof(data) /*blength(info)*/, security, &decode_status);


     printf("nas header  decode extended_protocol_discriminator:0x%x\n, security_header_type:0x%x\n,sequence_number:0x%x\n,message_authentication_code:0x%x\n",
	 decoded_nas_msg.header.extended_protocol_discriminator,
	 decoded_nas_msg.header.security_header_type,
	 decoded_nas_msg.header.sequence_number,
	 decoded_nas_msg.header.message_authentication_code);

	 MM_msg * decoded_mm_msg = &decoded_nas_msg.plain.mm;
	 printf("message type:0x%x\n",decoded_mm_msg->header.message_type);
	 printf("naskey tsc:0x%x\n",decoded_mm_msg->specific_msg.authentication_request.naskeysetidentifier.tsc);
	 printf("naskey tsc:0x%x\n",decoded_mm_msg->specific_msg.authentication_request.naskeysetidentifier.naskeysetidentifier);
	 printf("abba buffer:0x%x\n",*(unsigned char *)((decoded_mm_msg->specific_msg.authentication_request.abba)->data));
	 printf("rand buffer:0x%x\n",*(unsigned char *)((decoded_mm_msg->specific_msg.authentication_request.authenticationparameterrand)->data));
	 printf("autn buffer:0x%x\n",*(unsigned char *)((decoded_mm_msg->specific_msg.authentication_request.authenticationparameterautn)->data));
	 printf("eap message buffer:0x%x\n",*(unsigned char *)((decoded_mm_msg->specific_msg.authentication_request.eapmessage)->data));

     printf("AUTHENTICATION_REQUEST------------ end\n");
     return  0;
}

int auth_response()
{
     printf("AUTHENTICATION_RESPONSE------------ start\n");
     int size = NAS_MESSAGE_SECURITY_HEADER_SIZE; 
	 int bytes = 0;
   
	 nas_message_t	nas_msg;
	 memset (&nas_msg,		 0, sizeof (nas_message_t));
   
	 nas_msg.header.extended_protocol_discriminator = FIVEGS_MOBILITY_MANAGEMENT_MESSAGES;
	 nas_msg.header.security_header_type = SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED_CYPHERED;
	 uint8_t sequencenumber = 0xfe;
	 //uint32_t mac = 0xffffeeee;
	 uint32_t mac = 0xffee;
	 nas_msg.header.sequence_number = sequencenumber;
	 nas_msg.header.message_authentication_code= mac;
   
	 nas_msg.security_protected.header = nas_msg.header;
   
	 MM_msg * mm_msg = &nas_msg.plain.mm;
	 mm_msg->header.extended_protocol_discriminator = FIVEGS_MOBILITY_MANAGEMENT_MESSAGES;
	 mm_msg->header.security_header_type = SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED_CYPHERED;
	 mm_msg->header.message_type = AUTHENTICATION_RESPONSE;
   
	 memset (&mm_msg->specific_msg.authentication_response,		 0, sizeof (authentication_response_msg));
   
	
	
	 bstring param = bfromcstralloc(10, "\0");
	 uint8_t bitStream_rand = 0b00110110;
	 param->data = (unsigned char *)(&bitStream_rand);
	 param->slen = 1;

     bstring eapmsg = bfromcstralloc(10, "\0");
	 uint8_t bitStream_eap = 0b00110101;
	 eapmsg->data = (unsigned char *)(&bitStream_eap);
	 eapmsg->slen = 1; 
	 
   
	 mm_msg->specific_msg.authentication_response.presence = 0x07;
	 mm_msg->specific_msg.authentication_response.authenticationresponseparameter = param;
	 mm_msg->specific_msg.authentication_response.eapmessage = eapmsg;
	 
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
   
	 int length = BUF_LEN;
	 unsigned char data[BUF_LEN];
   
	 bstring  info = bfromcstralloc(length, "\0");//info the nas_message_encode result

	 #if 0
	 printf("1 start nas_message_encode \n");
	 printf("security %p\n",security);
	 printf("info %p\n",info);
	 #endif

     printf("encode-----------------\n");
	 printf("nas header encode extended_protocol_discriminator:0x%x,\nsecurity_header_type:0x%x,\nsequence_number:0x%x,\nmessage_authentication_code:0x%x,\n",
	 nas_msg.header.extended_protocol_discriminator,
	 nas_msg.header.security_header_type,
	 nas_msg.header.sequence_number,
	 nas_msg.header.message_authentication_code);

	 printf("message type:0x%x\n",mm_msg->header.message_type);
	 printf("presence:0x%x\n",mm_msg->specific_msg.authentication_response.presence);
	 printf("param:0x%x\n",*(unsigned char *)((mm_msg->specific_msg.authentication_response.authenticationresponseparameter)->data));
	 printf("eap message buffer:0x%x\n",*(unsigned char *)((mm_msg->specific_msg.authentication_response.eapmessage)->data));

	 //bytes = nas_message_encode (data, &nas_msg, 60/*don't know the size*/, security);
	 bytes = nas_message_encode (data, &nas_msg, sizeof(data)/*don't know the size*/, security);

	
	 //printf("2 nas_message_encode over\n");
	
	 int i = 0;

	 #if 0
	 for(;i<30;i++)
	   printf("nas msg byte test bype[%d] = 0x%x\n",i,data[i]);
	 #endif
	 
	 info->data = data;
	 info->slen = bytes;
	
   
   /*************************************************************************************************************************/
   /*********	  NAS DECODE	 ***********************/
   /************************************************************************************************************************/
	 
	 //printf("start nas_message_decode bytes:%d\n", bytes);
	 bstring plain_msg = bstrcpy(info); 
	 nas_message_security_header_t header = {0};
	 //fivegmm_security_context_t  * security = NULL;
	 nas_message_decode_status_t   decode_status = {0};
   
   //  int bytes = nas_message_decrypt((*info)->data,plain_msg->data,&header,blength(*info),security,&decode_status);
   
   
	 nas_message_t	decoded_nas_msg; 
	 memset (&decoded_nas_msg,		 0, sizeof (nas_message_t));
   
	 int decoder_rc = RETURNok;
	 //printf("calling nas_message_decode-----------\n");
	 //decoder_rc = nas_message_decode (plain_msg->data, &decoded_nas_msg, 60/*blength(info)*/, security, &decode_status);
     
	 decoder_rc = nas_message_decode (data, &decoded_nas_msg, sizeof(data) /*blength(info)*/, security, &decode_status);
     printf("decode-----------------\n");
     printf("nas header  decode extended_protocol_discriminator:0x%x,\nsecurity_header_type:0x%x,\nsequence_number:0x%x,\nmessage_authentication_code:0x%x,\n",
	 decoded_nas_msg.header.extended_protocol_discriminator,
	 decoded_nas_msg.header.security_header_type,
	 decoded_nas_msg.header.sequence_number,
	 decoded_nas_msg.header.message_authentication_code);

	 MM_msg * decoded_mm_msg = &decoded_nas_msg.plain.mm;
	 
	 printf("message type:0x%x\n",decoded_mm_msg->header.message_type);
	 printf("presence:0x%x\n",decoded_mm_msg->specific_msg.authentication_response.presence);
	 printf("param:0x%x\n",*(unsigned char *)((decoded_mm_msg->specific_msg.authentication_response.authenticationresponseparameter)->data));
	 printf("eap message buffer:0x%x\n",*(unsigned char *)((decoded_mm_msg->specific_msg.authentication_response.eapmessage)->data));

	 printf("AUTHENTICATION_RESPONSE------------ end\n");
     return  0;
}

int auth_failure()
{
     printf("AUTHENTICATION_FAILURE------------ start\n");
     int size = NAS_MESSAGE_SECURITY_HEADER_SIZE; 
	 int bytes = 0;
   
	 nas_message_t	nas_msg;
	 memset (&nas_msg,		 0, sizeof (nas_message_t));
   
	 nas_msg.header.extended_protocol_discriminator = FIVEGS_MOBILITY_MANAGEMENT_MESSAGES;
	 nas_msg.header.security_header_type = SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED_CYPHERED;
	 uint8_t sequencenumber = 0xfe;
	 //uint32_t mac = 0xffffeeee;
	 uint32_t mac = 0xffee;
	 nas_msg.header.sequence_number = sequencenumber;
	 nas_msg.header.message_authentication_code= mac;
   
	 nas_msg.security_protected.header = nas_msg.header;
   
	 MM_msg * mm_msg = &nas_msg.plain.mm;
	 mm_msg->header.extended_protocol_discriminator = FIVEGS_MOBILITY_MANAGEMENT_MESSAGES;
	 mm_msg->header.security_header_type = SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED_CYPHERED;
	 mm_msg->header.message_type = AUTHENTICATION_FAILURE;
   
	 memset (&mm_msg->specific_msg.authentication_failure,		 0, sizeof (authentication_failure_msg));
     
	 bstring param = bfromcstralloc(10, "\0");
	 uint8_t bitStream_rand = 0b00110110;
	 param->data = (unsigned char *)(&bitStream_rand);
	 param->slen = 1;

     
	  
     mm_msg->specific_msg.authentication_failure._5gmmcause = 0x80;
	 mm_msg->specific_msg.authentication_failure.presence = 0x07;
	 mm_msg->specific_msg.authentication_failure.authenticationfailureparameter= param;
	 
	 
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
   
	 int length = BUF_LEN;
	 unsigned char data[BUF_LEN];
   
	 bstring  info = bfromcstralloc(length, "\0");//info the nas_message_encode result

	 #if 0
	 printf("1 start nas_message_encode \n");
	 printf("security %p\n",security);
	 printf("info %p\n",info);
	 #endif

     printf("encode-----------------\n");
	 printf("nas header encode extended_protocol_discriminator:0x%x,\nsecurity_header_type:0x%x,\nsequence_number:0x%x,\nmessage_authentication_code:0x%x,\n",
	 nas_msg.header.extended_protocol_discriminator,
	 nas_msg.header.security_header_type,
	 nas_msg.header.sequence_number,
	 nas_msg.header.message_authentication_code);

	 printf("message type:0x%x\n",mm_msg->header.message_type);
	 printf("presence:0x%x\n",mm_msg->specific_msg.authentication_failure.presence);
	 printf("5gmmcause :0x%x\n", mm_msg->specific_msg.authentication_failure._5gmmcause);
	 printf("param:0x%x\n",*(unsigned char *)((mm_msg->specific_msg.authentication_failure.authenticationfailureparameter)->data));
	 

	 //bytes = nas_message_encode (data, &nas_msg, 60/*don't know the size*/, security);
	 bytes = nas_message_encode (data, &nas_msg, sizeof(data)/*don't know the size*/, security);

	
	 //printf("2 nas_message_encode over\n");
	
	 int i = 0;

	 #if 0
	 for(;i<30;i++)
	   printf("nas msg byte test bype[%d] = 0x%x\n",i,data[i]);
	 #endif
	 
	 info->data = data;
	 info->slen = bytes;
	
   
   /*************************************************************************************************************************/
   /*********	  NAS DECODE	 ***********************/
   /************************************************************************************************************************/
	 
	 //printf("start nas_message_decode bytes:%d\n", bytes);
	 bstring plain_msg = bstrcpy(info); 
	 nas_message_security_header_t header = {0};
	 //fivegmm_security_context_t  * security = NULL;
	 nas_message_decode_status_t   decode_status = {0};
   
   //  int bytes = nas_message_decrypt((*info)->data,plain_msg->data,&header,blength(*info),security,&decode_status);
   
   
	 nas_message_t	decoded_nas_msg; 
	 memset (&decoded_nas_msg,		 0, sizeof (nas_message_t));
   
	 int decoder_rc = RETURNok;
	 //printf("calling nas_message_decode-----------\n");
	 //decoder_rc = nas_message_decode (plain_msg->data, &decoded_nas_msg, 60/*blength(info)*/, security, &decode_status);
     
	 decoder_rc = nas_message_decode (data, &decoded_nas_msg, sizeof(data) /*blength(info)*/, security, &decode_status);
     printf("decode-----------------\n");
     printf("nas header  decode extended_protocol_discriminator:0x%x,\nsecurity_header_type:0x%x,\nsequence_number:0x%x,\nmessage_authentication_code:0x%x,\n",
	 decoded_nas_msg.header.extended_protocol_discriminator,
	 decoded_nas_msg.header.security_header_type,
	 decoded_nas_msg.header.sequence_number,
	 decoded_nas_msg.header.message_authentication_code);

	 MM_msg * decoded_mm_msg = &decoded_nas_msg.plain.mm;
	 
	
	 printf("message type:0x%x\n",decoded_mm_msg->header.message_type);
	 printf("presence:0x%x\n",decoded_mm_msg->specific_msg.authentication_failure.presence);
	 printf("5gmmcause :0x%x\n", decoded_mm_msg->specific_msg.authentication_failure._5gmmcause);
	 printf("param:0x%x\n",*(unsigned char *)((decoded_mm_msg->specific_msg.authentication_failure.authenticationfailureparameter)->data));
	 
	 printf("AUTHENTICATION_FAILURE------------ end\n");
	 
     return  0;
}

int auth_reject()
{
     printf("AUTHENTICATION_REJECT------------ start\n");
     int size = NAS_MESSAGE_SECURITY_HEADER_SIZE; 
	 int bytes = 0;
   
	 nas_message_t	nas_msg;
	 memset (&nas_msg,		 0, sizeof (nas_message_t));
   
	 nas_msg.header.extended_protocol_discriminator = FIVEGS_MOBILITY_MANAGEMENT_MESSAGES;
	 nas_msg.header.security_header_type = SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED_CYPHERED;
	 uint8_t sequencenumber = 0xfe;
	 //uint32_t mac = 0xffffeeee;
	 uint32_t mac = 0xffee;
	 nas_msg.header.sequence_number = sequencenumber;
	 nas_msg.header.message_authentication_code= mac;
   
	 nas_msg.security_protected.header = nas_msg.header;
   
	 MM_msg * mm_msg = &nas_msg.plain.mm;
	 mm_msg->header.extended_protocol_discriminator = FIVEGS_MOBILITY_MANAGEMENT_MESSAGES;
	 mm_msg->header.security_header_type = SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED_CYPHERED;
	 mm_msg->header.message_type = AUTHENTICATION_REJECT;
   
	 memset (&mm_msg->specific_msg.authentication_reject,		 0, sizeof (authentication_reject_msg));
     
	 bstring eapmsg = bfromcstralloc(10, "\0");
	 uint8_t bitStream_eapmsg = 0b00110110;
	 eapmsg->data = (unsigned char *)(&bitStream_eapmsg);
	 eapmsg->slen = 1;
    
    
	 mm_msg->specific_msg.authentication_reject.presence = 0x07;
	 mm_msg->specific_msg.authentication_reject.eapmessage= eapmsg;
	 
	 
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
   
	 int length = BUF_LEN;
	 unsigned char data[BUF_LEN] = {'\0'};
   
	 bstring  info = bfromcstralloc(length, "\0");//info the nas_message_encode result

	 #if 0
	 printf("1 start nas_message_encode \n");
	 printf("security %p\n",security);
	 printf("info %p\n",info);
	 #endif

     printf("encode-----------------\n");
	 printf("nas header encode extended_protocol_discriminator:0x%x,\nsecurity_header_type:0x%x,\nsequence_number:0x%x,\nmessage_authentication_code:0x%x,\n",
	 nas_msg.header.extended_protocol_discriminator,
	 nas_msg.header.security_header_type,
	 nas_msg.header.sequence_number,
	 nas_msg.header.message_authentication_code);

	 printf("message type:0x%x\n",mm_msg->header.message_type);
	 printf("presence:0x%x\n",mm_msg->specific_msg.authentication_reject.presence);
	 printf("param:0x%x\n",*(unsigned char *)((mm_msg->specific_msg.authentication_reject.eapmessage)->data));
	 

	 //bytes = nas_message_encode (data, &nas_msg, 60/*don't know the size*/, security);
	 bytes = nas_message_encode (data, &nas_msg, sizeof(data)/*don't know the size*/, security);

	
	 //printf("2 nas_message_encode over\n");
	
	 int i = 0;

	 #if 0
	 for(;i<30;i++)
	   printf("nas msg byte test bype[%d] = 0x%x\n",i,data[i]);
	 #endif
	 
	 info->data = data;
	 info->slen = bytes;
	
   
   /*************************************************************************************************************************/
   /*********	  NAS DECODE	 ***********************/
   /************************************************************************************************************************/
	 
	 //printf("start nas_message_decode bytes:%d\n", bytes);
	 bstring plain_msg = bstrcpy(info); 
	 nas_message_security_header_t header = {0};
	 //fivegmm_security_context_t  * security = NULL;
	 nas_message_decode_status_t   decode_status = {0};
   
   //  int bytes = nas_message_decrypt((*info)->data,plain_msg->data,&header,blength(*info),security,&decode_status);
   
   
	 nas_message_t	decoded_nas_msg; 
	 memset (&decoded_nas_msg,		 0, sizeof (nas_message_t));
   
	 int decoder_rc = RETURNok;
	 //printf("calling nas_message_decode-----------\n");
	 //decoder_rc = nas_message_decode (plain_msg->data, &decoded_nas_msg, 60/*blength(info)*/, security, &decode_status);
     
	 decoder_rc = nas_message_decode (data, &decoded_nas_msg, sizeof(data) /*blength(info)*/, security, &decode_status);
     printf("decode-----------------\n");
     printf("nas header  decode extended_protocol_discriminator:0x%x,\nsecurity_header_type:0x%x,\nsequence_number:0x%x,\nmessage_authentication_code:0x%x,\n",
	 decoded_nas_msg.header.extended_protocol_discriminator,
	 decoded_nas_msg.header.security_header_type,
	 decoded_nas_msg.header.sequence_number,
	 decoded_nas_msg.header.message_authentication_code);

	 MM_msg * decoded_mm_msg = &decoded_nas_msg.plain.mm;
	 
	 printf("message type:0x%x\n",decoded_mm_msg->header.message_type);
	 printf("presence:0x%x\n",decoded_mm_msg->specific_msg.authentication_reject.presence);
	 printf("param:0x%x\n",*(unsigned char *)((decoded_mm_msg->specific_msg.authentication_reject.eapmessage)->data));
	 
	 
	 printf("AUTHENTICATION_REJECT------------ end\n");
	 
     return 0;
}

#if 0
int auth_result()
{
     printf("AUTHENTICATION_RESULT------------ start\n");
     int size = NAS_MESSAGE_SECURITY_HEADER_SIZE; 
	 int bytes = 0;
   
	 nas_message_t	nas_msg;
	 memset (&nas_msg,		 0, sizeof (nas_message_t));
   
	 nas_msg.header.extended_protocol_discriminator = FIVEGS_MOBILITY_MANAGEMENT_MESSAGES;
	 nas_msg.header.security_header_type = SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED_CYPHERED;
	 uint8_t sequencenumber = 0xfe;
	 //uint32_t mac = 0xffffeeee;
	 uint32_t mac = 0xffee;
	 nas_msg.header.sequence_number = sequencenumber;
	 nas_msg.header.message_authentication_code= mac;
   
	 nas_msg.security_protected.header = nas_msg.header;
   
	 MM_msg * mm_msg = &nas_msg.plain.mm;
	 mm_msg->header.extended_protocol_discriminator = FIVEGS_MOBILITY_MANAGEMENT_MESSAGES;
	 mm_msg->header.security_header_type = SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED_CYPHERED;
	 mm_msg->header.message_type = AUTHENTICATION_RESULT;
   
	 memset (&mm_msg->specific_msg.authentication_result,		 0, sizeof (authentication_result_msg));
   
	 mm_msg->specific_msg.authentication_result.naskeysetidentifier.tsc = 1;
	 mm_msg->specific_msg.authentication_result.naskeysetidentifier.naskeysetidentifier = 0b101;
   
	 bstring abba = bfromcstralloc(10, "\0");
	 uint8_t bitStream_abba = 0b00110100;
	 abba->data = (unsigned char *)(&bitStream_abba);
	 abba->slen = 1; 
	 mm_msg->specific_msg.authentication_request.abba = abba;
   
	 bstring rand = bfromcstralloc(10, "\0");
	 uint8_t bitStream_rand = 0b00110111;
	 rand->data = (unsigned char *)(&bitStream_rand);
	 rand->slen = 1;
   
	 mm_msg->specific_msg.authentication_request.presence = 0x07;
	 mm_msg->specific_msg.authentication_request.authenticationparameterrand = rand;
	 mm_msg->specific_msg.authentication_request.authenticationparameterautn = abba;
	 mm_msg->specific_msg.authentication_request.eapmessage = abba;
	 
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
   
	 int length = BUF_LEN;
	 unsigned char data[BUF_LEN];
   
	 bstring  info = bfromcstralloc(length, "\0");//info the nas_message_encode result

	 #if 0
	 printf("1 start nas_message_encode \n");
	 printf("security %p\n",security);
	 printf("info %p\n",info);
	 #endif

	 printf("nas header encode extended_protocol_discriminator:0x%x\n, security_header_type:0x%x\n,sequence_number:0x%x\n,message_authentication_code:0x%x\n",
	 nas_msg.header.extended_protocol_discriminator,
	 nas_msg.header.security_header_type,
	 nas_msg.header.sequence_number,
	 nas_msg.header.message_authentication_code);

	 printf("message type:0x%x\n",mm_msg->header.message_type);
	 printf("naskey tsc:0x%x\n",mm_msg->specific_msg.authentication_request.naskeysetidentifier.tsc);
	 printf("naskey tsc:0x%x\n",mm_msg->specific_msg.authentication_request.naskeysetidentifier.naskeysetidentifier);
	 printf("abba buffer:0x%x\n",*(unsigned char *)((mm_msg->specific_msg.authentication_request.abba)->data));
	 printf("rand buffer:0x%x\n",*(unsigned char *)((mm_msg->specific_msg.authentication_request.authenticationparameterrand)->data));
	 printf("autn buffer:0x%x\n",*(unsigned char *)((mm_msg->specific_msg.authentication_request.authenticationparameterautn)->data));
	 printf("eap message buffer:0x%x\n",*(unsigned char *)((mm_msg->specific_msg.authentication_request.eapmessage)->data));

	 //bytes = nas_message_encode (data, &nas_msg, 60/*don't know the size*/, security);
	 bytes = nas_message_encode (data, &nas_msg, sizeof(data)/*don't know the size*/, security);

	
	 //printf("2 nas_message_encode over\n");
	
	 int i = 0;
	 
	 #if 0
	 for(;i<30;i++)
	   printf("nas msg byte test bype[%d] = 0x%x\n",i,data[i]);
	 #endif
	 
	 info->data = data;
	 info->slen = bytes;
	
   
   /*************************************************************************************************************************/
   /*********	  NAS DECODE	 ***********************/
   /************************************************************************************************************************/
	 
	 printf("start nas_message_decode bytes:%d\n", bytes);
	 bstring plain_msg = bstrcpy(info); 
	 nas_message_security_header_t header = {0};
	 //fivegmm_security_context_t  * security = NULL;
	 nas_message_decode_status_t   decode_status = {0};
   
   //  int bytes = nas_message_decrypt((*info)->data,plain_msg->data,&header,blength(*info),security,&decode_status);
   
   
	 nas_message_t	decoded_nas_msg; 
	 memset (&decoded_nas_msg,		 0, sizeof (nas_message_t));
   
	 int decoder_rc = RETURNok;
	 printf("calling nas_message_decode-----------\n");
	 //decoder_rc = nas_message_decode (plain_msg->data, &decoded_nas_msg, 60/*blength(info)*/, security, &decode_status);
	 decoder_rc = nas_message_decode (data, &decoded_nas_msg, sizeof(data) /*blength(info)*/, security, &decode_status);


     printf("nas header  decode extended_protocol_discriminator:0x%x\n, security_header_type:0x%x\n,sequence_number:0x%x\n,message_authentication_code:0x%x\n",
	 decoded_nas_msg.header.extended_protocol_discriminator,
	 decoded_nas_msg.header.security_header_type,
	 decoded_nas_msg.header.sequence_number,
	 decoded_nas_msg.header.message_authentication_code);

	 MM_msg * decoded_mm_msg = &decoded_nas_msg.plain.mm;
	 printf("message type:0x%x\n",decoded_mm_msg->header.message_type);
	 printf("naskey tsc:0x%x\n",decoded_mm_msg->specific_msg.authentication_request.naskeysetidentifier.tsc);
	 printf("naskey tsc:0x%x\n",decoded_mm_msg->specific_msg.authentication_request.naskeysetidentifier.naskeysetidentifier);
	 printf("abba buffer:0x%x\n",*(unsigned char *)((decoded_mm_msg->specific_msg.authentication_request.abba)->data));
	 printf("rand buffer:0x%x\n",*(unsigned char *)((decoded_mm_msg->specific_msg.authentication_request.authenticationparameterrand)->data));
	 printf("autn buffer:0x%x\n",*(unsigned char *)((decoded_mm_msg->specific_msg.authentication_request.authenticationparameterautn)->data));
	 printf("eap message buffer:0x%x\n",*(unsigned char *)((decoded_mm_msg->specific_msg.authentication_request.eapmessage)->data));

     printf("AUTHENTICATION_REQUEST------------ end\n");
     return  0;
}
#endif

int main()
{ 
  
  auth_request();
  auth_response();
  auth_failure();
  auth_reject();
  
  return 0;
}
