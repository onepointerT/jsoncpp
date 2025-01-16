
#pragma once

#include <list>
#include <string>
#include <string_view>

#include "json_objects.hpp"

namespace jsoncpp {

std::string_view variableValue( const std::string str_value, const JsonListView* json_values
                            , const bool recusive_replace = true, const char var_delim = '=' );

namespace vv { // variable value extension

std::string_view* replaceVariablesInString( const char* str, const JsonListView* json_value_ref, const char var_delim = '=' );
std::string_view findNextVariable( const char* str, size_t& pos, const char var_delim = '=' );
const std::list< std::string_view >* findVariablesInString( const char* str, const char var_delim = '=' );

} // namespace vv
} // namespace jsoncpp
