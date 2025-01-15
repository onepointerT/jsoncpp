
#pragma once

#include <initializer_list>
#include <string>

#include "json_detail.hpp"
#include "json_objects.hpp"
#include "json_types.hpp"

namespace jsoncpp {




template< typename... Types, class T >
concept is_json_serializable = requires ( T t, Types... ts ) {
    std::is_base_of< JsonSerializable< Types... >, T >::value;
};


template< typename... Types, class T >
    requires is_json_serializable< Types..., T >
const char* typeToString( T& json_serializable_obj );


template< typename... Types, class T >
    requires is_json_serializable< Types..., T >
T& stringToType( const char* json_serializable_txt );




template< typename... Types >
class JsonSerializable
    :   protected detail::RefListNamed< Types... >
    ,   public types::JsonType< JsonSerializable< Types... >, std::string >
{
protected:
    virtual const char* toString() const {
        return typeToString<Types..., JsonSerializable< Types... >>(*this);
    }

    virtual std::string toBaseType() {
        return typeToString<Types..., JsonSerializable< Types... >>(*this);
    }

    virtual JsonValue* toJson() const {
        return JsonTextSerializableType< std::string >::toJson();
    }

    static JsonSerializable< Types... >& toJsonType( const JsonValue& json_value
                                                        , JsonSerializable< Types... > json_serializable
    ) {
        // Check type
        std::any* a = new std::any( json_serializable.get(json_value.first) );
        // Set value
        json_serializable.emplace( json_value.first, jsoncpp::stringToType<decltype(a->type())>(json_value.second) );
        return json_serializable;
    }

    static JsonSerializable< Types... >& toJsonType( const JsonObjectView& jov ) {
        JsonSerializable< Types... > json_serializable = new JsonSerializable< Types... >(jov.children()->keys());

        for ( JsonObjectView* jovc : *jov.children() ) {
            /*JsonValue* json_value = new JsonValue( jovc->key(), jovc->value() );
            toJsonType( json_value, json_serializable );*/
            json_serializable.emplace( jovc->key(), jsoncpp::stringToType<decltype(a->type())>(json_value.second) );
        }

        json_serializable.key = jov.key();
        json_serializable.value = jov.value();

        return json_serializable;
    }
    
    // TODO: operator=

public:
    JsonSerializable( std::initializer_list il )
        :   detail::RefListNamed< Types... >( detail::newRefList(il) )
        ,   types::JsonType< JsonSerializable< Types... >, std::string >()
    {}

    template< typename R, typename P >
    virtual R assign( const P param );

    template< typename R, typename P >
    virtual R assign( const char* key, const P param ) {
        (*this)[key] = param;
    }
};


template< typename... Types, class T >
    requires is_json_serializable< Types..., T >
const char* typeToString( T& json_serializable_obj ) {

}


template< typename... Types, class T >
    requires is_json_serializable< Types..., T >
T& stringToType( const char* json_serializable_txt ) {

}



} // namespace jsoncpp
