/*
 * Object.h
 *
 *  Created on: Jul 26, 2018
 *      Author: voiture
 */

#ifndef OBJECT_H_
#define OBJECT_H_





#include "ModelBase.h"

namespace org {
namespace openapitools {
namespace server {
namespace model {

class  Object : public ModelBase
{
public:
    Object();
    virtual ~Object();

    /////////////////////////////////////////////
    /// ModelBase overrides
    void validate() override ;

    nlohmann::json toJson() const override ;
    void fromJson(const nlohmann::json& json) override;


     /////////////////////////////////////////////
    /// Object manipulation
   // web::json::value getValue(const utility::string_t& key) const;
   // void setValue(const utility::string_t& key, const web::json::value& value);

private:
    nlohmann::json m_object;
};

}
}
}
}




#endif /* OBJECT_H_ */
