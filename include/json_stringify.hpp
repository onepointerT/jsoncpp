
#pragma once

#include <string>

namespace jsoncpp {

// Forward declaration
class Json;


/**
 * @brief Create a string from a json object
 * @param json A pointer to a `Json` objects instance
 * @returns A string ending and starting with braces (`{...}`)
 */
std::string stringify( const Json* json );
/**
 * @brief Create a json object from a json string ending and starting
 *      with braces `{...}`
 * @param json_str The string to serialize to json object
 * @returns A pointer to a `Json` object's instance on success, `nullptr` else
 */
Json* rstringify( const std::string json_str );



} // namespace jsoncpp
