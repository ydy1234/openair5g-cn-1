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


#include "AssignEbi.h"

namespace org {
namespace openapitools {
namespace server {
namespace model {

AssignEbi::AssignEbi()
{
    m_PduSessionId = 0;
    m_ArpsIsSet = false;
    m_ReleasedEbiListIsSet = false;
    
}

AssignEbi::~AssignEbi()
{
}

void AssignEbi::validate()
{
    // TODO: implement validation
}

nlohmann::json AssignEbi::toJson() const
{
    nlohmann::json val = nlohmann::json::object();

    val["pduSessionId"] = m_PduSessionId;
    {
        nlohmann::json jsonArray;
        for( auto& item : m_Arps )
        {
            jsonArray.push_back(ModelBase::toJson(item));
        }
        
        if(jsonArray.size() > 0)
        {
            val["arps"] = jsonArray;
        } 
    }
    {
        nlohmann::json jsonArray;
        for( auto& item : m_ReleasedEbiList )
        {
            jsonArray.push_back(ModelBase::toJson(item));
        }
        
        if(jsonArray.size() > 0)
        {
            val["releasedEbiList"] = jsonArray;
        } 
    }
    

    return val;
}

void AssignEbi::fromJson(const nlohmann::json& val)
{
    setPduSessionId(val.at("pduSessionId"));
    {
        m_Arps.clear();
        if(val.find("arps") != val.end())
        {
            for( auto& item : val["arps"] )
            {
                
                if(item.is_null())
                {
                    m_Arps.push_back( Arp() );
                }
                else
                {
                    Arp newItem;
                    newItem.fromJson(item);
                    m_Arps.push_back( newItem );
                }
                
            }
        }
    }
    {
        m_ReleasedEbiList.clear();
        if(val.find("releasedEbiList") != val.end())
        {
            for( auto& item : val["releasedEbiList"] )
            {
                //m_ReleasedEbiList.push_back(item);
                
            }
        }
    }
    
}


int32_t AssignEbi::getPduSessionId() const
{
    return m_PduSessionId;
}
void AssignEbi::setPduSessionId(int32_t const value)
{
    m_PduSessionId = value;
    
}
std::vector<Arp>& AssignEbi::getArps()
{
    return m_Arps;
}
bool AssignEbi::arpsIsSet() const
{
    return m_ArpsIsSet;
}
void AssignEbi::unsetArps()
{
    m_ArpsIsSet = false;
}
std::vector<EpsBearerId>& AssignEbi::getReleasedEbiList()
{
    return m_ReleasedEbiList;
}
bool AssignEbi::releasedEbiListIsSet() const
{
    return m_ReleasedEbiListIsSet;
}
void AssignEbi::unsetReleasedEbiList()
{
    m_ReleasedEbiListIsSet = false;
}

}
}
}
}

