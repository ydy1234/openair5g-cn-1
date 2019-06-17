#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/epoll.h>


#include "Ngap_ProtocolIE-Field.h"
#include "Ngap_NGAP-PDU.h"
#include "Ngap_InitiatingMessage.h"
#if 1
#include "Ngap_BroadcastPLMNItem.h"
#include "Ngap_GNB-ID.h"
#include "Ngap_GlobalGNB-ID.h"
#include "Ngap_PagingDRX.h"
#include "Ngap_SliceSupportItem.h"
#include "Ngap_SupportedTAItem.h"
#include "Ngap_UnsuccessfulOutcome.h"
#endif

#include "bstrlib.h"
#include "intertask_interface_types.h"
#include "ngap_amf_encoder.h"
#include "sctp_primitives_client.h"
#include "intertask_interface.h"
#include "assertions.h"
#include "intertask_interface_init.h"
#include "sctp_primitives_server.h"
#include "Ngap_CriticalityDiagnostics-IE-Item.h"
#include "Ngap_CriticalityDiagnostics-IE-List.h"
#include "Ngap_ProtocolIE-Field.h"
#include "Ngap_UserLocationInformationEUTRA.h"




#include "ngap_amf.h"
#include "amf_app.h"
#include "log.h"
#include "ngap_common.h"
#include "sctp_gNB_defs.h"


#include "sctp_gNB_defs.h"

#include "nas_message.h"
#include "sctp_primitives_server.h"


Ngap_SupportedTAList_t       g_SupportedTAList;
Ngap_GlobalRANNodeIDList_t   g_glocalRANNodeIDList;

int g_supportTaListCount = 0;

void mccmnc_to_pLMN_buf(char buf[3], const char *mcc, const char *mnc) {
    int i;

    char _mcc[3];
    for (i=0; i<3; i++)
      _mcc[i] = mcc[i] - '0';

    char _mnc[3];
    if (strlen(mnc) == 2) {
        _mnc[0] = mnc[0];
        _mnc[1] = mnc[1];
        _mnc[2] = '0' + 15;
    } else {
        for (i=0; i<3; i++)
            _mnc[i] = mnc[i];
    }
    for (i=0; i<3; i++)
      _mnc[i] = _mnc[i] - '0';

    buf[0]  = _mcc[1] << 4;
    buf[0] |= _mcc[0];
    buf[1]  = _mnc[2] << 4;
    buf[1] |= _mcc[2];
    buf[2]  = _mnc[1] << 4;
    buf[2] |= _mnc[0];
}

void fill_pLMNIdentity(void *dest, const char *mcc, const char *mnc) {
    char buf[3];
    mccmnc_to_pLMN_buf(buf, mcc, mnc);
	OCTET_STRING_fromBuf(dest, buf, 3);
}

void fill_gNB_ID(Ngap_GNB_ID_t *gnb_id, unsigned int index) {
    uint32_t gNB_ID = htonl(index);
    gnb_id->present = Ngap_GNB_ID_PR_gNB_ID;
	gnb_id->choice.gNB_ID.buf = calloc(4, sizeof(uint8_t));
	gnb_id->choice.gNB_ID.size = 4;
	memcpy(gnb_id->choice.gNB_ID.buf, &gNB_ID, 4);
}

void fill_tAC(Ngap_TAC_t *item, unsigned int tac) {
    uint32_t tAC = htonl(tac);
    const char *ptr = (void *)&tAC;
    OCTET_STRING_fromBuf(item, ptr + 1, 3);
	OAILOG_DEBUG (LOG_NGAP,"tAC:0x%x,0x%x,0x%x",item->buf[0],item->buf[1],item->buf[2]);
}

