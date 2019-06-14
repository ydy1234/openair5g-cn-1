#include <string.h>  
#include <stdlib.h> 

#include "nas_message.h"
#include "common_types.h"

//#include "mm_msg.h"
#include "bstrlib.h"
#include "mmData.h"
#include "common_defs.h"
#include "OCTET_STRING.h"
#include "log.h"

#define BUFFER_LEN 256
void downlink_nas_transport_with_authentication_request(uint8_t *data)
{
	
       OAILOG_DEBUG (LOG_NAS,"AUTHENTICATION_REQUEST encode dump ------------------");
	int size = NAS_MESSAGE_SECURITY_HEADER_SIZE; 
	int bytes = 0;
	  
	nas_message_t  nas_msg;
	memset (&nas_msg,		0, sizeof (nas_message_t));
	  
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
	  
	memset (&mm_msg->specific_msg.authentication_request,		0, sizeof (authentication_request_msg));
	  
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
	  
		
	  
	//construct security context
	fivegmm_security_context_t * security = calloc(1,sizeof(fivegmm_security_context_t));
	security->selected_algorithms.encryption = NAS_SECURITY_ALGORITHMS_NEA1;
	security->dl_count.overflow = 0xffff;
	security->dl_count.seq_num =  0x23;
	security->knas_enc[0] = 0x14;
	security->selected_algorithms.integrity = NAS_SECURITY_ALGORITHMS_NIA1;
	security->knas_int[0] = 0x41;
	//complete sercurity context
	  
	int length = BUFFER_LEN;
	//unsigned char data[UPLINK_BUFF_LEN] = {'\0'};
	  
	bstring  info = bfromcstralloc(length, "\0");//info the nas_message_encode result
	


	OAILOG_DEBUG (LOG_NAS,"nas header encode extended_protocol_discriminator:0x%x, security_header_type:0x%x,sequence_number:0x%x,message_authentication_code:0x%x",
	nas_msg.header.extended_protocol_discriminator,
	nas_msg.header.security_header_type,
	nas_msg.header.sequence_number,
	nas_msg.header.message_authentication_code);

	
	OAILOG_DEBUG (LOG_NAS,"message type:0x%x",mm_msg->header.message_type);
	OAILOG_DEBUG (LOG_NAS,"naskey tsc:0x%x",mm_msg->specific_msg.authentication_request.naskeysetidentifier.tsc);
	OAILOG_DEBUG (LOG_NAS,"naskey tsc:0x%x",mm_msg->specific_msg.authentication_request.naskeysetidentifier.naskeysetidentifier);
	OAILOG_DEBUG (LOG_NAS,"abba buffer:0x%x",*(unsigned char *)((mm_msg->specific_msg.authentication_request.abba)->data));
	OAILOG_DEBUG (LOG_NAS,"rand buffer:0x%x",*(unsigned char *)((mm_msg->specific_msg.authentication_request.authenticationparameterrand)->data));
	OAILOG_DEBUG (LOG_NAS,"autn buffer:0x%x",*(unsigned char *)((mm_msg->specific_msg.authentication_request.authenticationparameterautn)->data));
	OAILOG_DEBUG (LOG_NAS,"eap message buffer:0x%x",*(unsigned char *)((mm_msg->specific_msg.authentication_request.eapmessage)->data));
	//bytes = nas_message_encode (data, &nas_msg, 60/*don't know the size*/, security);
	bytes = nas_message_encode (data, &nas_msg, BUFFER_LEN/*don't know the size*/, security);

	OAILOG_DEBUG (LOG_NAS,"authentication_request encode finished !!!");
}



int amf_handle_mm_msg_registration_request(registration_request_msg * registration_request)
{
    //printf("amf_handle_mm_msg_registration_request---------- start\n");

    
    uint8_t  *data = calloc(BUFFER_LEN, sizeof(uint8_t));
	memset(data, 0, BUFFER_LEN );
	downlink_nas_transport_with_authentication_request(data);
	
	bstring nas_msg;
	nas_msg =  blk2bstr(data, BUFFER_LEN);

	amf_app_itti_send_ngap_dl_nas_transport_request(0x80, 0x90, nas_msg);
	
    return  0;
}

