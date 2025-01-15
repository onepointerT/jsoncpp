
#include "regex.hpp"

#include <regex>


namespace jsoncpp {


Regex::Regex( const char* str )
    :   std::string( str )
    ,   flag( Singleline )
{}

Regex::Regex( const char* str, const FlagType flag_type )
    :   std::string( str )
    ,   flag( flag_type )
{}

std::regex_constants::syntax_option_type Regex::type( const FlagType) const {
    return ( flag == FlagType::Multiline 
                ? std::regex_constants::ECMAScript | std::regex_constants::multiline
                : std::regex_constants::ECMAScript
            );
}

Regex& Regex::operator=( const char* str ) {
    Regex* regex = new Regex( str );
    return *regex;
}

Regex& Regex::operator+( const Regex& other ) {
    *this += other;
    return *this;
}

Regex& Regex::operator+( const char* other ) {
    *this += other;
    return *this;
}

std::string_view Regex::match( const char* str ) const {
    std::cmatch cm;
    if ( std::regex_match( str, cm, std::regex(*this, type()) ) )
        return cm[0].str();
    return "";
}

std::vector< std::string_view >& Regex::matchall( const char* str ) const {
    std::smatch sm;
    std::string search_str = str;
    std::regex_search( search_str, sm, std::regex(*this, type()) );

    std::vector< std::string_view >* vec = new std::vector< std::string_view >();
    for ( unsigned int rsi = 0; rsi < sm.size(); rsi++ ) {
        std::string_view strv = sm[rsi].str();
        vec->push_back( strv );
    }
    return *vec;
}


} // namespace jsoncpp

