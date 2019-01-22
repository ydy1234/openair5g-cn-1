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


#include "N1N2MessageTransferError.h"

namespace org {
namespace openapitools {
namespace server {
namespace model {

N1N2MessageTransferError::N1N2MessageTransferError()
{
    m_ErrInfoIsSet = false;
    
}

N1N2MessageTransferError::~N1N2MessageTransferError()
{
}

void N1N2MessageTransferError::validate()
{
    // TODO: implement validation
}

nlohmann::json N1N2MessageTransferError::toJson() const
{
    nlohmann::json val = nlohmann::json::object();

    val["error"] = ModelBase::toJson(m_Error);
    if(m_ErrInfoIsSet)
    {
        val["errInfo"] = ModelBase::toJson(m_ErrInfo);
    }
    

    return val;
}

void N1N2MessageTransferError::fromJson(const nlohmann::json& val)
{
    if(val.find("errInfo") != val.end())
    {
        if(!val["errInfo"].is_null())
        {
            N1N2MsgTxfrErrDetail newItem;
            newItem.fromJson(val["errInfo"]);
            setErrInfo( newItem );
        }
        
    }
    
}


ProblemDetails N1N2MessageTransferError::getError() const
{
    return m_Error;
}
void N1N2MessageTransferError::setError(ProblemDetails const& value)
{
    m_Error = value;
    
}
N1N2MsgTxfrErrDetail N1N2MessageTransferError::getErrInfo() const
{
    return m_ErrInfo;
}
void N1N2MessageTransferError::setErrInfo(N1N2MsgTxfrErrDetail const& value)
{
    m_ErrInfo = value;
    m_ErrInfoIsSet = true;
}
bool N1N2MessageTransferError::errInfoIsSet() const
{
    return m_ErrInfoIsSet;
}
void N1N2MessageTransferError::unsetErrInfo()
{
    m_ErrInfoIsSet = false;
}

}
}
}
}

