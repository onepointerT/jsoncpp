
#include "json_stringify.hpp"

#include "json_objects.hpp"

namespace jsoncpp {

std::string stringify( const Json* json ) {
    return Json::toString( json );
}


Json* rstringify( const std::string json_str ) {
    return Json::fromString( json_str.c_str() );
}

} // namespace jsoncpp