
#pragma once

#include <string>

namespace jsoncpp {

class Json;


std::string stringify( const Json* json );
Json* rstringify( const std::string json_str );



} // namespace jsoncpp
