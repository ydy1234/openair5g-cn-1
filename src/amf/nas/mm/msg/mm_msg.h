#include "mmMsgDef.h"

/*
 * Structure of EMM plain NAS message
 * ----------------------------------
 */
typedef union {
  mm_msg_header_t header;
  authentication_request_msg authentication_request;//Done 19/03/2019
  authentication_response_msg authentication_response;//Done 19/03/2019
  //authentication_result_msg authentication_result;
  authentication_failure_msg authentication_failure;//Done  19/03/2019
  authentication_reject_msg authentication_reject;//Done 19/03/2019
  registration_request_msg registration_request;
  registration_accept_msg registration_accept;
  registration_complete_msg registration_complete;
  registration_reject_msg registration_reject;
  uplink_nas_transport_msg uplink_nas_transport;
  downlink_nas_transport_msg downlink_nas_transport;
  //deregistration_request_msg deregistration_request;
  service_request_msg service_request;
  service_accept_msg service_accept;
  service_reject_msg service_reject;
  configuration_update_command_msg configuration_update_command;
  configuration_update_complete_msg configuration_update_complete;
  identity_request_msg identity_request;
  identity_response_msg identity_response;
  notification_msg notification;
  notification_response_msg notification_response;
  security_mode_command_msg security_mode_command;
  security_mode_complete_msg security_mode_complete;
  security_mode_reject_msg security_mode_reject;
  fiveg_mm_status_msg fiveg_mm_status;
}MM_msg;