int downlink_nas_transport_with_authentication_result(uint8_t *data)
{
       OAILOG_DEBUG (LOG_NAS,"AUTHENTICATION_RESULT encode dump ------------------");
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
	 
   
	 bstring eapmsg = bfromcstralloc(10, "\0");
	 uint8_t bitStream_eap = 0b00110111;
	 eapmsg->data = (unsigned char *)(&bitStream_eap);
	 eapmsg->slen = 1;

     mm_msg->specific_msg.authentication_result.eapmessage = eapmsg;
	 mm_msg->specific_msg.authentication_result.presence   = 0x07;
	 mm_msg->specific_msg.authentication_result.abba       = abba;
	 
	 
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
   
	 int length = BUFFER_LEN;
	 //unsigned char data[BUF_LEN] = {'\0'};
   
	 bstring  info = bfromcstralloc(length, "\0");//info the nas_message_encode result


     
     
	 OAILOG_DEBUG (LOG_NAS,"nas header encode extended_protocol_discriminator:0x%x, security_header_type:0x%x,sequence_number:0x%x,message_authentication_code:0x%x",
	 nas_msg.header.extended_protocol_discriminator,
	 nas_msg.header.security_header_type,
	 nas_msg.header.sequence_number,
	 nas_msg.header.message_authentication_code);

	 OAILOG_DEBUG (LOG_NAS,"message type:0x%x",mm_msg->header.message_type);
	 OAILOG_DEBUG (LOG_NAS,"naskey tsc:0x%x",mm_msg->specific_msg.authentication_result.naskeysetidentifier.tsc);
	 OAILOG_DEBUG (LOG_NAS,"naskey tsc:0x%x",mm_msg->specific_msg.authentication_result.naskeysetidentifier.naskeysetidentifier);
	 OAILOG_DEBUG (LOG_NAS,"abba buffer:0x%x",*(unsigned char *)((mm_msg->specific_msg.authentication_result.abba)->data));
	 OAILOG_DEBUG (LOG_NAS,"presence:0x%x", mm_msg->specific_msg.authentication_result.presence);
	 OAILOG_DEBUG (LOG_NAS,"eap message buffer:0x%x",*(unsigned char *)((mm_msg->specific_msg.authentication_result.eapmessage)->data));
     

	 //bytes = nas_message_encode (data, &nas_msg, 60/*don't know the size*/, security);
	 bytes = nas_message_encode (data, &nas_msg, BUFFER_LEN/*don't know the size*/, security);
	 return 0;
}

