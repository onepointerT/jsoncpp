
#pragma once

#include <list>
#include <string>
#include <string_view>

#include "json_objects.hpp"

namespace jsoncpp {

/**
 * @brief Replace all variative variable values found in `str_value` with the values found in `json_values`.
 *      If `recursive_replace == true`, then a variative variable will be even replaced, when found after
 *      all variable-replacements are done. The delimiter before and behind a variate variable in a json value
 *      string is then delimited with `var_delim`, e.g. `=variate_var_name=`. A bivariative variable can contain
 *      key paths. Returns the value as `string_view` with all bivariate variables substituted by it's string values.
 */
std::string_view variableValue( const std::string str_value, JsonListView* json_values
                            , const bool recusive_replace = true, const char var_delim = '=' );


/** @brief The variative variable value extension */
namespace vv { // variate variable value extension

/**
 * @brief Replace all variative variable values found in `str_value` with the values found in `json_values`.
 *      If `recursive_replace == true`, then a variative variable will be even replaced, when found after
 *      all variable-replacements are done. The delimiter before and behind a variate variable in a json value
 *      string is then delimited with `var_delim`, e.g. `=variate_var_name=`. A bivariative variable can contain
 *      key paths. Returns the value as as pointer-to `string_view` with all bivariate variables substituted by it's string values.
 */
std::string_view* replaceVariablesInString( const char* str, JsonListView* json_value_ref, const char var_delim = '=' );
/**
 * @brief Find the next bivariate variable in `str` starting at and with `pos`, which will be set to the position
 *      behind the variable's delimiter afterwards.
 */
std::string_view findNextVariable( const char* str, size_t& pos, const char var_delim = '=' );
/**
 * @brief Find all variative variables in `str` delimiited before and behind by `var_delim`, e.g. `=root.sub.values_name=`.
 */
const std::list< std::string_view >* findVariablesInString( const char* str, const char var_delim = '=' );

} // namespace vv
} // namespace jsoncpp
