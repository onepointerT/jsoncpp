
#pragma once

#include <charconv>
#include <cstddef>
#include <string>
#include <vector>

namespace jsoncpp {

/**
 * @brief An atomic json value with the value's name as first attribute and the value
 *      as second attribute.
 */
using JsonValue = std::pair< std::string, std::string >;


/**
 * @brief Conversion function (T -> const char*)
 * @tparam T The type to transform from
 * @param val The value to transform to string
 * @returns The transformed string
 * @note Please define one per own inheriting `JsonType< BT, T >` one per own
 *      inheriting `JsonTextSerializableType<T>`
 */
template< typename T >
const char* typeToString( const T& val );

/**
 * @brief Conversion function. Strongly used for `JsonType< JT, BT >`
 * @tparam ST The start type to transform from
 * @tparam RT The return type to get from the transformation
 * @param start_type The value type to transform to `RT`
 * @returns A reference to `RT`, which is been transformed from `start_type`
 * @note Please define two per own `JsonType< JT, BT >`, one for each conversion direction
 */
template< typename ST, typename RT >
RT& typeToType( const ST& start_type );

/**
 * @brief Conversion function.
 * @tparam T The type to transform to
 * @param str The string to transform
 * @returns A reference of `T` which is been transformed from `str`
 * @note Please define one per own inheriting `JsonType< BT, T >` one per own
 *      inheriting `JsonTextSerializableType<T>`
 */
template< typename T >
T& stringToType( const char* str );

/**
 * @brief Create a new `JsonValue` from the value reference of the type `const T`
 * @tparam T The type to transform from
 * @param value A value reference to set in the json value
 * @returns A pointer to a `JsonValue` with unset key and the value `value` as string set
 * @note Uses `typeToString<T>( value )`
 */
template< typename T >
JsonValue* typeToJsonValue( const T& value ) {
    return new JsonValue( "", typeToString<T>( value ) );
}

/**
 * @brief Create a reference from the instance's value of a `JsonValue` reference
 * @tparam T The type to transform to
 * @param value The reference to a `JsonValue`
 * @returns A transformed reference to `T`
 * @note Uses `stringToType<T>( value.second.c_str() )`
 */
template< typename T >
T& jsonValueToType( const JsonValue& value ) {
    return stringToType< T >( value.second.c_str() );
}


/**
 * @brief This is a base class for all types that shall be serializable
 *      from and to json, e. thus `Json`, `JsonObjectView` and `JsonListView`
 */
class JsonSerializableObject {
public:
    /**
     * @brief The name of the json object's value, which is to be implemented
     *      e.g. by `JsonTextSerializableType<T>`
     */
    std::string key;

protected:
    /**
     * @brief Constructor
     * @param keystr The name of the value as string, this means the first value of the
     *      `JsonValue` pair or the first of a `"keystr_name": "value"` string with or without comma
     */
    JsonSerializableObject( const char* keystr );

    /**
     * @brief Convert a string to a type `T`
     * @tparam T The type to convert to
     * @param str The string to convert
     * @returns A reference to `T`, the string was thus converted to `T`
     * @note Uses `stringToType<T>( str )`
     */
    template< typename T >
    T& toType( const char* str ) const {
        return stringToType< T >( str );
    }

    /**
     * @brief Convert a type `T` to a string
     * @tparam T The type to convert from
     * @param value The valued type to convert
     * @returns A reference to `std::string`, the `T& value` was thus converted to the string
     * @returns Uses `typeToString<T>(value)`
     */
    template< typename T >
    std::string toStringValue( const T& value ) const {
        return typeToString< T >( value );
    }

    /**
     * @brief Convert this `JsonSerializableObject` to a `JsonValue`
     * @returns A pointer to a `JsonValue` with the key already set
     */
    virtual JsonValue* toJson() const;

    /**
     * @brief Convert this inherited `JsonSerializableObject` to a `JsonValue`
     * @tparam T The type to convert from
     * @param value A reference to a value to convert to string for the value of
     *      the `JsonValue`
     * @returns A pointer to a `JsonValue` with every value already set
     * @note This function requires the function `typeToString<T>()`
     */
    template< typename T >
    JsonValue* toJson( const T& value ) const {
        return new JsonValue( this->key, this->toStringValue<T>(value) );
    }

    /**
     * @brief Convert this inherited `JsonSerializableObject` to a `JsonValue`
     * @tparam T The type to convert from
     * @param value A reference to a value to convert to string for the value of
     *      the `JsonValue`
     * @returns A reference to a `JsonValue` with every value already set
     * @note This function requires the function `typeToString<T>()`
     */
    template< typename T >
    JsonValue& toJsonValue( const T& value ) const {
        return *( new JsonValue( this->key, this->toStringValue<T>(value) ) );
    }

    /**
     * @brief Convert the reference to a `JsonValue` to this inherited `JsonSerializableObject`
     * @tparam T The type to convert from, e.g. the `T& value` of your deriving class
     * @param value A reference to a `JsonValue` to convert from string for the value of
     *      the `JsonValue`
     * @returns A reference to the value `T` which is determined by `jsonValueToType<T>(value)`
     * @note This function requires the function `stringToType<T>()`
     */
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