void fill_GlobalRANNodeID_with_GlobalGNBID(Ngap_GlobalRANNodeID_t *ngap_GlobalRANNodeID) {
	ngap_GlobalRANNodeID->present = Ngap_GlobalRANNodeID_PR_globalGNB_ID;
    Ngap_GlobalGNB_ID_t *globalGNB_ID = calloc(1, sizeof(struct Ngap_GlobalGNB_ID));
	ngap_GlobalRANNodeID->choice.globalGNB_ID = globalGNB_ID;

	fill_pLMNIdentity(&globalGNB_ID->pLMNIdentity, "208", "89");
    fill_gNB_ID(&globalGNB_ID->gNB_ID, 513);
    //asn_fprint(stderr, &asn_DEF_Ngap_GlobalRANNodeID, ngap_GlobalRANNodeID);

	OAILOG_DEBUG (LOG_NGAP,"pLMNIdentity:0x%x,0x%x,0x%x",globalGNB_ID->pLMNIdentity.buf[0],globalGNB_ID->pLMNIdentity.buf[1],globalGNB_ID->pLMNIdentity.buf[2]);
	OAILOG_DEBUG (LOG_NGAP,"gNB_ID:0x%x,0x%x,0x%x,0x%x",
	globalGNB_ID->gNB_ID.choice.gNB_ID.buf[0],globalGNB_ID->gNB_ID.choice.gNB_ID.buf[1],
	globalGNB_ID->gNB_ID.choice.gNB_ID.buf[2],globalGNB_ID->gNB_ID.choice.gNB_ID.buf[3]);
}
typedef struct {
    int sst;
    int sd;
} snssai_t;

Ngap_SliceSupportItem_t *make_sliceSupportItem(const snssai_t slice) {
    Ngap_SliceSupportItem_t *item;
    item = calloc (1, sizeof(Ngap_SliceSupportItem_t));
    const char sst = slice.sst;
    OCTET_STRING_fromBuf(&item->s_NSSAI.sST, &sst, 1);
	OAILOG_DEBUG (LOG_NGAP,"s_NSSAI.sST:0x%x",item->s_NSSAI.sST.buf[0]);
    if (slice.sd >= 0 )
    {
        uint32_t sd = ntohl(slice.sd);
        const char *sd_ptr = (const char *)&sd + 1;
        Ngap_SD_t *sD = calloc(1, sizeof(Ngap_SD_t));
        item->s_NSSAI.sD = sD;
		
        OCTET_STRING_fromBuf(sD, sd_ptr, 3);
		OAILOG_DEBUG (LOG_NGAP,"s_NSSAI.sD:0x%x,0x%x,0x%x",item->s_NSSAI.sD->buf[0],item->s_NSSAI.sD->buf[1],item->s_NSSAI.sD->buf[2]);
    }
    return item;
}


Ngap_BroadcastPLMNItem_t *make_Ngap_BroadcastPLMNItem(const char *mcc, const char *mnc,
                                                      const snssai_t slice_list[], const int slice_list_len) {
    Ngap_BroadcastPLMNItem_t *item;
    item = calloc (1, sizeof(Ngap_BroadcastPLMNItem_t));
	fill_pLMNIdentity(&item->pLMNIdentity, mcc, mnc);
	OAILOG_DEBUG (LOG_NGAP,"pLMNIdentity:0x%x,0x%x,0x%x",item->pLMNIdentity.buf[0],item->pLMNIdentity.buf[1],item->pLMNIdentity.buf[2]);
    Ngap_SliceSupportItem_t *sliceSupportItem;
    int i;
    for (i=0; i<slice_list_len; i++) {
        sliceSupportItem = make_sliceSupportItem(slice_list[i]);
        ASN_SEQUENCE_ADD (&item->tAISliceSupportList.list, sliceSupportItem);
    }
    return item;
}

void fill_broadcastPLMNList_1(Ngap_BroadcastPLMNList_t *list) {
    Ngap_BroadcastPLMNItem_t *item;
    snssai_t slice_list[1] = { { 1, 100 } };
    item = make_Ngap_BroadcastPLMNItem("208", "89", slice_list, 100);
    ASN_SEQUENCE_ADD(list, item);

    snssai_t slice_list2[2] = { { 100, 100 }, { 4, 5000} };
    item = make_Ngap_BroadcastPLMNItem("208", "93", slice_list2, 2);
    ASN_SEQUENCE_ADD(list, item);
}

