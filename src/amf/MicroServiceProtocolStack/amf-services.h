#include "pistache/endpoint.h"
#include "pistache/http.h"
#include "pistache/router.h"
#ifdef __linux__
#include <vector>
#include <signal.h>
#include <unistd.h>
#endif
//Communication Services
#include "AMFStatusChangeSubscribeApiImpl.h"
#include "AMFStatusChangeSubscribeModifyApiImpl.h"
#include "AMFStatusChangeUnSubscribeApiImpl.h"
#include "CreateUEContxtApiImpl.h"
#include "EBIAssignmentApiImpl.h"
#include "N1N2MessageSubscribeApiImpl.h"
#include "N1N2MessageTransferApiImpl.h"
#include "N1N2MessageUnSubscribeApiImpl.h"
#include "NonUEN2InfoSubscribeApiImpl.h"
#include "NonUEN2InfoUnSubscribeApiImpl.h"
#include "NonUEN2MessageTransferApiImpl.h"
#include "ReleaseUEContxtApiImpl.h"
#include "UEContextTransferApiImpl.h"
//Event Exposure Services
#include "CreateSubscriptionApiImpl.h"
#include "DeleteSubscriptionApiImpl.h"
#include "ModifySubscriptionApiImpl.h"
//Location Service
#include "ProvideLocationApiImpl.h"
//MT Service
#include "EnableUEReachabilityApiImpl.h"
#include "ProvideDomainSelectionInfoApiImpl.h"

using namespace org::openapitools::server::api;
using namespace org::openapitools::server::model;

class AMFServicesManager {
public:
	AMFServicesManager(Pistache::Address address) : m_httpEndpoint(std::make_shared<Pistache::Http::Endpoint>(address))  {
		m_router = std::make_shared<Pistache::Rest::Router>();
		//Communication Services
		m_AMFStatusChangeSubscribeApiImpl = std::make_shared<AMFStatusChangeSubscribeApiImpl> (m_router);
		m_AMFStatusChangeSubscribeModifyApiImpl = std::make_shared<AMFStatusChangeSubscribeModifyApiImpl> (m_router);
		m_AMFStatusChangeUnSubscribeApiImpl = std::make_shared<AMFStatusChangeUnSubscribeApiImpl> (m_router);
		m_CreateUEContxtApiImpl = std::make_shared<CreateUEContxtApiImpl> (m_router);
		m_EBIAssignmentApiImpl = std::make_shared<EBIAssignmentApiImpl> (m_router);
		m_N1N2MessageSubscribeApiImpl = std::make_shared<N1N2MessageSubscribeApiImpl> (m_router);
		m_N1N2MessageTransferApiImpl = std::make_shared<N1N2MessageTransferApiImpl> (m_router);
		m_N1N2MessageUnSubscribeApiImpl = std::make_shared<N1N2MessageUnSubscribeApiImpl> (m_router);
		m_NonUEN2InfoSubscribeApiImpl = std::make_shared<NonUEN2InfoSubscribeApiImpl> (m_router);
		m_NonUEN2InfoUnSubscribeApiImpl = std::make_shared<NonUEN2InfoUnSubscribeApiImpl> (m_router);
		m_NonUEN2MessageTransferApiImpl = std::make_shared<NonUEN2MessageTransferApiImpl> (m_router);
		m_ReleaseUEContxtApiImpl = std::make_shared<ReleaseUEContxtApiImpl> (m_router);
		m_UEContextTransferApiImpl = std::make_shared<UEContextTransferApiImpl> (m_router);
        //Event Exposure Services
		//m_CreateSubscriptionApiImpl = std::make_shared<CreateSubscriptionApiImpl> (m_router);
		//m_DeleteSubscriptionApiImpl = std::make_shared<DeleteSubscriptionApiImpl> (m_router);
		//m_ModifySubscriptionApiImpl = std::make_shared<ModifySubscriptionApiImpl> (m_router);
		//Location Service
		m_ProvideLocationApiImpl = std::make_shared<ProvideLocationApiImpl> (m_router);
	    //MT Service
	    m_EnableUEReachabilityApiImpl = std::make_shared<EnableUEReachabilityApiImpl> (m_router);
	    m_ProvideDomainSelectionInfoApiImpl = std::make_shared<ProvideDomainSelectionInfoApiImpl> (m_router);
	}
	void init(size_t thr = 1);
	void start();
	void shutdown();

private:
	//Communication Services
	std::shared_ptr<Pistache::Http::Endpoint> m_httpEndpoint;
	std::shared_ptr<Pistache::Rest::Router> m_router;
	std::shared_ptr<AMFStatusChangeSubscribeApiImpl> m_AMFStatusChangeSubscribeApiImpl;
	std::shared_ptr<AMFStatusChangeSubscribeModifyApiImpl> m_AMFStatusChangeSubscribeModifyApiImpl;
	std::shared_ptr<AMFStatusChangeUnSubscribeApiImpl> m_AMFStatusChangeUnSubscribeApiImpl;
    std::shared_ptr<CreateUEContxtApiImpl> m_CreateUEContxtApiImpl;
    std::shared_ptr<EBIAssignmentApiImpl> m_EBIAssignmentApiImpl;
    std::shared_ptr<N1N2MessageSubscribeApiImpl> m_N1N2MessageSubscribeApiImpl;
	std::shared_ptr<N1N2MessageTransferApiImpl> m_N1N2MessageTransferApiImpl;
	std::shared_ptr<N1N2MessageUnSubscribeApiImpl> m_N1N2MessageUnSubscribeApiImpl;
	std::shared_ptr<NonUEN2InfoSubscribeApiImpl> m_NonUEN2InfoSubscribeApiImpl;
	std::shared_ptr<NonUEN2InfoUnSubscribeApiImpl> m_NonUEN2InfoUnSubscribeApiImpl;
	std::shared_ptr<NonUEN2MessageTransferApiImpl> m_NonUEN2MessageTransferApiImpl;
	std::shared_ptr<ReleaseUEContxtApiImpl> m_ReleaseUEContxtApiImpl;
	std::shared_ptr<UEContextTransferApiImpl> m_UEContextTransferApiImpl;
    //Event Exposure Services
    //std::shared_ptr<CreateSubscriptionApiImpl>  m_CreateSubscriptionApiImpl;
    //std::shared_ptr<DeleteSubscriptionApiImpl> m_DeleteSubscriptionApiImpl;
    //std::shared_ptr<ModifySubscriptionApiImpl> m_ModifySubscriptionApiImpl;
    //Location Service
    std::shared_ptr<ProvideLocationApiImpl> m_ProvideLocationApiImpl;
    //MT Service
    std::shared_ptr<EnableUEReachabilityApiImpl> m_EnableUEReachabilityApiImpl;
	std::shared_ptr<ProvideDomainSelectionInfoApiImpl> m_ProvideDomainSelectionInfoApiImpl;



};


