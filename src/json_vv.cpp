
#include "json_vv.hpp"

#include <string>

#include "json_syntax.hpp"

namespace jsoncpp {

std::string_view variableValue( const std::string str_value, const JsonListView* json_values, const bool recusive_replace, const char var_delim ) {

    std::string sv = str_value;

    size_t pos = 0;
    // While there is a variable in `sv`, it will be replaced, if recursive replacing is on
    while ( vv::findNextVariable( sv.c_str(), pos, var_delim ).compare( sv ) == 0 ) {
        sv = *vv::replaceVariablesInString( sv.c_str(), json_values, var_delim );
        if ( ! recusive_replace ) break;
    }

    return sv;
}

namespace vv { // variable value extension


std::string_view* replaceVariablesInString( const char* str, const JsonListView* json_value_ref, const char var_delim ) {

    std::string* s = new std::string( str );
    const std::list< std::string_view >& vars
        = *vv::findVariablesInString( str );
    
    JsonObjectView* jov = nullptr;
    for ( const std::string_view v : vars ) {
        if ( jov == nullptr ) jov = findByKeyPath( v.data(), json_value_ref );
        else jov = findByKeyPath( v.data(), jov );
        if ( jov == nullptr ) continue;

        const size_t pos_var_start = s->find_first_of( var_delim + v.data() + var_delim );
        if ( pos_var_start == s->npos ) continue;
        s->replace( pos_var_start + v.length() + 3, v.length() + 2, jov->value() );
    }

    return new std::string_view( *s );

}



std::string_view findNextVariable( const char* str, size_t& pos, const char var_delim ) {

    std::string_view strv = str;

    const size_t pos_var_start = strv.find_first_of( var_delim, pos );
    if ( pos_var_start == strv.npos ) { pos = strv.npos; return str; }
    const size_t pos_var_end = strv.find_first_of( var_delim, pos_var_start + 1 );
    if ( pos_var_start == strv.npos ) { pos = strv.npos; return str; }

    size_t cpos = pos_var_start + 1;
    while ( cpos < pos_var_end ) if ( strv[cpos] != ' ' ) ++cpos; else { pos = strv.npos; return str; }

    // Yes, it's a real variable
    pos = pos_var_end + 1;
    return strv.substr( pos_var_start + 1, pos_var_end - 1 + pos_var_start );
}


const std::list< std::string_view >* findVariablesInString( const char* str, char var_delim ) {

    std::list< std::string_view >* lst  = new std::list< std::string_view >();
    std::string_view strv = str;

    size_t pos = 0;
    while ( pos != std::string::npos ) {
        strv = vv::findNextVariable( str, pos, var_delim );
        if ( strv != str ) lst->push_front( strv );
    }
    lst->sort();

    return lst;
}

} // namespace vv
} // namespace jsoncpp