void fill_broadcastPLMNList_2(Ngap_BroadcastPLMNList_t *list) {
    Ngap_BroadcastPLMNItem_t *item;
    snssai_t slice_list[3] = { { 1, 100 }, { 2, 100 }, { 3, 100 } };
    item = make_Ngap_BroadcastPLMNItem("208", "89", slice_list, 3);
    ASN_SEQUENCE_ADD(list, item);
}

// IEs

Ngap_NGSetupRequestIEs_t *make_GlobalRANNodeID_ie() {
	Ngap_NGSetupRequestIEs_t *ie;
	ie = calloc(1, sizeof(Ngap_NGSetupRequestIEs_t));

	ie->id = Ngap_ProtocolIE_ID_id_GlobalRANNodeID;
	ie->criticality = Ngap_Criticality_reject;
	ie->value.present = Ngap_NGSetupRequestIEs__value_PR_GlobalRANNodeID;

    fill_GlobalRANNodeID_with_GlobalGNBID(&ie->value.choice.GlobalRANNodeID);
    //asn_fprint(stderr, &asn_DEF_Ngap_NGSetupRequestIEs, ie);

    return ie;
}

Ngap_NGSetupRequestIEs_t *make_RANNodeName_ie(const char *name) {
	Ngap_NGSetupRequestIEs_t *ie;
	ie = calloc(1, sizeof(Ngap_NGSetupRequestIEs_t));

   	ie->id = Ngap_ProtocolIE_ID_id_RANNodeName;
	ie->criticality = Ngap_Criticality_reject;
	ie->value.present = Ngap_NGSetupRequestIEs__value_PR_RANNodeName;
	OCTET_STRING_fromBuf (&ie->value.choice.RANNodeName, name, strlen (name));
	OAILOG_DEBUG (LOG_NGAP,"RANNodeName:%s", name);
    return ie;
}

Ngap_NGSetupRequestIEs_t *make_DefaultPagingDRX_ie(e_Ngap_PagingDRX drx ) {
	Ngap_NGSetupRequestIEs_t *ie;
	ie = calloc(1, sizeof(Ngap_NGSetupRequestIEs_t));

	ie->id =Ngap_ProtocolIE_ID_id_DefaultPagingDRX;
	ie->criticality = Ngap_Criticality_reject;
	ie->value.present = Ngap_NGSetupRequestIEs__value_PR_PagingDRX;
	ie->value.choice.PagingDRX = drx;
	
	OAILOG_DEBUG (LOG_NGAP,"PagingDRX:0x%x",ie->value.choice.PagingDRX);
    return ie;
}

Ngap_SupportedTAItem_t *make_SupportedTaItem_1() {
    Ngap_SupportedTAItem_t *item;
    item = (Ngap_SupportedTAItem_t *)calloc(1, sizeof(Ngap_SupportedTAItem_t));
    fill_tAC(&item->tAC, 257);
    fill_broadcastPLMNList_1(&item->broadcastPLMNList);
    return item;
}

Ngap_SupportedTAItem_t *make_SupportedTaItem_2() {
    Ngap_SupportedTAItem_t *item;
    item = (Ngap_SupportedTAItem_t *)calloc(1, sizeof(Ngap_SupportedTAItem_t));
    fill_tAC(&item->tAC, 65537);
    fill_broadcastPLMNList_2(&item->broadcastPLMNList);
    return item;
}

Ngap_NGSetupRequestIEs_t *make_supportedTAList(void) {
	Ngap_NGSetupRequestIEs_t *ie = NULL;
    ie = calloc(1, sizeof(Ngap_NGSetupRequestIEs_t));
    ie->id =Ngap_ProtocolIE_ID_id_SupportedTAList;
    ie->criticality = Ngap_Criticality_reject;
    ie->value.present = Ngap_NGSetupRequestIEs__value_PR_SupportedTAList;

    Ngap_SupportedTAItem_t *ta;

    ta = make_SupportedTaItem_1();
    ASN_SEQUENCE_ADD(&ie->value.choice.SupportedTAList.list, ta);

    ta = make_SupportedTaItem_2();
    ASN_SEQUENCE_ADD(&ie->value.choice.SupportedTAList.list, ta);

    return ie;
}