int downlink_nas_transport_with_security_mode_command(uint8_t *data)
{
         OAILOG_DEBUG (LOG_NAS,"SECURITY_MODE_COMMAND encode dump----------------");
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
	 mm_msg->header.message_type = SECURITY_MODE_COMMAND;
   
	 memset (&mm_msg->specific_msg.security_mode_command,		 0, sizeof (security_mode_command_msg));


     mm_msg->specific_msg.security_mode_command.nassecurityalgorithms.typeOfCipheringAlgorithm = 0x03;
	 mm_msg->specific_msg.security_mode_command.nassecurityalgorithms.typeOfIntegrityProtectionAlgorithm = 0x04;

	 mm_msg->specific_msg.security_mode_command.naskeysetidentifier.tsc = 1;
	 mm_msg->specific_msg.security_mode_command.naskeysetidentifier.naskeysetidentifier = 0x02;
		 
	 mm_msg->specific_msg.security_mode_command.uesecuritycapability.nea = 0x05;
	 mm_msg->specific_msg.security_mode_command.uesecuritycapability.nia = 0x06;
     mm_msg->specific_msg.security_mode_command.presence = 0x1f;

	 mm_msg->specific_msg.security_mode_command.imeisvrequest = 0x09;
	 mm_msg->specific_msg.security_mode_command.epsnassecurityalgorithms.typeOfCipheringAlgoithm = 0x01;
	 mm_msg->specific_msg.security_mode_command.epsnassecurityalgorithms.typeOfIntegrityProtectionAlgoithm = 0x02; 

	 mm_msg->specific_msg.security_mode_command.additional5gsecurityinformation.hdp = 1;
	 mm_msg->specific_msg.security_mode_command.additional5gsecurityinformation.rinmr = 0;

     bstring eapmessage = bfromcstralloc(10, "\0");
     uint8_t bitStream_eapmessage = 0b00110100;
     eapmessage->data = (unsigned char *)(&bitStream_eapmessage);
     eapmessage->slen = 1; 
     
	 mm_msg->specific_msg.security_mode_command.eapmessage =  eapmessage;

	 bstring abba = bfromcstralloc(10, "\0");
     uint8_t bitStream_abba = 0b00110100;
     abba->data = (unsigned char *)(&bitStream_abba);
     abba->slen = 1; 
	 mm_msg->specific_msg.security_mode_command.abba =  abba;

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
   
	 int length = BUFFER_LEN;
	 //unsigned char data[BUFFER_LEN] = {'\0'};
   
	 bstring  info = bfromcstralloc(length, "\0");//info the nas_message_encode result
	 
	 
    
	 OAILOG_DEBUG (LOG_NAS,"nas header encode extended_protocol_discriminator:0x%x, security_header_type:0x%x,sequence_number:0x%x,message_authentication_code:0x%x",
	 nas_msg.header.extended_protocol_discriminator,
	 nas_msg.header.security_header_type,
	 nas_msg.header.sequence_number,
	 nas_msg.header.message_authentication_code);

	 OAILOG_DEBUG (LOG_NAS,"message type:0x%x",mm_msg->header.message_type);
	 OAILOG_DEBUG (LOG_NAS,"nassecurityalgorithms,typeOfCipheringAlgorithm:0x%x,typeOfIntegrityProtectionAlgorithm:0x%x",
	    mm_msg->specific_msg.security_mode_command.nassecurityalgorithms.typeOfCipheringAlgorithm,
		mm_msg->specific_msg.security_mode_command.nassecurityalgorithms.typeOfIntegrityProtectionAlgorithm );
	 OAILOG_DEBUG (LOG_NAS,"naskeysetidentifier,tsc:0x%x,naskeysetidentifier:0x%x",
		mm_msg->specific_msg.security_mode_command.naskeysetidentifier.tsc,
		mm_msg->specific_msg.security_mode_command.naskeysetidentifier.naskeysetidentifier);
	 OAILOG_DEBUG (LOG_NAS,"uesecuritycapability.nea:0x%x,nia:0x%x",		
		mm_msg->specific_msg.security_mode_command.uesecuritycapability.nea,
		mm_msg->specific_msg.security_mode_command.uesecuritycapability.nia);

	 OAILOG_DEBUG (LOG_NAS,"presence:0x%x",mm_msg->specific_msg.security_mode_command.presence);
	 
	 OAILOG_DEBUG (LOG_NAS,"imeisvrequest:0x%x",mm_msg->specific_msg.security_mode_command.imeisvrequest);

	 OAILOG_DEBUG (LOG_NAS,"epsnassecurityalgorithms, typeOfCipheringAlgoithm:0x%x,typeOfIntegrityProtectionAlgoithm:0x%x",
		mm_msg->specific_msg.security_mode_command.epsnassecurityalgorithms.typeOfCipheringAlgoithm,
		mm_msg->specific_msg.security_mode_command.epsnassecurityalgorithms.typeOfIntegrityProtectionAlgoithm); 

	  OAILOG_DEBUG (LOG_NAS,"additional5gsecurityinformation,hdp:0x%x,rinmr:0x%x",
		mm_msg->specific_msg.security_mode_command.additional5gsecurityinformation.hdp,
		mm_msg->specific_msg.security_mode_command.additional5gsecurityinformation.rinmr);

        OAILOG_DEBUG (LOG_NAS,"eap message buffer:0x%x",*(unsigned char *)((mm_msg->specific_msg.security_mode_command.eapmessage)->data));
	 OAILOG_DEBUG (LOG_NAS,"abba buffer:0x%x",*(unsigned char *)((mm_msg->specific_msg.security_mode_command.abba)->data));
	 //bytes = nas_message_encode (data, &nas_msg, 60/*don't know the size*/, security);
	 bytes = nas_message_encode (data, &nas_msg, BUFFER_LEN/*don't know the size*/, security);

     return  0;
}


