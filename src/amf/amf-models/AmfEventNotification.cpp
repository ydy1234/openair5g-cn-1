/**
* Namf_EventExposure Service
* AMF Event Exposure Service
*
* OpenAPI spec version: 1.R15.0.0
* 
*
* NOTE: This class is auto generated by OpenAPI Generator (https://openapi-generator.tech).
* https://openapi-generator.tech
* Do not edit the class manually.
*/


#include "AmfEventNotification.h"

namespace org {
namespace openapitools {
namespace server {
namespace model {

AmfEventNotification::AmfEventNotification()
{
    m_SubscriptionId = "";
    m_NotifCorelationId = "";
    m_ReportsIsSet = false;
    
}

AmfEventNotification::~AmfEventNotification()
{
}

void AmfEventNotification::validate()
{
    // TODO: implement validation
}

nlohmann::json AmfEventNotification::toJson() const
{
    nlohmann::json val = nlohmann::json::object();

    val["subscriptionId"] = ModelBase::toJson(m_SubscriptionId);
    val["notifCorelationId"] = ModelBase::toJson(m_NotifCorelationId);
    {
        nlohmann::json jsonArray;
        for( auto& item : m_Reports )
        {
            jsonArray.push_back(ModelBase::toJson(item));
        }
        
        if(jsonArray.size() > 0)
        {
            val["reports"] = jsonArray;
        } 
    }
    

    return val;
}

void AmfEventNotification::fromJson(const nlohmann::json& val)
{
    setSubscriptionId(val.at("subscriptionId"));
    setNotifCorelationId(val.at("notifCorelationId"));
    {
        m_Reports.clear();
        if(val.find("reports") != val.end())
        {
            for( auto& item : val["reports"] )
            {
                
                if(item.is_null())
                {
                    m_Reports.push_back( AmfEventReport() );
                }
                else
                {
                    AmfEventReport newItem;
                    newItem.fromJson(item);
                    m_Reports.push_back( newItem );
                }
                
            }
        }
    }
    
}


std::string AmfEventNotification::getSubscriptionId() const
{
    return m_SubscriptionId;
}
void AmfEventNotification::setSubscriptionId(std::string const& value)
{
    m_SubscriptionId = value;
    
}
std::string AmfEventNotification::getNotifCorelationId() const
{
    return m_NotifCorelationId;
}
void AmfEventNotification::setNotifCorelationId(std::string const& value)
{
    m_NotifCorelationId = value;
    
}
std::vector<AmfEventReport>& AmfEventNotification::getReports()
{
    return m_Reports;
}
bool AmfEventNotification::reportsIsSet() const
{
    return m_ReportsIsSet;
}
void AmfEventNotification::unsetReports()
{
    m_ReportsIsSet = false;
}

}
}
}
}