void add_NGSetupRequest_ie(Ngap_NGSetupRequest_t *ngapSetupRequest, Ngap_NGSetupRequestIEs_t *ie) {
    int ret;
	ret = ASN_SEQUENCE_ADD(&ngapSetupRequest->protocolIEs.list, ie);
    if ( ret != 0 ) {
        fprintf(stderr, "Failed to add ie\n");
    }
}

// PDU funcs

void check_NGAP_pdu_constraints(Ngap_NGAP_PDU_t *pdu) {
    int ret;
    char errbuf[512];
    size_t errlen =sizeof(errbuf);
    ret = asn_check_constraints(&asn_DEF_Ngap_NGAP_PDU, pdu, errbuf, &errlen);
    if(ret != 0) {
        fprintf(stderr,"Constraintvalidationfailed:%s\n", errbuf);
    }
}
#include  "bstrlib.h"
#include  "intertask_interface_types.h"

Ngap_NGAP_PDU_t *make_NGAP_SetupRequest() {

    OAILOG_FUNC_IN (LOG_NGAP);
	OAILOG_DEBUG(LOG_NGAP,"encode ng setup request dump--------");
    // prepare PDU message with NGSetupRequest content

	Ngap_NGAP_PDU_t *pdu;
	pdu = calloc(1, sizeof(Ngap_NGAP_PDU_t));

	pdu->present = Ngap_NGAP_PDU_PR_initiatingMessage;
	pdu->choice.initiatingMessage = calloc(1, sizeof(Ngap_InitiatingMessage_t));
	pdu->choice.initiatingMessage->procedureCode = Ngap_ProcedureCode_id_NGSetup;
	pdu->choice.initiatingMessage->criticality = Ngap_Criticality_reject;
	pdu->choice.initiatingMessage->value.present = Ngap_InitiatingMessage__value_PR_NGSetupRequest;

	Ngap_NGSetupRequest_t *ngapSetupRequest;
	ngapSetupRequest = &pdu->choice.initiatingMessage->value.choice.NGSetupRequest;

    // Make NGSetupRequest IEs and add it to message
	Ngap_NGSetupRequestIEs_t *ie;
   
    ie = make_GlobalRANNodeID_ie();
    add_NGSetupRequest_ie(ngapSetupRequest, ie);

    ie = make_RANNodeName_ie("test gNB");
    add_NGSetupRequest_ie(ngapSetupRequest, ie);

    ie = make_DefaultPagingDRX_ie(Ngap_PagingDRX_v128);
    add_NGSetupRequest_ie(ngapSetupRequest, ie);

    ie = make_supportedTAList();
    add_NGSetupRequest_ie(ngapSetupRequest, ie);

    return pdu;
}

#define BASE_PORT 36400
#define CONNECT_SCTP_SERVER_PORT (BASE_PORT+12)

#define NGAP_MAX_NUM 32
#define NGAP_MAX_FD_SIZE 1024

struct epoll_event stEvents[NGAP_MAX_NUM];
struct epoll_event event;


int g_ngap_epoll_fd = 0;
int g_ngap_sctp_server_fd = -1;

int ngap_epoll_init(int fdsize)
{
    g_ngap_epoll_fd  = epoll_create(fdsize);
    return 0;
}
static void ngap_epoll_event_add(int epollfd,int fd,int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&ev);
}

static void ngap_epoll_event_del(int epollfd,int fd,int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,&ev);
}

static void ngap_epoll_event_mod(int epollfd, int fd,int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&ev);
}

