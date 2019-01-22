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


#include "Arp.h"

namespace org {
namespace openapitools {
namespace server {
namespace model {

Arp::Arp()
{
    m_PriorityLevel = 0;
    
}

Arp::~Arp()
{
}

void Arp::validate()
{
    // TODO: implement validation
}

nlohmann::json Arp::toJson() const
{
    nlohmann::json val = nlohmann::json::object();

    val["priorityLevel"] = m_PriorityLevel;
    val["preemptCap"] = ModelBase::toJson(m_PreemptCap);
    val["preemptVuln"] = ModelBase::toJson(m_PreemptVuln);
    

    return val;
}

void Arp::fromJson(const nlohmann::json& val)
{
    setPriorityLevel(val.at("priorityLevel"));
    
}


int32_t Arp::getPriorityLevel() const
{
    return m_PriorityLevel;
}
void Arp::setPriorityLevel(int32_t const value)
{
    m_PriorityLevel = value;
    
}
PreemptionCapability Arp::getPreemptCap() const
{
    return m_PreemptCap;
}
void Arp::setPreemptCap(PreemptionCapability const& value)
{
    m_PreemptCap = value;
    
}
PreemptionVulnerability Arp::getPreemptVuln() const
{
    return m_PreemptVuln;
}
void Arp::setPreemptVuln(PreemptionVulnerability const& value)
{
    m_PreemptVuln = value;
    
}

}
}
}
}

