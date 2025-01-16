
#pragma once

#include <list>
#include <string>
#include <string_view>

#include "json_objects.hpp"
#include "json_serializer.hpp"
#include "json_types.hpp"
#include "regex.hpp"


namespace jsoncpp {

JsonObjectView* findByKeyPath( const char* key_path, JsonObjectView* jsonobj );
JsonObjectView* findByKeyPath( const char* key_path, JsonListView* jsonlist );
JsonObjectView* findByKeyPath( const char* key_path, Json* json );

namespace regex {

make_regex(jsonstr)             "\"[\s\w\d]\"";
make_regex(jsondelim)           "\:\ ";
make_regex(jsonobj)             "\{\n[\w\s\d\:\"\n]+\n\}";
make_regex(jsonkv)              jsonstr + "\n?" + jsondelim + jsonstr + ",\n";


} // namespace regex
namespace syntax {

std::string make_indent( const unsigned int indent_size );
unsigned int count_indent( const char* str, size_t pos = 0 );
std::string_view of_indent( const char* str, const unsigned int indent_size, size_t pos = 0 );
std::string_view of_until( const char* str, const char* delim = "\n", size_t pos = 0 );
std::string_view of_since( const char* str, const char* delim = "\n", size_t pos = 0 );
std::list< std::string_view >& tokenize( const char* str, const char* delim = ",", size_t pos = 0 );

using SerializableTextType = JsonValue;
using SerializableType = JsonTextSerializableType< std::string >;

JsonValue* parseJsonKeyValue( const std::string jsonkvstr );
JsonObjectView* findJsonObject( const SerializableTextType* json );
std::list< JsonObjectView* >* findJsonObjects( const std::string jsonstr );
Json* findJson( const std::string jsonstr );



} // namespace syntax
} // namespace jsoncpp