
#pragma once

#include <initializer_list>
#include <list>
#include <ranges>
#include <string>
#include <string_view>

#include "json_types.hpp"
#include "json_serializer.hpp"


/** @brief The namespace of the library JsonCPP */
namespace jsoncpp {

// Forward declaration
class JsonObjectView;


/**
 * @brief The object-view model for lists of `JsonObjectView` pointers
 */
class JsonListView
    :   protected std::list< JsonObjectView* >
{
public:
    /** @brief The native C++ type, which this list view has inherited of */
    typedef typename std::list< JsonObjectView* > jsonobjectview_list_t;

    /**
     * @brief Constructor.
     */
    JsonListView();

    /**
     * @brief Find an json object by its key inside of this list.
     * @param key The key string of the searched list item.
     * @returns A `JsonObjectView` item pointer or `nullptr`, if the item was not founnd
     */
    JsonObjectView* findKey( const char* key );
    /**
     * @brief Find an json object by its key inside of this list.
     * @param key The key string of the searched list item.
     * @returns A `JsonObjectView` item reference.
     * @note If the item was not found, a new item is created for the key string.
     */
    JsonObjectView& findKeyRef( const char* key );

    /**
     * @brief Find an json object by its key path inside of this list.
     * @param key The key string of the searched list item.
     * @returns A `JsonObjectView` item pointer.
     * @note If the item was not found, a new item is created for the key string.
     * @note You may call this operator like this: `json["root_key.sub.sub"] << "A sample string.";`.
     */
    JsonObjectView* operator[]( const char* key );
    /**
     * @brief Find an json object by its key inside of this list.
     * @param key The key string of the searched list item.
     * @returns A `JsonObjectView` item pointer.
     * @note If the item was not found, a new item is created for the key string.
     * @note You may call this operator like this: `json["root_key.sub.sub"] << "A sample string.";`.
     */
    const JsonObjectView* operator[]( const char* key ) const;

    /**
     * @brief Add a new `JsonObjectView` to this list.
     * @param json_obj A pointer to the added-to-be json object.
     * @returns A reference to this `JsonListView` object.
     */
    JsonListView& operator<<( JsonObjectView* json_obj );
    /**
     * @brief Add a new `JsonObjectView` to this list.
     * @param json_obj A reference to the added-to-be json object.
     * @returns A reference to this `JsonListView` object.
     */
    JsonListView& operator<<( JsonObjectView& json_obj );
    /**
     * @brief Add another `jsonobjectview_list_t` to this list.
     * @param json_obj A pointer to the added-to-be json object list.
     * @returns A reference to this `JsonListView` object.
     */
    JsonListView& operator<<( std::list< JsonObjectView* >* json_obj_lst );

    /**
     * @brief Copy this json list view to an other native C++ list with `JsonObjectView` pointers.
     * @param other The empty other list without or with elements inside.
     * @returns A reference to this `JsonListView` object.
     */
    const JsonListView& operator>>( std::list< JsonObjectView* >& other ) const;
    /**
     * @brief Copy this json list view to a `types::Arrey`.
     * @param other The empty other array without or with elements inside.
     * @returns A reference to this `JsonListView` object.
     */
    const JsonListView& operator>>( types::Array& other ) const;
    /**
     * @brief Copy this json list view to an `JsonValue` with one key and one value string.
     * @param other The empty or containing `JsonValue`.
     * @returns A reference to this `JsonListView` object.
     */
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

    /**
     * @brief Get this `JsonListView` as native C++ string.
     * @param base_indent The number of prefixing spaces at the level of this `JsonListView`
     * @returns This as a `std::string`.
     */
    std::string toString( const unsigned int base_indent = 0 ) const;
    /**
     * @brief Get this `JsonListView` as native C++ string.
     * @param base_indent The number of prefixing spaces at the level of this `JsonListView`
     * @returns This as a `const char*`.
     */
    
    const char* c_str( const unsigned int base_indent = 0 ) const;

    /**
     * @brief Construct a list from a list value
     * @param str The value with a key-value list
     * @returns A `JsonListView` pointer-to-object.
     * @note A key-value list is surrounded by braces `{...}` in json.
     */
    static JsonListView* fromString( const char* str );

    /**
     * @brief Get all key strings in this `JsonListView` as `std::initializer_list`
     * @returns All key strings of the values in this list view.
     */
    std::initializer_list<std::string>& keys() const noexcept;
};


/**
 * @brief The object-view model for json key-value objects and key-list objects.
 */
class JsonObjectView
    :   public JsonValue
{
protected:
    /**
     * @brief All children of this `JsonObjectView` as pointer to a `JsonListView`
     */
    JsonListView* m_children;

public:
    /**
     * @brief Constructor.
     * @param json A `JsonValue` to construct this `JsonObjectView` of
     */
    JsonObjectView( const JsonValue& json );
    /**
     * @brief Constructor.
     * @param key The key string of the value
     * @param value The json value as string
     */
    JsonObjectView( const char* key, const char* value );
    /**
     * @brief Constructor.
     * @param keyvalue_pair A key-value pair delimited with ": " and quotation marks.
     */
    JsonObjectView( const char* keyvalue_pair );

    /**
     * @brief Find an json object by its key inside of this json object.
     * @param key The key string of the searched list item.
     * @returns A `JsonObjectView` item pointer.
     * @note If the item was not found, a new item is created for the key string.
     * @note You may call this operator like this: `json["root_key.sub.sub"] << "A sample string.";`.
     */
    JsonObjectView* operator[]( const char* key );
    /**
     * @brief Find an json object by its key inside of this json object.
     * @param key The key string of the searched list item.
     * @returns A `JsonObjectView` item pointer.
     * @note If the item was not found, a new item is created for the key string.
     * @note You may call this operator like this: `json["root_key.sub.sub"] << "A sample string.";`.
     */
    const JsonObjectView* operator[]( const char* key ) const;

    /**
     * @brief Get this `Json` as native C++ string.
     * @param base_indent The number of prefixing spaces at the level of this `Json`
     * @returns This as a `std::string`.
     */
    const std::string toString( const unsigned int base_indent = 0 ) const;

    /**
     * @brief Get the key of this `Json` object as `std::string`.
     */
    std::string key() const;
    /**
     * @brief Get the key of this `Json` object as `std::string`.
     */
    std::string value() const;
    /**
     * @brief Get the children as pointer to a `JsonListView`
     */
    JsonListView* children() noexcept;
    /**
     * @brief Get the children as pointer to a `JsonListView`
     */
    const JsonListView* children() const noexcept;

    /**
     * @brief Iterator for this type.
     */
    typedef typename JsonListView::iterator iterator;
    /**
     * @brief Const iterator for this type.
     */
    typedef typename JsonListView::const_iterator const_iterator;
    /**
     * @brief The begin of the `JsonListView` of all children of this `JsonValue` object
     */
    iterator begin() noexcept;
    /**
     * @brief The begin of the `JsonListView` of all children of this `JsonValue` object
     */
    const_iterator begin() const noexcept;
    /**
     * @brief The end of the `JsonListView` of all children of this `JsonValue` object
     */
    iterator end() noexcept;
    /**
     * @brief The end of the `JsonListView` of all children of this `JsonValue` object
     */
    const_iterator end() const noexcept;

    /**
     * @brief The size of the `JsonListView` of all children of this `JsonValue` object
     */
    size_t size() const;
    /**
     * @brief Check, if there are children of this `Json` object view
     */
    bool empty() const noexcept;
    /**
     * @brief Clear the list of children of this `Json` value view
     */
    void clear() noexcept;

    /**
     * @brief Serialize or give the key string and the value of this `JsonValue` to an C++ type `T`
     * @tparam `T` The C++ type or class to give the value to
     * @param json_class The C++ type or class reference where this `JsonValue` view is to be handed in and set.
     * @returns A reference to this `Json` object view.
     * @note Use like `json >> json_cls;`
     */
    template< typename T >
        requires std::is_base_of< JsonValue, T >::value
    JsonObjectView& operator>>( T& json_class ) {
        json_class.first = this->first;
        json_class.second = this->second;
        return *this;
    }

    /**
     * @brief Serialize or give the key string and the value of this `JsonValue` to an C++ type `T`
     * @tparam `T` The C++ type or class to give the value to
     * @param json_type The C++ type or class reference where this `JsonValue` view is to be handed in and set.
     * @returns A reference to this `Json` object view.
     * @note Use like `json >> json_type;`
     */
    template< typename T >
    JsonObjectView& operator>>( JsonTextSerializableType<T>& json_type ) {
        json_type.key = this->first;
        json_type.value = stringToType<T>( this->second );
        return *this;
    }
    /**
     * @brief Serialize or give the key string and the value of this `JsonValue` to an C++ type `T`
     * @tparam `T` The C++ type or class to give the value to
     * @tparam `BT` The`base type of the `JsonType< T, BT >`
     * @param json_type The C++ type or class reference where this `JsonValue` view is to be handed in and set.
     * @returns A reference to this `Json` object view.
     * @note Use like `json >> json_type;`
     */
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
    /**
     * @brief Add a `JsonValue` to the children of this json view
     * @param json_value A reference to a json value
     * @returns A reference to the new `JsonObjectView`
     */
    JsonObjectView& operator<<( const JsonValue& json_value );
    /**
     * @brief Add a `JsonValue` to the children of this json view
     * @param json_value A pointer to a json value
     * @returns A reference to the new `JsonObjectView`
     */
    JsonObjectView& operator<<( const JsonValue* json_value );
    /**
     * @brief Add a custom type inherited from `types::JsonType< JT, BT >` to the children of this json view
     * @tparam `JT` The Json type of the own `types::JsonType< JT, BT >` text serializable json type.
     * @tparam `BT` The base type of the own `types::JsonType< JT, BT >` text serializable json type.
     * @note The value `T& value` of the inherited `JsonTextSerializable<BT>` is `BT` for more easy definition of
     *      the cusom `typeToString<BT>` function.
     * @param json_value A reference to a json value
     * @returns A reference to the new `JsonObjectView`
     */
    template< typename JT, typename BT >
        requires std::is_base_of< types::JsonType< JT, BT >, JT >::value
    JsonObjectView& operator<<( const JT& custom_json_type ) {
        JsonObjectView* json_obj = new JsonObjectView( custom_json_type.key.c_str()
                                , typeToString< BT >( custom_json_type.value )
        );
        json_obj->first = custom_json_type.first;
        json_obj->second = typeToString< BT >( custom_json_type.second );
        this->children()->push_front( json_obj );
        return *json_obj;
    }

    /**
     * @brief A static greater function for comparison of two `JsonObjectView` references by their key strings.
     * @returns`True, if `rhs` is greater than `lhs`. Compares the key strings.
     */
    static bool greater( const JsonObjectView& lhs, const JsonObjectView& rhs );
    /**
     * @brief A static greater function for comparison of two `JsonObjectView` pointers by their key strings.
     * @returns`True, if `rhs` is greater than `lhs`. Compares the key strings.
     */
    static bool greater_ptr( const JsonObjectView* lhs, const JsonObjectView* rhs );
    /**
     * @brief C++20-conform comparison operator for the use in native C++ `std::list` objects and in other comparisons
     * @returns A value of `std::strong_ordering`, and thus `std::strong_ordering::equivalent` only, if the value is also equal.
     */
    std::strong_ordering operator<=>( const JsonObjectView& other );
    /**
     * @brief C++20-conform comparison operator for the use in native C++ `std::list` objects and in other comparisons
     * @returns A value of `std::strong_ordering`, and thus `std::strong_ordering::equivalent` only, if the value is also equal.
     */
    std::strong_ordering operator<=>( const JsonObjectView* other );

    /**
     * @brief Read a JsonValue from a string
     * @param keyvalue_pair A key value pair with the syntax `"name": "value"`.
     * @returns A pointer to a `JsonValue` on success
     */
    static JsonValue* fromStringJsonValue( const char* keyvalue_pair );
    /**
     * @brief Read a `JsonObjectView` from a string
     * @param str The string to parse, starting and ending with braces like `{...}`
     * @returns A pointer to a `JsonObjectView` on success
     */
    static JsonObjectView* fromString( const char* str );
};


/**
 * @brief The class containing an json object enclosed by braces `{...}`.
 */
class Json
    :   public JsonListView
{
protected:
    

public:
    /**
     * @brief Constructor.
     */
    Json();
    /**
     * @brief Constructor. Reads an json object from string
     * @param str The string to parse, starting and ending with braces like `{...}`
     */
    Json( const char* str );

    /**
     * @brief Search this json object for a named `JsonObjectView`
     * @param key The key string to search for
     * @returns A reference to a `JsonObjectView` which is referenced by `key` in this 
     *      json object.
     */
    JsonObjectView& at( const char* key );
    /**
     * @brief Search this json object for a named `JsonObjectView`
     * @param key The key string to search for
     * @returns A reference to a `JsonObjectView` which is referenced by `key` in this 
     *      json object.
     */
    const JsonObjectView& at( const char* key ) const;
    /**
     * @brief Search this json object for a named `JsonObjectView`
     * @param key The key or name path string to search for
     * @returns A reference to a `JsonObjectView` which is referenced by `key` in this 
     *      json object.
     * @note Name path are the hierarchical keys of the json objects/values separated by '.'
     */
    JsonObjectView& operator[]( const char* key );
    /**
     * @brief Search this json object for a named `JsonObjectView`
     * @param key The key or name path string to search for
     * @returns A reference to a `JsonObjectView` which is referenced by `key` in this 
     *      json object.
     * @note Name path are the hierarchical keys of the json objects/values separated by '.'
     */
    const JsonObjectView& operator[]( const char* key ) const;

    /**
     * @brief Add a json value to this json object
     * @param json_value The reference to the json value
     * @return A reference to `this`
     */
    Json& operator<<( const JsonValue& json_value );
    /**
     * @brief Add a json value to this json object
     * @param json_value The pointer to the json value
     * @return A reference to `this`
     */
    Json& operator<<( const JsonValue* json_value );
    /**
     * @brief Add a complete json object view to this json object
     * @param json_obj The pointer to the json object view
     * @return A reference to `this`
     */
    Json& operator<<( JsonObjectView* json_obj );
    /**
     * @brief Add a complete list of `JsonObjectView` pointers to this json object
     * @param json_obj_lst The reference to the list
     * @return A reference to `this`
     */
    Json& operator<<( std::list< JsonObjectView* >& json_obj_lst );
    /**
     * @brief Add a custom json value to this json object
     * @tparam The type of the custom json value
     * @param custom_json_type The reference to the json value
     * @return A reference to `this`
     * @note The custom type `JT` must be re-constructible with 
     *      `new JsonObjectView( *(custom_json_type.toJson()) )`.
     */
    template< typename JT >
    Json& operator<<( const JT& custom_json_type ) {
        *this << new JsonObjectView( *(custom_json_type.toJson()) );
        return *this;
    }

    /**
     * @brief Create a json object from string
     * @param str A json object ending and starting with braces `{...}`
     * @returns A pointer to a json object `Json`
     */
    static Json* fromString( const char* str );
    /**
     * @brief Read a json object to string
     * @param json The pointer to a json object that is to be stringified
     * @returns A string starting and ending with braces with values inside (`{...}`)
     */
    static std::string toString( const Json* json );
};

} // namespace jsoncpp