sctp_data_t * ngap_connect_sctp_server( )
{
    int sd = 0;
    sctp_data_t * sctp_data_p = NULL;
	char *local_ip_addr[] = {"192.168.2.122"};
	char  remote_ip_addr[] = "192.168.2.122";
	
    sctp_data_p = (sctp_data_t *) calloc (1, sizeof(sctp_data_t));
  	if (sctp_data_p == NULL)  exit(1);
  	if(sd  = sctp_connect_to_remote_host (local_ip_addr, 1, remote_ip_addr, CONNECT_SCTP_SERVER_PORT, SOCK_STREAM, sctp_data_p) < 0)
    {
        OAILOG_ERROR(LOG_SCTP,"conn sctp server:%s,id:%d, err:%s failed\n",remote_ip_addr,errno, strerror(errno));
		free(sctp_data_p);
		sctp_data_p = NULL;
		return NULL;
	}
	
	return sctp_data_p;
}

int ngap_amf_connect_sctp_server()
{
    int sctp_server_fd = -1;
    //connect sctp server
    sctp_data_t * sctp_data_p = NULL;
	sctp_data_p = ngap_connect_sctp_server();
	if(!sctp_data_p)
	{
	    OAILOG_ERROR(LOG_SCTP,"connect sctp server port %d failed\n", CONNECT_SCTP_SERVER_PORT);
	    return -1;
	}

    sctp_server_fd  = sctp_data_p->sd;
	
	free(sctp_data_p);
	sctp_data_p = NULL;

	return  sctp_server_fd;
}


#define SCTP_RECV_BUFFER_SIZE  4096
void ngap_sctp_read_server_data(int fd)
{
    int                                     flags = 0, recvSize = 0;
    //socklen_t								from_len = 0;
    int								        from_len = 0;
    struct sctp_sndrcvinfo					sinfo = {0};
    struct sockaddr_in 					    addr = {0};
    char 								    recvBuffer[SCTP_RECV_BUFFER_SIZE] = {0};

    memset ((void *)&addr, 0, sizeof (struct sockaddr_in));
    from_len = (socklen_t) sizeof (struct sockaddr_in);
    memset ((void *)&sinfo, 0, sizeof (struct sctp_sndrcvinfo));
    recvSize = sctp_recvmsg (fd, (void *)recvBuffer, SCTP_RECV_BUFFER_SIZE, (struct sockaddr *)&addr, &from_len, &sinfo, &flags);
    
    if (recvSize <= 0)  //<= 
    {   
        if(recvSize < 0)
        {
            OAILOG_DEBUG (LOG_SCTP, "An error occured during read,recvSize:%d\n",recvSize);
            OAILOG_ERROR (LOG_SCTP, "sctp_recvmsg: %s:%d\n", strerror (errno), errno);
        }
        //continue;  
        //==0 sctp mechanism ?
        return NULL;
    }
	OAILOG_DEBUG(LOG_SCTP,"recv size:%d\n", recvSize);
    if (flags & MSG_NOTIFICATION)
    {  		    
        union sctp_notification  *snp = (union sctp_notification *)recvBuffer;
        switch (snp->sn_header.sn_type) 
		{
            case SCTP_SHUTDOWN_EVENT:
			{
               OAILOG_DEBUG (LOG_SCTP, "SCTP_SHUTDOWN_EVENT received\n");
               //return sctp_handle_com_down((sctp_assoc_id_t) snp->sn_shutdown_event.sse_assoc_id);
            }
            case SCTP_ASSOC_CHANGE: 
			{
               OAILOG_DEBUG(LOG_SCTP, "SCTP association change event received\n");
               //return handle_assoc_change(sd, ppid, &snp->sn_assoc_change);
            }
            default: 
			{
               //OAILOG_WARNING(LOG_SCTP, "Unhandled notification type %u\n", snp->sn_header.sn_type);
               break;
            }
        }
    }
    else
    {
         MessagesIds message_id = MESSAGES_ID_MAX;
         Ngap_NGAP_PDU_t decoded_pdu = {0};
         uint8_t * buffer_p = NULL;
         bstring b = blk2bstr(recvBuffer, recvSize);
          
         ngap_amf_decode_pdu(&decoded_pdu, b,  &message_id);
         ngap_amf_handle_message(0,0,&decoded_pdu);
   	     //break;
    }
    return 0;
}
void *sctp_socket_thread (void *args_p)
{
    int iRet = -1;
	int iFd  = -1;
	int i  = 0;
	
    //epoll init   
    ngap_epoll_init(NGAP_MAX_FD_SIZE);

    //sctp connect server
	g_ngap_sctp_server_fd = ngap_amf_connect_sctp_server();
	if(g_ngap_sctp_server_fd == -1)
	{
	   sleep(2);  //reconn
       g_ngap_sctp_server_fd = ngap_amf_connect_sctp_server();
	   if(g_ngap_sctp_server_fd == -1)
	   	{
	        return -1;
	   	}
	}
	
	//other add;

	//epoll add 
    ngap_epoll_event_add(g_ngap_epoll_fd, g_ngap_sctp_server_fd, EPOLLERR| EPOLLIN | EPOLLHUP);	

	while(true)
    {
        iRet = epoll_wait(g_ngap_epoll_fd, stEvents, NGAP_MAX_NUM, -1);
        if (-1 == iRet)
        {
            //sleep(1);
            usleep(100000); //100ms
            continue;
        }
        //t = time(NULL);
        for (i = 0;i < iRet; i++)
        {
            iFd = stEvents[i].data.fd;
            if (!(stEvents[i].events & EPOLLIN))
            {
                if ((stEvents[i].events & EPOLLHUP))
                {
                    epoll_ctl(g_ngap_epoll_fd, EPOLL_CTL_DEL, iFd, &event);
                    //printf(" ngap del %d from epoll .\n",  iFd);
                    close(iFd);
					//reconn sctp server, add event ?
                }
                continue;
            }
	        if(g_ngap_sctp_server_fd == iFd)
	        {
                ngap_sctp_read_server_data(iFd);
			}
        }
	}

    return NULL;
}

