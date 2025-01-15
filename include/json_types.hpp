
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


template< typename T >
class JsonTextSerializableType
    :   public JsonSerializableObject
{
public:
    T& value;

protected:
    JsonTextSerializableType( const char* keystr, T& valueref )
        :   JsonSerializableObject( keystr )
        ,   value( valueref )
    {}

    virtual const char* toString() const {
        return typeToString<T>( value );
    }

    virtual T& toType( const char* str ) {
        value = stringToType<T>( str );
        return value;
    }

public:
    virtual JsonValue& toJsonValue() {
        JsonValue* json_value = new JsonValue( key, str() );
        return *json_value;
    };

    virtual JsonValue* toJson() const {
        return new JsonValue( key, str() );
    }

    virtual T& fromJsonValue( const JsonValue& json_value ) {
        this->value = JsonSerializableObject::fromJsonValue<T>( json_value );
        return this->value;
    }

    virtual void operator>>( JsonValue& json_value ) {
        json_value = toJsonValue();
    }
    virtual void operator>>( T& json_value ) {
        json_value = this->value;
    }

    virtual JsonTextSerializableType<T>& operator<<( const JsonValue& json_value ){
        fromJsonValue( json_value );
        return *this;
    }

    virtual const std::string str() const {
        return toString();
    }

    virtual const char* c_str() const {
        return toString();
    }

    template< typename R, typename P >
    friend R assign( const P param );

    JsonTextSerializableType<T>& assign( const JsonTextSerializableType<T>& json_tobj ) {
        this->key = json_tobj.key;
        this->value = json_tobj.value;
        return *this;
    }
    JsonTextSerializableType<T>& assign( const JsonValue& json_value ) {
        this->key = json_value.first;
        this->value = json_value.second;
        return *this;
    }

    template< typename R, typename P >
    R operator=( const P param ) {
        return this->assign(param);
    }

    JsonTextSerializableType<T>& operator=( const JsonTextSerializableType<T>& json_tobj ) {
        return this->assign( json_tobj );
    }

    JsonTextSerializableType<T>& operator=( const JsonValue& json_value ) {
        return this->assign( json_value );
    }
};


#define JSONCPP_TYPE(cls) \
    template<> \
    class JsonTextSerializableType<cls>; \


namespace types {
namespace detail {

} // namespace detail
// value = false / null / true / object / array / number / string


template< class JT, typename BT >
class JsonType
    :   public JsonTextSerializableType< BT >
{
protected:
    JT* m_jsontype;

public:
    JsonType( const char* keystr, const BT& valueref, JT* inheriting_type  )
        :   JsonTextSerializableType< BT >( keystr, valueref )
        ,   m_jsontype( inheriting_type )
    {}
    JsonType( const char* keystr, const BT value, JT* inheriting_type )
        :   JsonTextSerializableType< BT >( keystr, *(new BT(value)) )
        ,   m_jsontype( inheriting_type )
    {}
    JsonType( const std::string keystr, const BT value, JT* inheriting_type )
        :   JsonTextSerializableType< BT >( keystr.c_str(), *(new BT(value))  )
        ,   m_jsontype( inheriting_type )
    {}

    static BT& toBaseType( const JT& json_type_value ) {
        return typeToType< JT, BT >( json_type_value );
    }

    BT& toBaseType() const {
        return this->value;
    }

    static JT& toJsonType( const BT& base_type_value ) {
        return typeToType< BT, JT >( base_type_value );
    }

    static JT& toJsonType( const BT base_type_value ) {
        BT* bt = new BT( base_type_value );
        return toJsonType( *bt );
    }

    JT& toJsonType() const {
        return toJsonType( this->value );
    }

    virtual const std::string str() const {
        return JsonSerializableObject::toStringValue< BT >( toBaseType() );
    }
    virtual const char* c_str() const {
        return str().c_str();
    }

    virtual JT& operator=( const JsonType<JT, BT>& json_type ) {
        this->key = json_type.key;
        this->value = json_type.value;
        return *(this->m_jsontype);
    }

    JsonType<JT, BT>& assign( const JsonType<JT, BT>& json_type_value ) {
        JsonType<JT, BT>* jt = new JsonType<JT, BT>( json_type_value.key, json_type_value.value );
        return *jt;
    }

    template< typename R, typename P >
    friend R assign( const P param );

    template< typename R, typename P >
    BT& assign( const JT& json_type_value ) {
        return toBaseType( json_type_value );
    }

    template< typename R, typename P >
    BT& assign( const JsonType<JT, BT> json_type ) {
        return json_type.value;
    }

    template< typename R, typename P >
    JT& assign( const JsonType<JT, BT>& json_type ) {
        this->key = json_type.key;
        this->value = json_type.value;
        return toJsonType( json_type );
    }

    template< typename R, typename P >
    JT& assign( const BT& base_type_value ) {
        this->value = toJsonType( base_type_value );
        return toJsonType();
    }

    template< typename R, typename P >
    JT& assign( const BT base_type_value ) {
        this->value = toJsonType( base_type_value );
        return toJsonType();
    }

    using JsonTextSerializableType< BT >::assign;

    template< typename R, typename P >
    R operator=( const P param ) {
        return this->assign(param);
    }
    using JsonTextSerializableType< BT >::operator=;

    JsonType<JT, BT>& operator=( JT& json_type_value ) {
        return this->operator=<>( json_type_value );
    }

    BT& operator=( const JT& json_type_value ) {
        return this->operator=<>( json_type_value );
    }

    /*BT& operator=( const JsonType<JT, BT> json_type ) {
        return this->operator=<>( json_type );
    }

    JT& operator=( const JsonType<JT, BT>& json_type ) {
        return this->operator=<>( json_type );
    }*/

    JT& operator=( const BT& base_type_ref ) {
        return this->operator=<>( base_type_ref );
    }

    JT& operator=( const BT base_type_value ) {
        return this->operator=<>( base_type_value );
    }

    void operator>>( BT base_type_value ) const {
        base_type_value = this->value;
    }

    void operator>>( BT& base_type_ref ) const {
        base_type_ref = this->value;
    }

    void operator>>( JT& json_type_ref) const {
        json_type_ref = *(new JT(this->key, this->value));
    }
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