int amf_handle_mm_msg_authentication_response(authentication_response_msg * authentication_response)
{
    //printf(" amf_handle_mm_msg_authentication_response-------------start\n");
    
    //authentication result
    uint8_t  *data = calloc(BUFFER_LEN, sizeof(uint8_t));
	memset(data, 0, BUFFER_LEN );
	downlink_nas_transport_with_authentication_result(data);
	
	bstring nas_msg;
	nas_msg =  blk2bstr(data, BUFFER_LEN);
	
    amf_app_itti_send_ngap_dl_nas_transport_request(0x80, 0x90, nas_msg);

	
	sleep(3);
   
    //security mode command
    memset(data, 0, BUFFER_LEN );
    downlink_nas_transport_with_security_mode_command(data);
	bstring nas_msg1;
	nas_msg1 =  blk2bstr(data, BUFFER_LEN);
	
    amf_app_itti_send_ngap_dl_nas_transport_request(0x80, 0x90, nas_msg1);
	
    return  0;
}

int amf_handle_mm_msg_security_mode_complete(security_mode_complete_msg *security_mode_complete)
{
    //printf("amf_handle_mm_msg_security_mode_complete---------- start\n");
    
    return 0;
}
int amf_handle_mm_msg_security_mode_reject(security_mode_reject_msg *security_mode_reject)
{
    //printf("amf_handle_mm_msg_security_mode_reject---------- start\n");
    
    return 0;
}

int decode_registration_request_dump(nas_message_t decoded_nas_msg)
{

        OAILOG_DEBUG (LOG_NAS,"decode registration_request dump-----------------");
	OAILOG_DEBUG (LOG_NAS,"nas header  decode extended_protocol_discriminator:0x%x, security_header_type:0x%x,sequence_number:0x%x,message_authentication_code:0x%x",
			decoded_nas_msg.header.extended_protocol_discriminator,
			decoded_nas_msg.header.security_header_type,
			decoded_nas_msg.header.sequence_number,
			decoded_nas_msg.header.message_authentication_code);

	MM_msg * decoded_mm_msg = &decoded_nas_msg.plain.mm;
	OAILOG_DEBUG (LOG_NAS,"message type:0x%x",decoded_mm_msg->header.message_type);
	OAILOG_DEBUG (LOG_NAS,"naskey tsc:0x%x",decoded_mm_msg->specific_msg.authentication_request.naskeysetidentifier.tsc);
	OAILOG_DEBUG (LOG_NAS,"naskey tsc:0x%x",decoded_mm_msg->specific_msg.authentication_request.naskeysetidentifier.naskeysetidentifier);
	OAILOG_DEBUG (LOG_NAS,"abba buffer:0x%x",*(unsigned char *)((decoded_mm_msg->specific_msg.authentication_request.abba)->data));
	OAILOG_DEBUG (LOG_NAS,"rand buffer:0x%x",*(unsigned char *)((decoded_mm_msg->specific_msg.authentication_request.authenticationparameterrand)->data));
	OAILOG_DEBUG (LOG_NAS,"autn buffer:0x%x",*(unsigned char *)((decoded_mm_msg->specific_msg.authentication_request.authenticationparameterautn)->data));
	OAILOG_DEBUG (LOG_NAS,"eap message buffer:0x%x",*(unsigned char *)((decoded_mm_msg->specific_msg.authentication_request.eapmessage)->data));


	return 0;
}
int  decode_authentication_response_dump(nas_message_t   decoded_nas_msg)
{

         OAILOG_DEBUG (LOG_NAS,"decode authentication_response dump-----------------");
         OAILOG_DEBUG (LOG_NAS,"nas header  decode extended_protocol_discriminator:0x%x,security_header_type:0x%x,sequence_number:0x%x,message_authentication_code:0x%x",
	 decoded_nas_msg.header.extended_protocol_discriminator,
	 decoded_nas_msg.header.security_header_type,
	 decoded_nas_msg.header.sequence_number,
	 decoded_nas_msg.header.message_authentication_code);

	 MM_msg * decoded_mm_msg = &decoded_nas_msg.plain.mm;
	 
	 OAILOG_DEBUG (LOG_NAS,"message type:0x%x",decoded_mm_msg->header.message_type);
	 OAILOG_DEBUG (LOG_NAS,"presence:0x%x",decoded_mm_msg->specific_msg.authentication_response.presence);
	 OAILOG_DEBUG (LOG_NAS,"param:0x%x",*(unsigned char *)((decoded_mm_msg->specific_msg.authentication_response.authenticationresponseparameter)->data));
	 OAILOG_DEBUG (LOG_NAS,"eap message buffer:0x%x",*(unsigned char *)((decoded_mm_msg->specific_msg.authentication_response.eapmessage)->data));

 
         return  0;
}