int  ngap_create_socket_thread(sctp_data_t *sctp_arg_p)
{
    static pthread_t         sctp_thread = 0;
    if (pthread_create (&sctp_thread, NULL, &sctp_socket_thread, (void *)sctp_arg_p) < 0) 
	{
        OAILOG_ERROR (LOG_SCTP, "pthread_create: %s:%d", strerror (errno), errno);
        return -1;
    }
    return 0;
}
#if 0
sctp_data_t * ngap_connect_sctp_server( )
{
    int sd = 0;
    sctp_data_t * sctp_data_p = NULL;
	char *local_ip_addr[] = {"10.112.43.231"};
	char  remote_ip_addr[] = "10.112.43.231";
	
    sctp_data_p = (sctp_data_t *) calloc (1, sizeof(sctp_data_t));
  	if (sctp_data_p == NULL)  exit(1);
  	if(sd  = sctp_connect_to_remote_host (local_ip_addr, 1, remote_ip_addr, CONNECT_SCTP_SERVER_PORT, SOCK_STREAM, sctp_data_p) < 0)
    {
        printf("conn sctp server:%s,id:%d, err:%s failed\n",remote_ip_addr,errno, strerror(errno));
		free(sctp_data_p);
		sctp_data_p = NULL;
		return NULL;
	}
	
	return sctp_data_p;
}
#endif
static  handler(int signo)
{
    uint8_t sig = 0 ;
    switch(signo) {
    case SIGUSR1:  //处理信号 SIGUSR1 
		sig =  1;
        break;
    case SIGUSR2: //处理信号 SIGUSR2
        sig =  2;
        break;
    default:      //本例不支持
        break;
    }

    if(sig)
    {
	    uint32_t ppid =  60;
	    Ngap_NGAP_PDU_t *pdu = NULL;
	    if(sig == 1)
	       pdu = make_NGAP_SetupRequest();
	    else if(sig == 2)
	       pdu = make_NGAP_InitialUEMessage();
	       //pdu = make_NGAP_UplinkNasTransport(UPLINK_NAS_TRANSPORT_WITH_AUTHENTICATION_RESPONSE);

       // debug
       asn_fprint(stderr, &asn_DEF_Ngap_NGAP_PDU, pdu);

       check_NGAP_pdu_constraints(pdu);
       //encode_pdu_to_aper_and_write_to_stdout(pdu);

	   //encode
       size_t buffer_size = 1000;
       void *buffer = calloc(1,buffer_size);
       asn_enc_rval_t er;

       er = aper_encode_to_buffer(&asn_DEF_Ngap_NGAP_PDU, NULL, pdu, buffer, buffer_size);
       OAILOG_DEBUG(LOG_NGAP,"sctp client send buffer(%x) length(%d)",buffer,er.encoded);
	
       ngap_sctp_send_msg(g_ngap_sctp_server_fd, 60, 0, buffer,er.encoded);

       ASN_STRUCT_FREE(asn_DEF_Ngap_NGAP_PDU, pdu);
    }
}

