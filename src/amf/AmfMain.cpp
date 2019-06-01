/**
* AMF Communicaiton Service
* AMF Communication Service
*
* OpenAPI spec version: 1.R15.0.0
*
*
* NOTE: This class is auto generated by OpenAPI Generator (https://openapi-generator.tech).
* https://openapi-generator.tech
* Do not edit the class manually.
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

//#include "amf-services.h"

////////////////////////////////////////////////// itti header /////////////////////////////////
extern "C"{
#include "intertask_interface.h"
#include "assertions.h"
#include "intertask_interface_init.h"
#include "sctp_primitives_server.h"
#include "ngap_amf.h"
#include "amf_app.h"
#include "log.h"
#include "amf_config.h"
//#include "nas_mm.h"
}
///////////////////////////////////////////////////////////////////////////////////////////////


/*
#define PISTACHE_SERVER_THREADS 2

static Pistache::Http::Endpoint *httpEndpoint;
#ifdef __linux__
static void sigHandler(int sig){
    switch(sig){
        case SIGINT:
            std::cout<<"Received signal SIGINT\n";
            httpEndpoint->shutdown();
            exit(1);
        case SIGQUIT:
        case SIGTERM:
        case SIGHUP:
        default:
        	httpEndpoint->shutdown();
            break;
    }
    exit(0);
}

static void setUpUnixSignals(std::vector<int> quitSignals) {
    sigset_t blocking_mask;
    sigemptyset(&blocking_mask);
    for (auto sig : quitSignals)
        sigaddset(&blocking_mask, sig);

    struct sigaction sa;
    sa.sa_handler = sigHandler;
    sa.sa_mask    = blocking_mask;
    sa.sa_flags   = 0;

    for (auto sig : quitSignals)
        sigaction(sig, &sa, nullptr);
}
#endif


*/
/*
void * demo_receiver_thread(__attribute__((unused)) void *args)
{
        itti_mark_task_ready (TASK_DEMO_RECEIVER);
        while (1) {
                cout<<" itti in receiving message----\n"<<endl;
                MessageDef *received_message_p = NULL;
                // void itti_receiver_msg(task_id_t task_id, MessageDef **rx_msg);
                itti_receive_msg (TASK_DEMO_RECEIVER, &received_message_p);
                DevAssert (received_message_p != NULL);
                // #define ITTI_MSG_ID(mSGpTR) ((mSGpTR)->ittiMsgHeader.messageId)
                switch (ITTI_MSG_ID (received_message_p)) {
                        case MESSAGE_DEMO:
                                cout<<"TASK_DEMO_RECEIVER received a MESSAGE_DEMO\n This demo will exit in 3 seconds.\n"<<endl;
                                break;
                        default:
                                cout<<"TASK_DEMO_RECEIVER received a unknown message\n"<<endl;
                }
                itti_free (ITTI_MSG_ORIGIN_ID (received_message_p), received_message_p);
                received_message_p = NULL;
        }
}
*/

/*
using namespace org::openapitools::server::api;
using namespace std;
std::unordered_map<std::string,org::openapitools::server::model::UeContext> RecordUEContext;
*/
int main(
    int argc,
    char * argv[])
{
/*
    char * pid_dir;
    char * pid_file_name;
    pid_file_name = get_exe_absolute_path("/var/run");
*/
    CHECK_INIT_RETURN (OAILOG_INIT (MAX_LOG_ENV, OAILOG_LEVEL_DEBUG, MAX_LOG_PROTOS));
    CHECK_INIT_RETURN (amf_config_parse_opt_line (argc,argv,&amf_config));
    CHECK_INIT_RETURN (itti_init (TASK_MAX, THREAD_MAX, MESSAGES_ID_MAX, tasks_info, messages_info,NULL,NULL));
    //CHECK_INIT_RETURN (nas_mm_init());
    //nas_mm_init();
    CHECK_INIT_RETURN (sctp_init(&amf_config));
    CHECK_INIT_RETURN (ngap_amf_init());
    //CHECK_INIT_RETURN (amf_app_init());
    OAILOG_DEBUG(LOG_NGAP,"NGAP\n");
    OAILOG_INFO(LOG_AMF_APP,"AMF-APP\n");


    //TTN (16/05/2019) first activate NGAP->SCTP

    //MessageDef                             *message_p;
    //Activate NGAP and wait for connection from gNB
    //message_p = itti_alloc_new_message (TASK_AMF_MAIN, ACTIVATE_MESSAGE);
    //itti_send_msg_to_task (TASK_NGAP, INSTANCE_DEFAULT, message_p);


/*
    MessageDef                             *message_p = NULL;

  message_p = itti_alloc_new_message (TASK_NGAP, SCTP_INIT_MSG);
  message_p->ittiMsg.sctpInit.port = 36412;
  message_p->ittiMsg.sctpInit.ppid = 60;
  message_p->ittiMsg.sctpInit.ipv4 = 1;
  message_p->ittiMsg.sctpInit.ipv6 = 0;
  message_p->ittiMsg.sctpInit.nb_ipv4_addr = 1;
  message_p->ittiMsg.sctpInit.ipv4_address[0] =117506058;
  message_p->ittiMsg.sctpInit.nb_ipv6_addr = 0;
  message_p->ittiMsg.sctpInit.ipv6_address[0] = "0:0:0:0:0:0:0:1";
  int ret = itti_send_msg_to_task (TASK_SCTP, INSTANCE_DEFAULT, message_p);
*/
/*

         CHECK_INIT_RETURN (itti_init (TASK_MAX, THREAD_MAX, MESSAGES_ID_MAX, tasks_info, messages_info,
#if ENABLE_ITTI_ANALYZER
                                           messages_definition_xml,
#else
                                                     NULL,
#endif
                                                               NULL));

        // int itti_create_task(task_id_t task_id, void *(*function)(void *), void *args_p);
        if (itti_create_task (TASK_DEMO_RECEIVER, &demo_receiver_thread, NULL) < 0) {
          cout<<"Error while creating TASK_DEMO_RECEIVER task\n"<<endl;
          return -1;
        }

        // MessageDef *itti_alloc_new_message(task_id_t origin_tid, MessagesIds msg_id);
        MessageDef *message_p = itti_alloc_new_message(TASK_DEMO_SENDER, MESSAGE_DEMO);
        if (message_p) {
                //use the macro which is defined in the demo_messages_types.h
                DEMO_DATA_IND (message_p).name = "zgw";
                DEMO_DATA_IND (message_p).university = "SYSU";
                //int itti_send_msg_to_task(task_id_t dest_tid, instance_t inst, MessageDef *mesg);
                int send_res = itti_send_msg_to_task(TASK_DEMO_RECEIVER, INSTANCE_DEFAULT, message_p);
        }
    cout<<"init itti success"<<endl;
*/
/*


#ifdef __linux__
    std::vector<int> sigs{SIGQUIT, SIGINT, SIGTERM, SIGHUP};
    setUpUnixSignals(sigs);
#endif
*/

    //launch AMF services
/*
    Pistache::Address amf_service_addr(Pistache::Ipv4::any(), Pistache::Port(5001));
    AMFServicesManager amfServicesManager(amf_service_addr);
    amfServicesManager.init(2);
    amfServicesManager.start();
    amfServicesManager.shutdown();
*/
    //std::thread amf_services_manager_app(&AMFServicesManager::start, amfServicesManager);

    itti_wait_tasks_end();
    return 0;
}

