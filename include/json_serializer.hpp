
#pragma once

#include <charconv>
#include <cstddef>
#include <string>
#include <vector>

namespace jsoncpp {

/**
 * @brief An atomic json value
 */
using JsonValue = std::pair< std::string, std::string >;


/**
 * @brief Conversion function (T -> const char*)
 * @note Please define one per own `JsonType< BT, T >`
 */
template< typename T >
const char* typeToString( const T& val );

/**
 * @brief Conversion function. Strongly used for `JsonType< JT, BT >`
 * @note Please define two per own `JsonType< JT, BT >`, one for each conversion direction
 */
template< typename ST, typename RT >
RT& typeToType( const ST& start_type );

/**
 * @brief Conversion function.
 */
template< typename T >
T& stringToType( const char* str );

template< typename T >
JsonValue* typeToJsonValue( const T& value );

template< typename T >
T& jsonValueToType( const JsonValue& value );


class JsonSerializableObject {
public:
    std::string key;

protected:
    JsonSerializableObject( const char* keystr );

    template< typename T >
    T& toType( const char* str ) const {
        return stringToType< T >( str );
    }

    template< typename T >
    std::string toStringValue( const T& value ) const {
        return typeToString< T >( value );
    }

    virtual JsonValue* toJson() const {
        return new JsonValue( key, "" );
    }

    template< typename T >
    JsonValue& toJsonValue( const T& value ) const {
        return std::make_pair( key, *typeToJsonValue<T>( value ) );
    }

    template< typename T >
    T& fromJsonValue( const JsonValue& value ) {
        this->key = value.first;
        return jsonValueToType<T>( value );
    }
};


#define FALSE "false"
#define TRUE "true"
#define NULLPTR_T std::nullptr_t
#define NULLSTR "null"


template<>
const char* typeToString( const bool& val ) {
    if ( val == true ) return TRUE;
    return FALSE;
}

template<>
bool& stringToType( const char* str ) {
    bool* bl = new bool(true);
    if ( str == FALSE ) bl = new bool(false);
    return *bl;
}


template<>
const char* typeToString( const std::nullptr_t& val ) {
    return NULLSTR;
}

template<>
std::nullptr_t& stringToType( const char* str ) {
    std::nullptr_t* null = new std::nullptr_t(nullptr);
    return *null;
}


template<>
const char* typeToString( const int& val ) {
    std::string str;
    return std::to_chars( str.data(), str.data(), val ).ptr;
}

template<>
int& stringToType( const char* str ) {
    std::string s = str;
    int result{};
    std::from_chars( s.data(), s.data() + s.size(), result );
    int* i = new int(result);
    return *i;
}


template<>
const char* typeToString( const unsigned int& val ) {
    std::string str;
    return std::to_chars( str.data(), str.data(), val ).ptr;
}

template<>
unsigned int& stringToType( const char* str ) {
    std::string s = str;
    unsigned int result{};
    std::from_chars( s.data(), s.data() + s.size(), result );
    unsigned int* i = new uint(result);
    return *i;
}


template<>
const char* typeToString( const double& val ) {
    std::string str;
    return std::to_chars( str.data(), str.data(), val ).ptr;
}

template<>
double& stringToType( const char* str ) {
    std::string s = str;
    double result{};
    std::from_chars( s.data(), s.data() + s.size(), result );
    double* i = new double(result);
    return *i;
}


template<>
JsonValue* typeToJsonValue( const std::string& value ) {
    return new JsonValue( "", value );
}

template<>
JsonValue* typeToJsonValue( const std::nullptr_t& value ) {
    return new JsonValue( "", NULLSTR );
}

template<>
JsonValue* typeToJsonValue( const int& value ) {
    return new JsonValue( "", std::to_string( value ) );
}

template<>
JsonValue* typeToJsonValue( const uint& value ) {
    return new JsonValue( "", std::to_string( value ) );
}

template<>
JsonValue* typeToJsonValue( const double& value ) {
    return new JsonValue( "", std::to_string( value ) );
}


template<>
std::string& jsonValueToType( const JsonValue& value ) {
    std::string* s = new std::string( value.second );
    return *s;
}

template<>
std::nullptr_t& jsonValueToType( const JsonValue& value ) {
    return *(new std::nullptr_t(nullptr));
}

template<>
int& jsonValueToType( const JsonValue& value ) {
    int* i = new int(std::stoi( value.second ));
    return *i;
}

template<>
uint& jsonValueToType( const JsonValue& value ) {
    uint* ui = new uint(std::stoul( value.second.c_str() ));
    return *ui;
}

template<>
double& jsonValueToType( const JsonValue& value ) {
    double* d = new double(std::stod( value.second ));
    return *d;
}


} // namespace jsoncpp
