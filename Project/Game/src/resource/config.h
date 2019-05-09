#ifndef PLATFORMER_RESOURCE_CONFIG_H
#define PLATFORMER_RESOURCE_CONFIG_H

#include <json/json.h>

class Config {
   public:
    virtual ~Config() {}
    //!Return root
    virtual const Json::Value& GetRoot() const = 0;
    //! Return value 
    virtual const Json::Value& GetValue(const std::string& name) const = 0;

    //! Parse an image/sprite Json object into <id, filename, rects> tuple
    virtual std::tuple<std::string, std::string, std::vector<std::vector<int>>>
    ParseImageValue(const Json::Value& value) const = 0;
};

#endif