int main( int argc, char * argv[])
{
	//为两个信号设置信号处理函数
    signal(SIGUSR1, handler);
    signal(SIGUSR2, handler);

    
    CHECK_INIT_RETURN (OAILOG_INIT (LOG_SPGW_ENV, OAILOG_LEVEL_DEBUG, MAX_LOG_PROTOS));
	OAILOG_DEBUG(LOG_NGAP, "initial fake gnb......");
    nas_message_t  nas_msg;
    
    //fprintf(stderr, "Capture loopback with wireshark and test with 2 terminals:\n");
    //fprintf(stderr, "  terminal 1: $ socat SCTP-LISTEN:38412,reuseaddr,fork STDOUT\n");
    //fprintf(stderr, "  terminal 2: $ ./NGSetupRequest | socat STDIN SCTP-CONNECT:127.0.0.1:38412,end-close\n\n");

    ngap_create_socket_thread(NULL);
    #if 0
    sctp_data_t * sctp_data_p = NULL;
	sctp_data_p = ngap_connect_sctp_server();
	if(!sctp_data_p)
		return -1;
	//ngap_recv_from_sctp_server(sctp_data_p);
    #endif
	
	OAILOG_DEBUG(LOG_NGAP,"initial socket, wait......");
	
	sleep(5);  //init socket;
	if(g_ngap_sctp_server_fd < 0)
	{
	   OAILOG_ERROR(LOG_NGAP,"init sctp server port %d failed, exit", CONNECT_SCTP_SERVER_PORT);
       exit(0);
	}
	#if 0
	uint32_t ppid =  60;
	Ngap_NGAP_PDU_t *pdu = NULL;
	pdu = make_NGAP_SetupRequest();
	//pdu = make_NGAP_InitialUEMessage();
	//pdu = make_NGAP_UplinkNasTransport(UPLINK_NAS_TRANSPORT_WITH_AUTHENTICATION_RESPONSE);

    // debug
    asn_fprint(stderr, &asn_DEF_Ngap_NGAP_PDU, pdu);

    check_NGAP_pdu_constraints(pdu);
    //encode_pdu_to_aper_and_write_to_stdout(pdu);

	//encode
    size_t buffer_size = 1000;
    void *buffer = calloc(1,buffer_size);
    asn_enc_rval_t er;

    er = aper_encode_to_buffer(&asn_DEF_Ngap_NGAP_PDU, NULL, pdu, buffer, buffer_size);
    OAILOG_DEBUG(LOG_NGAP,"sctp client send buffer(%x) length(%d)",buffer,er.encoded);
	
    ngap_sctp_send_msg(g_ngap_sctp_server_fd, 60, 0, buffer,er.encoded);

    ASN_STRUCT_FREE(asn_DEF_Ngap_NGAP_PDU, pdu);
	#endif
    #if 0   
    pdu = make_NGAP_InitialUEMessage();

	//pdu = make_NGAP_UplinkNasTransport(UPLINK_NAS_TRANSPORT_WITH_AUTHENTICATION_RESPONSE);

    // debug
    asn_fprint(stderr, &asn_DEF_Ngap_NGAP_PDU, pdu);

    check_NGAP_pdu_constraints(pdu);
    //encode_pdu_to_aper_and_write_to_stdout(pdu);

	//encode
    //size_t buffer_size = 1000;
    //void *buffer = calloc(1,buffer_size);
    //asn_enc_rval_t er;
    memset(buffer, 0, buffer_size);

    er = aper_encode_to_buffer(&asn_DEF_Ngap_NGAP_PDU, NULL, pdu, buffer, buffer_size);
    printf("sctp client send buffer(%x) length(%d)\n",buffer,er.encoded);
	
    ngap_sctp_send_msg(g_ngap_sctp_server_fd, 60, 0, buffer,er.encoded);
    #endif
	
	while(1)
	{
        sleep(10);
	}
    
	#if 0
	
	//sctp_send_msg (sctp_data_p, 60, 0, buffer,er.encoded);
    int                                     flags = 0, n = 0;
    #define SCTP_RECV_BUFFER_SIZE  1024
    //socklen_t								from_len = 0;
    int								from_len = 0;
    struct sctp_sndrcvinfo					sinfo = {0};
    struct sockaddr_in 					    addr = {0};
    char 								    recvBuffer[SCTP_RECV_BUFFER_SIZE] = {0};
    int sd = sctp_data_p->sd;
   	while(1)
  	{
        memset ((void *)&addr, 0, sizeof (struct sockaddr_in));
        from_len = (socklen_t) sizeof (struct sockaddr_in);
        memset ((void *)&sinfo, 0, sizeof (struct sctp_sndrcvinfo));
        n = sctp_recvmsg (sd, (void *)recvBuffer, SCTP_RECV_BUFFER_SIZE, (struct sockaddr *)&addr, &from_len, &sinfo, &flags);
        printf("recv size:%d,from_len:%d\n", n,from_len);
        if (n < 0)
		{
		    
            OAILOG_DEBUG (LOG_SCTP, "An error occured during read\n");
             OAILOG_ERROR (LOG_SCTP, "sctp_recvmsg: %s:%d\n", strerror (errno), errno);
             continue;
        }
        if (flags & MSG_NOTIFICATION)
  		{  		    
            union sctp_notification                *snp = (union sctp_notification *)recvBuffer;
            switch (snp->sn_header.sn_type) 
		    {
            case SCTP_SHUTDOWN_EVENT: {
               OAILOG_DEBUG (LOG_SCTP, "SCTP_SHUTDOWN_EVENT received\n");
               //return sctp_handle_com_down((sctp_assoc_id_t) snp->sn_shutdown_event.sse_assoc_id);
             }
             case SCTP_ASSOC_CHANGE: {
               OAILOG_DEBUG(LOG_SCTP, "SCTP association change event received\n");
               //return handle_assoc_change(sd, ppid, &snp->sn_assoc_change);
             }
             default: {
               OAILOG_WARNING(LOG_SCTP, "Unhandled notification type %u\n", snp->sn_header.sn_type);
               break;
             }
             }
           }
           else
           {
          	   MessagesIds message_id = MESSAGES_ID_MAX;
               Ngap_NGAP_PDU_t decoded_pdu = {0};
               uint8_t * buffer_p = NULL;
               bstring b = blk2bstr(recvBuffer, n);
          
          	 
               printf("NGAP_SetupRequest-------------decode, length:%d\n", n);
               ngap_amf_decode_pdu(&decoded_pdu, b,  &message_id);
               ngap_amf_handle_message(0,0,&decoded_pdu);
   			   break;
           }
    }
	#endif
	
	#if 0
    //decode
	MessagesIds message_id = MESSAGES_ID_MAX;
    Ngap_NGAP_PDU_t decoded_pdu = {0};
	bstring b = blk2bstr(buffer, er.encoded);
	printf("NGAP_SetupRequest-------------decode, length:%d\n", er.encoded);
    ngap_amf_decode_pdu(&decoded_pdu, b,  &message_id);
    ngap_amf_handle_message(0,0,&decoded_pdu);
    #endif
	
    //ASN_STRUCT_FREE(asn_DEF_Ngap_NGAP_PDU, pdu);
}
