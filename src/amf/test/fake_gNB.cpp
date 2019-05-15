/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under 
 * the Apache License, Version 2.0  (the "License"); you may not use this file
 * except in compliance with the License.  
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */



#include "pistache/endpoint.h"
#include "pistache/http.h"
#include "pistache/router.h"
#ifdef __linux__
#include <vector>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <syslog.h>
#include <string.h>
#endif

#include <iostream>
#include <unordered_map>

#include "amf-services.h"


////////////////////////////////////////////////// itti header /////////////////////////////////
extern "C"{
#include "sctp_primitives_client.h"
#include "intertask_interface.h"
#include "assertions.h"
#include "intertask_interface_init.h"
#include "sctp_primitives_server.h"
#include "ngap_amf.h"
#include "amf_app.h"
#include "log.h"
}
///////////////////////////////////////////////////////////////////////////////////////////////


using namespace org::openapitools::server::api;
using namespace std;
std::unordered_map<std::string,org::openapitools::server::model::UeContext> RecordUEContext;

int main(
    int argc,
    char * argv[])
{

	  int nb_gNB = 1;
	  sctp_data_t * sctp_data_p = NULL;
	  sctp_data_p = (sctp_data_t *) calloc (1, sizeof(sctp_data_t));
	  if (sctp_data_p == NULL)  exit(1);

	  char *local_ip_addr[] = {"127.0.0.1"};
	  char remote_ip_addr[] = "127.0.0.1";

	  //test SCTP
	  for (int i = 0; i < nb_gNB; i++) {
	    sctp_connect_to_remote_host (local_ip_addr, 1, remote_ip_addr, 36412, SOCK_STREAM, sctp_data_p);
	  }

	  //test NGAP
	  //test gNB

	  while (1) {
	    sleep (1);
	  }

	  return (0);

}



