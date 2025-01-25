
#pragma once

#include <cstdarg>
#include <list>
#include <map>
#include <ranges>
#include <string>
#include <string_view>
#include <typeinfo>
#include <type_traits>

#include "json_serializer.hpp"


namespace jsoncpp {


/**
 * @brief This is a text-serializable class that has a value reference of the type `T`
 * @tparam The type of the value of the json value's type
 */
template< typename T >
class JsonTextSerializableType
    :   public JsonSerializableObject
{
public:
    /** @brief A reference to `T`, the value of the json value's type */
    T& value;

protected:
    /**
     * @brief Constructor
     * @param keystr The name of the value as string
     * @param valueref A reference to `T`, the value
     */
    JsonTextSerializableType( const char* keystr, T& valueref )
        :   JsonSerializableObject( keystr )
        ,   value( valueref )
    {}

    /**
     * @brief Get the value reference as string
     * @note Uses `typeToString<T>( value )`
     */
    virtual const char* toString() const {
        return typeToString<T>( value );
    }

    /**
     * @brief Make a string to your value reference's type
     * @param str The value string to transform
     * @returns The new value of `this->value`
     * @note Uses `stringToType<T>( str )`
     */
    virtual T& toType( const char* str ) {
        this->value = stringToType<T>( str );
        return this->value;
    }

public:
    /**
     * @brief Convert this inherited `JsonTextSerializableType` to a `JsonValue`
     * @returns A reference to a `JsonValue` with every value already set
     * @note This function requires the function `typeToString<T>()`
     */
    virtual JsonValue& toJsonValue() {
        return JsonSerializableObject::toJsonValue<T>( this->value );
    };

    /**
     * @brief Convert this inherited `JsonTextSerializableType` to a `JsonValue`
     * @returns A pointer to a `JsonValue` with every value already set
     * @note This function requires the function `typeToString<T>()`
     */
    virtual JsonValue* toJson() const {
        return JsonSerializableObject::toJson<T>( this->value );
    }

    /**
     * @brief Convert the reference to a `JsonValue` to this inherited `JsonTextSerializableType`
     * @param json_value A reference to a `JsonValue` to convert from string for the value of
     *      the `JsonValue`
     * @returns A reference to the value `T` which is determined by `jsonValueToType<T>(value)`
     * @note This function requires the function `stringToType<T>()`
     */
    virtual T& fromJsonValue( const JsonValue& json_value ) {
        this->value = JsonSerializableObject::fromJsonValue<T>( json_value );
        return this->value;
    }

    /**
     * @brief Set a json value of this instance with the streaming operator.
     * @param json_value The value to hand the key/value pair of this object's instance to
     */
    virtual void operator>>( JsonValue& json_value ) {
        json_value = this->toJsonValue();
    }

    /**
     * @brief Set only the json value of this instance with the streaming operator.
     * @param json_value The value to hand the value `T` of this object's instance to
     */
    virtual void operator>>( T& json_value ) {
        json_value = this->value;
    }

    /**
     * @brief Set all values of this key/value pair instance of a json value to this instance
     *      with the streaming operator.
     * @param json_value The value to get the key/value pair of this object's instance from
     * @returns A reference to this instance
     */
    virtual JsonTextSerializableType<T>& operator<<( const JsonValue& json_value ){
        this->value = this->fromJsonValue( json_value );
        return *this;
    }

    /**
     * @brief Get the value reference as string
     */
    virtual const std::string str() const {
        return this->toString();
    }

    /**
     * @brief Get the value reference as C-string
     */
    virtual const char* c_str() const {
        return this->toString();
    }

    /**
     * @brief Get the key/value pair as string in the format `"key": "value"`
     */
    virtual const std::string strJson() const {
        return "\"" + this->key + "\": \"" + this->toStringValue<T>( this->value ) + "\"";
    }

    /**
     * @brief Assign a new value
     * @tparam R The type returned
     * @tparam P The type parameterized
     * @param param The value to set
     * @returns A reference to `this` in the first definition of the function
     * @note This function is also used in the generic `R operator=(P)` definitions and elsewhere
     */
    template< typename R, typename P >
    R assign( const P param ) {
        this->value = typeToType< P, T >( param );
        return *this;
    }

    /**
     * @brief Assign a new value from another `JsonTextSerializableType<T>`
     * @param json_tobj A reference to an instance of another `JsonTextSerializableType<T>`
     * @returns A reference to `this`
     */
    JsonTextSerializableType<T>& assign( const JsonTextSerializableType<T>& json_tobj ) {
        this->key = json_tobj.key;
        this->value = json_tobj.value;
        return *this;
    }
    /**
     * @brief Assign a new value from another `JsonValue`
     * @param json_value A reference to an instance of a `JsonValue`
     * @returns A reference to `this`
     */
    JsonTextSerializableType<T>& assign( const JsonValue& json_value ) {
        this->fromJsonValue( json_value );
        return *this;
    }

    /**
     * @brief Assign an value `P param` to this instance of `JsonTextSerializableType<T>` object
     * @tparam R The returned value, typically a reference to `this` or its inheriting instance
     * @tparam P The type to assign to this instance
     * @param param The value `P` to assign to the value of `T&` of this instance
     * @returns Whatever the fitting `assign< R, P >` funnction returns
     * @note A value `P` is assignable to a class `R` with value type `T`, when it is `<P,T>` assignable.
     */
    template< typename R, typename P >
    R operator=( const P param ) {
        return this->template assign<R, P>(param);
    }

    /**
     * @brief Assign a new value from another `JsonTextSerializableType<T>`
     * @param json_tobj A reference to an instance of another `JsonTextSerializableType<T>`
     * @returns Whatever the fitting `assign< R, P >` funnction returns
     */
    JsonTextSerializableType<T>& operator=( const JsonTextSerializableType<T>& json_tobj ) {
        return this->assign( json_tobj );
    }

    /**
     * @brief Assign a new value from another `JsonValue`
     * @param json_value A reference to an instance of a `JsonValue`
     * @returns Whatever the fitting `assign< R, P >` funnction returns
     */
    JsonTextSerializableType<T>& operator=( const JsonValue& json_value ) {
        return this->assign( json_value );
    }
};


/** @brief Advice the preprocessor to compile one object of your value `T` of `JsonTextSerializableType<T>` for you. */
#define JSONCPP_TYPE(value_type) \
    template<> \
    class JsonTextSerializableType<decltype(value_type)>; \


/**
 * @brief The `namespace types {...}` includes every built-in type and the
 *      generic `JsonType< YourType, BaseType >` to inherit from and define.
 */
namespace types {
namespace detail {

} // namespace detail
// value = false / null / true / object / array / number / string


/**
 * @brief A type that includes and inherits function for transformation of e.g.
 *      `struct YourStruct` with `YourStruct = BT` to a json-text serializable class
 *      `class YourJsonType` with `YourJsonType = JT` that extensible and responsibly
 *      enables you to serialized your struct-typed classes to json and json strings/files.
 * @example JsonType<YourType, BaseStructedType>
 * \code
 * struct CacaoExpression {
    std::string_view expr_str;
};

struct ddkml_obj {
    std::string_view var_name = "";
    std::string sql = "";
    CacaoExpression cacaoext;
    std::string dstruct = "";
};


class DdkmlObj
    :   public jsoncpp::types::JsonType< DdkmlObj, ddkml_obj >
{...}; // <- Your definitions there
\endcode

    need only a few functions to be string-transformable and usable with json:

\code
    namespace jsoncpp {
template<>
ddkml::ddkml_obj& stringToType( const char* str );
template<>
const char* typeToString( const ddkml::ddkml_obj& obj );
template<>
ddkml::ddkml_obj& typeToType( const jsoncpp::JsonValue& json );
template<>
jsoncpp::JsonValue& typeToType( const ddkml::ddkml_obj& obj );
} // namespace jsoncpp
\endcode
 * @note All functions you need for the inherited `JsonTextSerializableType<T>` and
 *      its value `T& value` are described there and above. The class `JsonType<JT, BT>`
 *      also needs (additional to the string-to-type functions the `typeToType<BT, JsonValue>`
 *      function in both transformation directions.)
 * @tparam JT The type of your text-serializable and json-usable class
 * @tparam BT The base type of the struct or value a json value is to be serialized to
 */
template< class JT, typename BT >
class JsonType
    :   public JsonTextSerializableType< BT >
{
protected:
    /** @brief A pointer to the inheriting type's instance for return-valued operators and function */
    JT* m_jsontype;

public:
    /**
     * @brief Constructor
     * @param keystr The name of the value, typically quotated left of the letter ':' in json key/value strings
     *      or the first value of a `JsonValue`
     * @param valueref A reference to an at least instanciatd value of the type `BT`
     * @param inheriting_type A pointer to the instance of the inheriting type for use at the return
     *          value of operators and non-virtual functions for besser suiting into your deriving class
     * @note This class inherits from `JsonTextSerializableType< BT >` so all serialization from and to string
     *      and `JsonValue` will be inherited and compiled for your class.
     */
    JsonType( const char* keystr, const BT& valueref, JT* inheriting_type  )
        :   JsonTextSerializableType< BT >( keystr, valueref )
        ,   m_jsontype( inheriting_type )
    {}
    /**
     * @brief Constructor
     * @param keystr The name of the value, typically quotated left of the letter ':' in json key/value strings
     *      or the first value of a `JsonValue`
     * @param value A value of the type `BT` to be initialized-to-reference by the type's copy-constructor
     * @param inheriting_type A pointer to the instance of the inheriting type for use at the return
     *          value of operators and non-virtual functions for besser suiting into your deriving class
     * @note This class inherits from `JsonTextSerializableType< BT >` so all serialization from and to string
     *      and `JsonValue` will be inherited and compiled for your class.
     */
    JsonType( const char* keystr, const BT value, JT* inheriting_type )
        :   JsonTextSerializableType< BT >( keystr, *(new BT(value)) )
        ,   m_jsontype( inheriting_type )
    {}
    /**
     * @brief Constructor
     * @param keystr The name of the value, typically quotated left of the letter ':' in json key/value strings
     *      or the first value of a `JsonValue`
     * @param value A value of the type `BT` to be initialized-to-reference by the type's copy-constructor
     * @param inheriting_type A pointer to the instance of the inheriting type for use at the return
     *          value of operators and non-virtual functions for besser suiting into your deriving class
     * @note This class inherits from `JsonTextSerializableType< BT >` so all serialization from and to string
     *      and `JsonValue` will be inherited and compiled for your class.
     */
    JsonType( const std::string keystr, const BT value, JT* inheriting_type )
        :   JsonTextSerializableType< BT >( keystr.c_str(), *(new BT(value))  )
        ,   m_jsontype( inheriting_type )
    {}

    static BT& toBaseType( const JT& json_type_value ) {
        return json_type_value.toBaseType();
    }

    BT& toBaseType() const {
        return this->value;
    }

    /**
     * @brief Get the value as string
     */
    virtual const std::string str() const {
        return JsonSerializableObject::toStringValue< BT >( toBaseType() );
    }
    /**
     * @brief Get the value as C-string
     */
    virtual const char* c_str() const {
        return str().c_str();
    }

    /**
     * @brief Assign another instance's reference to this object
     * @param json_type Another `JsonType<JT,BT>` with key and value set
     * @returns A reference to the inheriting instance
     */
    virtual JT& operator=( const JsonType<JT, BT>& json_type ) {
        this->key = json_type.key;
        this->value = json_type.value;
        return *(this->m_jsontype);
    }

    /**
     * @brief Assign another instance's reference to this object
     * @param json_type Another `JsonType<JT,BT>` with key and value set
     * @returns A reference to the inheriting instance
     */
    virtual JsonType<JT, BT>& assign( const JsonType<JT, BT>& json_type_value ) {
        this->key = json_type_value.key;
        this->value = json_type_value.value;
        return *this;
    }

    /**
     * @brief Assign a new value
     * @tparam R The type returned
     * @tparam P The type parameterized
     * @param param The value to set
     * @returns A reference to `this` in the first definition of the function
     * @note This function is also used in the generic `R operator=(P)` definitions and elsewhere
     */
    template< typename R, typename P >
    friend R assign( const P param );

    template< typename R, typename P >
    BT& assign( const JT& json_type_value ) {
        this->value = toBaseType( json_type_value );
        return this->value;
    }

    template< typename R, typename P >
    BT& assign( const JsonType<JT, BT> json_type ) {
        this->key = json_type.key;
        this->value = json_type.value;
        return json_type.value;
    }

    template< typename R, typename P >
    JT& assign( const JsonType<JT, BT>& json_type ) {
        this->key = json_type.key;
        this->value = json_type.value;
        return *(this->m_jsontype);
    }

    template< typename R, typename P >
    JT& assign( const BT& base_type_value ) {
        this->value = base_type_value;
        return *(this->m_jsontype);
    }

    template< typename R, typename P >
    JT& assign( const BT base_type_value ) {
        this->value = &base_type_value;
        return *(this->m_jsontype);
    }

    // Also use assignments from `JsonTextSerializableType<BT>`
    using JsonTextSerializableType< BT >::assign;

    /**
     * @brief A generic assignment operator.
     */
    template< typename R, typename P >
    R operator=( const P param ) {
        return this->template assign<R, P>(param);
    }

    // Also use assignment operators from `JsonTextSerializableType<BT>`
    using JsonTextSerializableType< BT >::operator=;

    JsonType<JT, BT>& operator=( JT& json_type_value ) {
        return this->template assign<JsonType<JT, BT>&, JT&>( json_type_value );
    }

    BT& operator=( const JT& json_type_value ) {
        return this->template assign<BT&, JT&>( json_type_value );
    }

    JT& operator=( const BT& base_type_ref ) {
        return this->template assign<JT&, BT&>( base_type_ref );
    }

    JT& operator=( const BT base_type_value ) {
        return this->template assign<JT&, BT>( base_type_value );
    }

    void operator>>( BT base_type_value ) const {
        base_type_value = this->value;
    }

    void operator>>( BT& base_type_ref ) const {
        base_type_ref = this->value;
    }

    void operator>>( JT& json_type_ref) const {
        json_type_ref = *(new JT(this->key, this->value, this));
    }

    // Also use streaming operators and set/get functions from `JsonTextSerializableType<BT>`
    using JsonTextSerializableType< BT >::operator>>;
    using JsonTextSerializableType< BT >::operator<<;

    operator JT() const { return this->m_jsontype; }
    operator BT() const { return this->value; }
};


class Bool
    :   public JsonType< Bool, bool >
{
public:
    Bool( const std::string keystr, const bool value );
};

#define JSON_BOOL(bl) jsoncpp::types::Bool( "Bool", bl )
#define JSON_TRUE JSON_BOOL(true)
#define JSON_FALSE JSON_BOOL(false)

#define JSON_NULLSTR "null"

class Null
    :   public JsonType< Null, std::nullptr_t >
{
public:
    Null( const std::string keystr );
    Null( const std::string keystr, const std::nullptr_t value = nullptr );

    virtual const std::string str() const;
};

#define JSON_NULL jsoncpp::types::Null( "Null", nullptr )

class Integer
    :   public JsonType< Integer, int >
{
public:
    Integer( const std::string keystr, const int value );
};

#define JSON_INTEGER(num) json::types::Integer( "Integer", num )

class IntegerUnsigned
    :   public JsonType< IntegerUnsigned, uint >
{
public:
    IntegerUnsigned( const std::string keystr, const uint value );
};

#define JSON_UINTEGER(num) json::types::IntegerUnsigned( "IntegerUnsigned", num )

class Double
    :   public JsonType< Double, double >
{
public:
    Double( const std::string keystr, const double value );
};

#define JSON_DOUBLE(num) json::types::Double( "Double", num )

namespace detail {
template< typename T >
concept json_number = requires ( T t ) {
       typeid(T) == typeid(int)
    || typeid(T) == typeid(uint)
    || typeid(T) == typeid(double);
};
} // namespace detail


template< typename N >
concept JSON_NUMBER = detail::json_number< N >;

template< typename N > requires detail::json_number<N>
class Number
    :   public JsonType< Number<N>, N >
{
public:
    Number( const std::string keystr, const N num )
        :   JsonType< Number<N>, N >( keystr, num, this )
    {}

    //template< typename JT >
    //friend void operator>>( JT json_type ) override;

    template< typename JT >
    void operator>>( Integer& integer ) {
        Number< JT >* n = this;
        if ( typeid(JT) == typeid(int) ) {
            Integer* integer_new = new Integer( this->key, this->value );
            integer = *integer_new;
        }
    }

    template< typename JT >
    void operator>>( IntegerUnsigned& uinteger ) {
        Number< JT >* n = this;
        if ( typeid(JT) == typeid(uint) ) {
            IntegerUnsigned* uinteger_new = new IntegerUnsigned( this->key, this->value );
            uinteger = *uinteger_new;
        }
    }

    template< typename JT >
    void operator>>( Double& doubl ) {
        Number< JT >* n = this;
        if ( typeid(JT) == typeid(double) ) {
            Double* doubl_new = new Double( this->key, this->value );
            doubl = *doubl_new;
        }
    }
};

#define JSON_NUMBER(num) Number< decltype(num) >( "Number", num  )


template class Number<int>;
template class Number<uint>;
template class Number<double>;


namespace detail {
template< typename T >
concept json_object_type = requires ( T t ) {
       std::is_convertible< T, bool >::value
    || std::is_convertible< T, std::nullptr_t >::value
    || std::is_convertible< T, int >::value
    || std::is_convertible< T, uint >::value
    || std::is_convertible< T, double >::value
    || std::is_convertible< T, std::string >::value;
};
} // namespace detail


template< typename T >
concept JSON_OBJECT = detail::json_object_type< T >;


class Object
    :   public JsonType< Object, std::string >
{
protected:
    template< typename T > requires detail::json_object_type< T >
    static std::string toStringValue( const T value ) {
        return typeToString<T>( value );
    }
    template< typename T > requires detail::json_object_type< T >
    T& toType( const std::string value ) {
        return stringToType<T>( value.c_str() );
    }

public:
    template< typename T > requires detail::json_object_type< T >
    Object( const std::string keystr, const T value )
        :   JsonType< Object, std::string >( keystr, toStringValue( value ), this )
    {}

    Object( const std::string keystr, const std::string value );

    template< typename BT > requires detail::json_object_type< BT >
    void operator>>( BT base_type_value ) const {
        base_type_value = toType< BT >( this->value );
    }

    template< typename BT > requires detail::json_object_type< BT >
    void operator>>( BT& base_type_ref ) const {
        BT* bt = new BT( toType<BT>(this->value) );
        base_type_ref = &bt;
    }

    template< typename BT > requires detail::json_object_type< BT >
    void operator>>( Number< BT >& base_type_num ) const {
        JsonType< std::string, BT >* jt = this;
        BT bt;
        *this >> bt;
        base_type_num = *(new Number< BT >( jt->key, bt ));
    }

    template< typename BT > requires detail::json_object_type< BT >
    BT& operator<<( const Object json_obj ) {
        this->key = json_obj.key;
        this->value = json_obj.value;
        return toType< BT >( this->value );
    }

    static bool greater( const Object& lhs, const Object& rhs );

    std::strong_ordering operator<=>( const Object& other );
};


class Array
    :   protected JsonType< Array, std::list< Object > >
{
public:
    Array( const std::string keystr );
    Array( const std::string keystr, std::list< Object > json_obj_lst );

    Object& operator[]( const size_t pos );
    const Object& operator[]( const size_t pos ) const;

    void operator>>( std::list< Object > json_obj_lst ) const;
    void operator>>( std::list< Object >& json_obj_lst ) const;

    Array& operator<<( Object& json_obj );
    Array& operator<<( std::list< Object >& json_obj_lst );
};


class String
    :   public JsonType< String, std::string_view >
{
public:
    String( const std::string keystr );
    String( const std::string keystr, const std::string valuestr );
};



} // namespace types

template<>
types::Bool& typeToType( const bool& start_type ) {
    if ( start_type == true ) return *(new JSON_TRUE);
    return *(new JSON_FALSE);
}

template<>
types::Null& typeToType( const std::nullptr_t& start_type ) {
    return *(new JSON_NULL);
}

template<>
std::nullptr_t& typeToType( const types::Null& start_type ) {
    std::nullptr_t* null = new std::nullptr_t(nullptr);
    return *null;
}

template<>
types::Integer& typeToType( const int& start_type ) {
    types::Integer* integer = new types::Integer( "Integer", start_type );
    return *integer;
}

template<>
types::IntegerUnsigned& typeToType( const unsigned int& start_type ) {
    types::IntegerUnsigned* uinteger = new types::IntegerUnsigned( "IntegerUnsigned", start_type );
    return *uinteger;
}

template<>
types::Double& typeToType( const double& start_type ) {
    types::Double* dbl = new types::Double( "Double", start_type );
    return *dbl;
}

template<>
types::Number<int>& typeToType( const int& start_type ) {
    types::Number<int>* integer = new types::Number<int>( "Number", start_type );
    return *integer;
}

template<>
types::Number<unsigned int>& typeToType( const unsigned int& start_type ) {
    types::Number<unsigned int>* uinteger = new types::Number<unsigned int>( "Number", start_type );
    return *uinteger;
}

template<>
types::Number<double>& typeToType( const double& start_type ) {
    types::Number<double>* dbl = new types::Number<double>( "Number", start_type );
    return *dbl;
}

template<>
types::Integer& typeToType( const types::Number<int>& start_type ) {
    types::Integer* integer = new types::Integer( "Integer", start_type.value );
    return *integer;
}

template<>
types::IntegerUnsigned& typeToType( const types::Number<uint>& start_type ) {
    types::IntegerUnsigned* uinteger = new types::IntegerUnsigned( "IntegerUnsigned", start_type.value );
    return *uinteger;
}

template<>
types::Double& typeToType( const types::Number<double>& start_type ) {
    types::Double* dbl = new types::Double( "Double", start_type.value );
    return *dbl;
}
    



template< typename ST, typename RT >
RT& typeToType( const ST& start_type ) {
    if ( std::is_base_of<types::JsonType<ST, RT>, ST>::value )
        return start_type.value;
    return typeToType<>( start_type );
}

} // namespace jsoncpp
