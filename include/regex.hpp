
#pragma once

#include <regex>
#include <string>
#include <string_view>
#include <vector>


namespace jsoncpp {


class Regex
    :   public std::string
{
public:
    enum FlagType {
        Multiline,
        Singleline
    } flag;

    Regex( const char* str );
    Regex( const char* str, const FlagType flag_type );

    std::regex_constants::syntax_option_type type( const FlagType flag_type = Singleline ) const;

    Regex& operator=( const char* str );
    Regex& operator+( const Regex& other );
    Regex& operator+( const char* other );

    Regex& operator<<( const char* other );
    Regex& operator<<( const std::string other );
    Regex& operator<<( const Regex& other );

    virtual std::string_view match( const char* str ) const;
    virtual std::vector< std::string_view >& matchall( const char* str ) const;

    typedef typename std::vector< std::string_view >::const_iterator match_iterator;
};


#define make_regex(var_name) inline jsoncpp::Regex var_name = 
#define make_sregex(var_name) inline jsoncpp::Regex var_name = jsoncpp::Regex("") << 

#define Matchall(str, var_name, regexp) std::vector< std::string_view >& var_name \
                                    = regexp.matchall( str );
#define Match(str, var_name, regexp) std::string_view var_name = regexp.match( str );



class RegexAdvanced
    :   public Regex
{
private:
    size_t pos_opening_setin_var() const;
    size_t pos_closing_setin_var() const;

public:
    enum FlagMode {
        Preamble,
        SetIn
    } mode = SetIn;

    RegexAdvanced( const char* str );
    RegexAdvanced( const char* str, const FlagMode flag_mode, const FlagType flag_type = Singleline );
    RegexAdvanced( const char* str, const FlagType );

    RegexAdvanced& operator=( const char* str );
    RegexAdvanced& operator+( const Regex& other );
    RegexAdvanced& operator+( const RegexAdvanced& other );
    RegexAdvanced& operator+( const char* other );

    RegexAdvanced& operator<<( const char* other );
    RegexAdvanced& operator<<( const std::string other );
    RegexAdvanced& operator<<( const Regex& other );
    RegexAdvanced& operator<<( const RegexAdvanced& other );
    
    bool has_setin_var() const;
    std::size_t find_setinvar_pos() const;
    std::regex fill_setin_variable( const char* value ) const;

    std::string_view match( const char* str, const char* in_between_setin_value ) const;
    std::vector< std::string_view >& matchall( const char* str, const char* in_between_setin_value ) const;
};


#define make_regexadv(var_name) inline jsoncpp::RegexAdvanced var_name = 
#define make_sregexadv(var_name) inline jsoncpp::RegexAdvanced var_name = jsoncpp::RegexAdvanced("") << 

#define MatchallAdv(str, var_name, regexp, setin_var) std::vector< std::string_view >& var_name \
                                    = regexp.matchall( str, setin_var );
#define MatchAdv(str, var_name, regexp, setin_var) std::string_view var_name = regexp.match( str, setin_var );


} // namespace jsoncpp