int  decode_security_mode_complete_dump(nas_message_t  decoded_nas_msg)
{
        OAILOG_DEBUG (LOG_NAS,"decode security_mode_complete dump-----------------");
	OAILOG_DEBUG (LOG_NAS,"nas header  decode extended_protocol_discriminator:0x%x, security_header_type:0x%x,sequence_number:0x%x,message_authentication_code:0x%x",
			decoded_nas_msg.header.extended_protocol_discriminator,
			decoded_nas_msg.header.security_header_type,
			decoded_nas_msg.header.sequence_number,
			decoded_nas_msg.header.message_authentication_code);

	MM_msg * mm_msg = &decoded_nas_msg.plain.mm;
	OAILOG_DEBUG (LOG_NAS,"message type:0x%x", mm_msg->header.message_type);


	OAILOG_DEBUG (LOG_NAS,"presence:0x%x",mm_msg->specific_msg.security_mode_complete.presence);
	OAILOG_DEBUG (LOG_NAS,"nasmessagecontainer:0x%x",*(unsigned char *)((mm_msg->specific_msg.security_mode_complete.nasmessagecontainer)->data));

        
	return  0;
}
int amf_handle_nas_mm_message(nas_message_t * nas_msg, tai_t tai, cgi_t cgi, nas_message_decode_status_t * decode_status)
{
  MM_msg *mmMsg = &(nas_msg->plain.mm);
  switch(mmMsg->header.message_type)
  {
    case REGISTRATION_REQUEST:
	{
               //decode_registration_request_dump(*nas_msg);
               amf_handle_mm_msg_registration_request(&mmMsg->specific_msg.registration_request);
    }
    break; 
	case AUTHENTICATION_RESPONSE:
	{
                //decode_authentication_response_dump(*nas_msg);
		amf_handle_mm_msg_authentication_response(&mmMsg->specific_msg.authentication_response);
	}
	break;
	case SECURITY_MODE_COMPLETE:
	{
                //decode_security_mode_complete_dump(*nas_msg);
		amf_handle_mm_msg_security_mode_complete(&mmMsg->specific_msg.security_mode_complete);
	}
	break;
	case SECURITY_MODE_REJECT:
	{
		amf_handle_mm_msg_security_mode_reject(&mmMsg->specific_msg.security_mode_reject);
	}
	break;
	default:
		//printf("mm unknown msg type:%d\n", mmMsg->header.message_type);
	break;
  }
}
