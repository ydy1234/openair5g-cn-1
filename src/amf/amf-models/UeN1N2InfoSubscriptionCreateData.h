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
/*
 * UeN1N2InfoSubscriptionCreateData.h
 *
 * 
 */

#ifndef UeN1N2InfoSubscriptionCreateData_H_
#define UeN1N2InfoSubscriptionCreateData_H_


#include "ModelBase.h"

#include "N1MessageClass.h"
#include "N2InformationClass.h"
#include <string>

namespace org {
namespace openapitools {
namespace server {
namespace model {

/// <summary>
/// 
/// </summary>
class  UeN1N2InfoSubscriptionCreateData
    : public ModelBase
{
public:
    UeN1N2InfoSubscriptionCreateData();
    virtual ~UeN1N2InfoSubscriptionCreateData();

    /////////////////////////////////////////////
    /// ModelBase overrides

    void validate() override;

    nlohmann::json toJson() const override;
    void fromJson(const nlohmann::json& json) override;

    /////////////////////////////////////////////
    /// UeN1N2InfoSubscriptionCreateData members

    /// <summary>
    /// 
    /// </summary>
    N2InformationClass getN2InformationClass() const;
    void setN2InformationClass(N2InformationClass const& value);
    bool n2InformationClassIsSet() const;
    void unsetN2InformationClass();
    /// <summary>
    /// 
    /// </summary>
    std::string getN2NotifyCallbackUri() const;
    void setN2NotifyCallbackUri(std::string const& value);
    bool n2NotifyCallbackUriIsSet() const;
    void unsetN2NotifyCallbackUri();
    /// <summary>
    /// 
    /// </summary>
    N1MessageClass getN1MessageClass() const;
    void setN1MessageClass(N1MessageClass const& value);
    bool n1MessageClassIsSet() const;
    void unsetN1MessageClass();
    /// <summary>
    /// 
    /// </summary>
    std::string getN1NotifyCallbackUri() const;
    void setN1NotifyCallbackUri(std::string const& value);
    bool n1NotifyCallbackUriIsSet() const;
    void unsetN1NotifyCallbackUri();
    /// <summary>
    /// 
    /// </summary>
    std::string getLmfId() const;
    void setLmfId(std::string const& value);
    bool lmfIdIsSet() const;
    void unsetLmfId();
    /// <summary>
    /// 
    /// </summary>
    std::string getSupportedFeatures() const;
    void setSupportedFeatures(std::string const& value);
    bool supportedFeaturesIsSet() const;
    void unsetSupportedFeatures();

protected:
    N2InformationClass m_N2InformationClass;
    bool m_N2InformationClassIsSet;
    std::string m_N2NotifyCallbackUri;
    bool m_N2NotifyCallbackUriIsSet;
    N1MessageClass m_N1MessageClass;
    bool m_N1MessageClassIsSet;
    std::string m_N1NotifyCallbackUri;
    bool m_N1NotifyCallbackUriIsSet;
    std::string m_LmfId;
    bool m_LmfIdIsSet;
    std::string m_SupportedFeatures;
    bool m_SupportedFeaturesIsSet;
};

}
}
}
}

#endif /* UeN1N2InfoSubscriptionCreateData_H_ */
