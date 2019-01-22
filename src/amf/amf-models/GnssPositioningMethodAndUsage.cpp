/**
* AMF Location Service
* AMF Location Service
*
* OpenAPI spec version: 1.R15.0.0
* 
*
* NOTE: This class is auto generated by OpenAPI Generator (https://openapi-generator.tech).
* https://openapi-generator.tech
* Do not edit the class manually.
*/


#include "GnssPositioningMethodAndUsage.h"

namespace org {
namespace openapitools {
namespace server {
namespace model {

GnssPositioningMethodAndUsage::GnssPositioningMethodAndUsage()
{
    
}

GnssPositioningMethodAndUsage::~GnssPositioningMethodAndUsage()
{
}

void GnssPositioningMethodAndUsage::validate()
{
    // TODO: implement validation
}

nlohmann::json GnssPositioningMethodAndUsage::toJson() const
{
    nlohmann::json val = nlohmann::json::object();

    val["mode"] = ModelBase::toJson(m_Mode);
    val["gnss"] = ModelBase::toJson(m_Gnss);
    val["usage"] = ModelBase::toJson(m_Usage);
    

    return val;
}

void GnssPositioningMethodAndUsage::fromJson(const nlohmann::json& val)
{
    
}


PositioningMode GnssPositioningMethodAndUsage::getMode() const
{
    return m_Mode;
}
void GnssPositioningMethodAndUsage::setMode(PositioningMode const& value)
{
    m_Mode = value;
    
}
GnssId GnssPositioningMethodAndUsage::getGnss() const
{
    return m_Gnss;
}
void GnssPositioningMethodAndUsage::setGnss(GnssId const& value)
{
    m_Gnss = value;
    
}
Usage GnssPositioningMethodAndUsage::getUsage() const
{
    return m_Usage;
}
void GnssPositioningMethodAndUsage::setUsage(Usage const& value)
{
    m_Usage = value;
    
}

}
}
}
}

