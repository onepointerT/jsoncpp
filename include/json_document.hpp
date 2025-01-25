
#pragma once

#include "json_objects.hpp"

namespace jsoncpp {

class JsonDocument {
public:
    /**
     * @brief Read a document from file
     * @param filepath The absolute/relative path to the json file
     * @returns A pointer to a json object
     */
    static Json* fromFile( const char* filepath );
    /**
     * @brief Write a json object to a document to file
     * @param filepath The absolute/relative path to the json file
     * @param json The json object to serialize to file
     */
    static void toFile( const char* filepath, const Json* json );
};

} // namespace jsoncpp
