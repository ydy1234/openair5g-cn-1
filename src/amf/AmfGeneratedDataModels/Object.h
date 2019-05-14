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
    void validate() ;

    nlohmann::json toJson() const ;
    void fromJson(nlohmann::json& json) ;

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
