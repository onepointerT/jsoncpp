
#pragma once

#include <list>
#include <string>
#include <string_view>

#include "json_objects.hpp"
#include "json_serializer.hpp"
#include "json_types.hpp"
#include "regex.hpp"


namespace jsoncpp {

/**
 * @brief Find a `JsonObjectView` by it's key path, e.g. `root.sub1.sub2`. If the value is not there
 *      it is created for you 
 * @param key_path Key path, separated by dots, or single key as string
 * @param jsonobj The pointer to a `JsonObjectView` to search in
 * @returns A pointer to a `JsonObjectView` that was thus found or created for `key_path` of `jsonobj`
 */
JsonObjectView* findByKeyPath( const char* key_path, JsonObjectView* jsonobj );
/**
 * @brief Find a `JsonObjectView` by it's key path, e.g. `root.sub1.sub2`. If the value is not there
 *      it is created for you 
 * @param key_path Key path, separated by dots, or single key as string
 * @param jsonlist The pointer to a `JsonListView` to search in
 * @returns A pointer to a `JsonObjectView` that was thus found or created for `key_path` of `jsonobj`
 */
JsonObjectView* findByKeyPath( const char* key_path, JsonListView* jsonlist );
/**
 * @brief Find a `JsonObjectView` by it's key path, e.g. `root.sub1.sub2`. If the value is not there
 *      it is created for you 
 * @param key_path Key path, separated by dots, or single key as string
 * @param json The pointer to a `Json` object to search in
 * @returns A pointer to a `JsonObjectView` that was thus found or created for `key_path` of `jsonobj`
 */
JsonObjectView* findByKeyPath( const char* key_path, Json* json );


/** @brief Some useful regular expressions that can be used with `Match` and `Matchall` (POSIX regexp). */
namespace regex {

make_regex(jsonstr)             "\"[\s\w\d]\"";
make_regex(jsondelim)           "\:\ ";
make_regex(jsonobj)             "\{\n[\w\s\d\:\"\n]+\n\}";
make_regex(jsonkv)              jsonstr + "\n?" + jsondelim + jsonstr + ",\n";


} // namespace regex


/** @brief Everything on json syntax strings and parsing of json object view model */
namespace syntax {

/**
 * @brief Make an indent (concatenated spaces, ' ') of the size `indent_size`
 */
std::string make_indent( const unsigned int indent_size );
/**
 * @brief Count the size of the prefixed indent next to the line at `pos`
 */
unsigned int count_indent( const char* str, size_t pos = 0 );
/**
 * @brief Get all lanes in `str` with an indent of the size of `indent_size` next to `pos`
 */
std::string_view of_indent( const char* str, const unsigned int indent_size, size_t pos = 0 );
/**
 * @brief Get everything in `str` until `delim` beginning at `pos`
 */
std::string_view of_until( const char* str, const char* delim = "\n", size_t pos = 0 );
/**
 * @brief Get everything in `str` since `delim` reversly beginning at `pos`
 */
std::string_view of_since( const char* str, const char* delim = "\n", size_t pos = 0 );
/**
 * @brief Get all parts in `str` per found `delim` beginning at `pos`
 */
std::list< std::string_view >& tokenize( const char* str, const char* delim = ",", size_t pos = 0 );

using SerializableTextType = JsonValue;
using SerializableType = JsonTextSerializableType< std::string >;

/**
 * @brief Get a key/value string of json as pointer to a `JsonValue`
 */
JsonValue* parseJsonKeyValue( const std::string jsonkvstr );
/**
 * @brief Find the json object `JsonObjectView` inside of a value part of a `JsonValue`
 */
JsonObjectView* findJsonObject( const JsonValue* json );
/**
 * @brief Get all json objects in a json string, possibly ending and beginning with braces `{...}`, as list
 */
std::list< JsonObjectView* >* findJsonObjects( const std::string jsonstr );
/**
 * @brief Parse a json object or array
 */
Json* findJson( const std::string jsonstr );



} // namespace syntax
} // namespace jsoncpp