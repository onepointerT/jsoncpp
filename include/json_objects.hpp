
#pragma once

#include <initializer_list>
#include <list>
#include <ranges>
#include <string>
#include <string_view>

#include "json_types.hpp"


namespace jsoncpp {


class JsonObjectView;



class JsonListView
    :   protected std::list< JsonObjectView* >
{
public:
    typedef typename std::list< JsonObjectView* > jsonobjectview_list_t;

    JsonListView();

    JsonObjectView* findKey( const char* key ) const;
    JsonObjectView& findKeyRef( const char* key );

    JsonObjectView* operator[]( const char* key );
    const JsonObjectView* operator[]( const char* key ) const;

    JsonListView& operator<<( JsonObjectView* json_obj );
    JsonListView& operator<<( JsonObjectView& json_obj );
    JsonListView& operator<<( std::list< JsonObjectView* >* json_obj_lst );

    const JsonListView& operator>>( std::list< JsonObjectView* >& other ) const;
    const JsonListView& operator>>( types::Array& other ) const;
    const JsonListView& operator>>( JsonValue& other ) const;

    using std::list< JsonObjectView* >::merge;
    using std::list< JsonObjectView* >::sort;
    using std::list< JsonObjectView* >::splice;

    using std::list< JsonObjectView* >::const_iterator;
    using std::list< JsonObjectView* >::iterator;
    using std::list< JsonObjectView* >::begin;
    using std::list< JsonObjectView* >::end;
    using std::list< JsonObjectView* >::size;

    using std::list< JsonObjectView* >::erase;
    using std::list< JsonObjectView* >::push_back;
    using std::list< JsonObjectView* >::remove;
    using std::list< JsonObjectView* >::remove_if;
    using std::list< JsonObjectView* >::unique;

    using std::list< JsonObjectView* >::empty;
    using std::list< JsonObjectView* >::clear;

    std::string toString( const unsigned int base_indent = 0 ) const;
    const char* c_str( const unsigned int base_indent = 0 ) const;

    static JsonListView* fromString( const char* str );

    std::initializer_list<std::string>& keys() const noexcept;
};


class JsonObjectView
    :   public JsonValue
{
protected:
    JsonListView* m_children;

public:
    JsonObjectView( const JsonValue& json );
    JsonObjectView( const char* key, const char* value );
    JsonObjectView( const char* keyvalue_pair );

    const std::string toString( const unsigned int base_indent = 0 ) const;

    std::string key() const;
    std::string value() const;
    JsonListView* children() noexcept;
    const JsonListView* children() const noexcept;

    typedef typename JsonListView::iterator iterator;
    typedef typename JsonListView::const_iterator const_iterator;
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;

    size_t size() const;
    bool empty() const noexcept;
    void clear() noexcept;

    template< typename T > requires std::is_base_of< JsonValue, T >::value
    JsonObjectView& operator>>( T& json_class ) {
        json_class.first = this->first;
        json_class.second = this->second;
        return *this;
    }

    template< typename T >
    JsonObjectView& operator>>( JsonTextSerializableType<T>& json_type ) {
        json_type.key = this->first;
        json_type.value = this->second;
        return *this;
    }
    template< typename T, typename BT >
    JsonObjectView& operator>>( types::JsonType< T, BT >& json_type ) {
        T& tobj = typeToType< JsonObjectView, T >( *this );
        json_type = typeToType< T, types::JsonType<T, BT> >( tobj );
        return *this;
    }
    /**
     * @note Define an `T& operator<<(JsonObject)` for custom types.
     */
    template< typename JT, typename BT >
        requires std::is_base_of< types::JsonType< JT, BT >, JT >::value
    JsonObjectView& operator>>( JT& custom_json_type ) {
        custom_json_type << *this;
        return *this;
    }
/*
    JsonObjectView& operator>>( types::Bool& json_type );
    JsonObjectView& operator>>( types::Null& json_type );
    JsonObjectView& operator>>( types::String& json_type );
    JsonObjectView& operator>>( types::Object& json_type );
    JsonObjectView& operator>>( types::Array& json_type );
    template< types::detail::json_number N >
    JsonObjectView& operator>>( types::Number< N >& json_type );
*/
    JsonObjectView& operator<<( const JsonValue& json_value );
    JsonObjectView& operator<<( const JsonValue* json_value );
    template< typename JT, typename BT >
        requires std::is_base_of< types::JsonType< JT, BT >, JT >::value
    JsonObjectView& operator<<( const JT& custom_json_type );

    static bool greater( const JsonObjectView& lhs, const JsonObjectView& rhs );
    static bool greater_ptr( const JsonObjectView* lhs, const JsonObjectView* rhs );
    std::strong_ordering operator<=>( const JsonObjectView& other );
    std::strong_ordering operator<=>( const JsonObjectView* other );

    static JsonValue* fromStringJsonValue( const char* keyvalue_pair );
    static JsonObjectView* fromString( const char* str );
};


class Json
    :   public JsonListView
{
protected:
    

public:
    Json();
    Json( const char* str );

    JsonObjectView& at( const char* key );
    const JsonObjectView& at( const char* key ) const;
    JsonObjectView& operator[]( const char* key );
    const JsonObjectView& operator[]( const char* key ) const;

    Json& operator<<( const JsonValue& json_value );
    Json& operator<<( const JsonValue* json_value );
    Json& operator<<( JsonObjectView* json_obj );
    Json& operator<<( std::list< JsonObjectView* >& json_obj_lst );
    template< typename JT >
    Json& operator<<( const JT& custom_json_type ) {
        *this << new JsonObjectView( *(custom_json_type.toJson()) );
        return *this;
    }

    static Json* fromString( const char* str );
    static std::string toString( const Json* json );
};

//JSONCPP_CLASS(JsonValue)

} // namespace jsoncpp
