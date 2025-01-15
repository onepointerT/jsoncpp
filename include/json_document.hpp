
#pragma once

#include "json_objects.hpp"

namespace jsoncpp {

class JsonDocument {
public:
    static Json* fromFile( const char* filepath );
    static void toFile( const char* filepath, const Json* json );
};

} // namespace jsoncpp
