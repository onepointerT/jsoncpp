
#pragma once

#include <initializer_list>
#include <string>

#include "json_detail.hpp"
#include "json_objects.hpp"
#include "json_types.hpp"

namespace jsoncpp {

// Forward declaration
template< typename... Types >
class JsonSerializable;


/** @brief A class is serializable with `jsoncpp` when it inherits `JsonSerializable<Types...>` or `types::JsonType<YourType, YourValueType>`.*/
template< typename... Types, class T >
concept is_json_serializable = requires ( T t, Types... ts ) {
    std::is_base_of< JsonSerializable< Types... >, T >::value;
};


/**
 * @brief The json-serializable type is needed to be formed as string. It is needed to write
 *      functions for `T& value` of `JsonTextSerializable<T>` so that `T` can be formed to strings.
 * @param json_serializable_obj Your serializable
 */
template< typename... Types, class T >
    requires is_json_serializable< Types..., T >
const char* typeToString( T& json_serializable_obj );


/**
 * @brief The json-serializable type is needed to be formed from string. It is needed to write
 *      functions for `T& value` of `JsonTextSerializable<T>` so that `T` can be formed from strings.
 * @param json_serializable_txt The serializable json text
 */

template< typename... Types, class T >
    requires is_json_serializable< Types..., T >
T& stringToType( const char* json_serializable_txt );



/**
 * @brief The generic serializable type of this library where you can inherit from
 * @tparam Types... The types the json serializable type will have
 * @note It is needed to implement the following free functions for each custom type `T`:
 *      * `typeToString< Types..., T >()`
 *      * `stringToType< Types..., T >()`
 *      * `typeToType< T, types::JsonType< JsonSerializable< Types..., std::string > >()`
 *      * `typeToType< types::JsonType< JsonSerializable< Types..., std::string >, T >()`
 *      * `JsonSerializable< Types... >.toJsonStr()`
 *      * some virtual and operator functions from `types::JsonType< T, std::string >` and derived classes
 *              as needed
 * @note More than one variable attributes of a type class could and should inherit for
 *      serialization from `JsonSerializable<Types...>`,
 *      e.g. inherits `JsonSerializable<types::Bool, types::Array, bool, JsonListView>` means,
 *          that your serializable type has 4 attributes of the types above with the usage of
 *      * each type needs to be transformable from and to string
 *      * each type needs to be transformable with the operators (at least `typeToType<YourType, JsonSerializable<...>>()` defined)
 *      * each of the 4 types needs to have a protected or public `get_reference` function
 *      * more definitions to your needings.
 */
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
            JsonValue* json_value = new JsonValue( jovc->key(), jovc->value() );
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
    return json_serializable_obj.toJsonStr().
}


template< typename... Types, class T >
    requires is_json_serializable< Types..., T >
T& stringToType( const char* json_serializable_txt ) {

}



} // namespace jsoncpp
