#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common_types.h"
#include "amf_app_defs.h" 
#include "common_defs.h"

void amf_app_handle_initial_ue_message       (itti_amf_app_initial_ue_message_t * const conn_est_ind_pP){
  int i = 0;
  bstring nas_msg = conn_est_ind_pP->nas;
  printf("gnb id(%d)\n",conn_est_ind_pP->gnb_id);
  printf("assoc id(%d)\n",conn_est_ind_pP->sctp_assoc_id);
  printf("nas test in amf_app_handle_initial_ue_message\n");
  printf("nas len(%d)\n",blength(nas_msg));
  uint8_t * buffer = (uint8_t *)(bdata(nas_msg));
  for(;i<blength(nas_msg);i++)
    printf("0x%x ",buffer[i]);
  printf("\n");

#if 1
  MessageDef                             *message_p = NULL;
  //message_p = itti_alloc_new_message (TASK_AMF_APP, NAS_INITIAL_UE_MESSAGE);
  message_p = itti_alloc_new_message (TASK_AMF_APP, NGAP_DATA_IND);
  NGAP_UL_DATA_IND(message_p).nas_msg = nas_msg;
  itti_send_msg_to_task (TASK_NAS_AMF, INSTANCE_DEFAULT, message_p);
#endif

}
