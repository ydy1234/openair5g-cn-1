#include <string.h>  
#include <stdlib.h> 

#include "nas_message.h"
#include "common_types.h"

int amf_handle_mm_msg_registration_request(registration_request_msg * registration_request){
  printf("function in amf_handle_mm_msg_registration_request\n");
}


int amf_handle_nas_mm_message(nas_message_t * nas_msg, tai_t tai, cgi_t cgi, nas_message_decode_status_t * decode_status){
  MM_msg *mmMsg = &(nas_msg->plain.mm);
  switch(mmMsg->header.message_type){
    case REGISTRATION_REQUEST:
      amf_handle_mm_msg_registration_request(&mmMsg->specific_msg.registration_request);
    break; 
  }
}
