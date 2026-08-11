# jsoncpp

##### A data handling and serialization library for JSON

###### v1.6.2

<p align="center"><img width="100" src="https://github.com/onepointerT/jsoncpp/blob/dev/doc/jsoncpp.png" alt="JsonC++ logo"></p>

The library allows to read and write files in JavaScriptObjectNotation (`*.json`), to access the values and define them in a ObjectView model with commonly useful C++ operators (see examples below).

It is possible to serialize the values into the standard C/C++ types and even customly developed types or classes via inheritance.

###### TODO: documentation

#### How to use

One might have a `ddml.json` document such like

```json
{
    "ddml": {
        "varname1": {
            "sql": "SELECT * FROM table;",
            "dstruct": "TableExampleElement"
        },
        "varname2": {
            "sql": "SELECT * FROM table WHERE ID = 1000;",
            "cacao": "#varname1{$sql}{\s\w}#varname2",
            "dstruct": "TableExampleElement"
        },
        "varname3": "SELECT * FROM table WHERE wordset = varname3;"
    }
}
```

and try to access it. So we have the following to do to use it with JsonC++:

```c++
#include <json.hpp>

using namespace jsoncpp;

// Read the document
Json json = *JsonDocument::fromFile("ddml.json");
// Get the value ddml
JsonObjectView& ddml = json["ddml"];
// Add a fourth variable
// (To use the key/value-path extension you need to use `operator[]`)
ddml["varname4"] << new JsonObjectView( "varname4", "{}" );
ddml["varname4.sql"] << "SELECT * FROM table2 WHERE wordset = varname3;";
ddml["varname4.dstruct"] << "TableExampleUserElement";
ddml["varname4.serialize"] << false;
// Or edit the first variable
ddml["varname1.dstruct"] = "TableExampleElementAll";
ddml["varname1.serialize"] << true;
// Write all changes to the file
JsonDocument::toFile("ddml.json", &json);
```

If we want to look at all changes, we might pass the `toString()` function to `std::cout`.

```c++
#include <iostream>

std::cout << "The current 'ddml.varname4' value is: " << ddml["varname4"].toString()
            << std::endl << std::endl;

std::cout << "The whole document would have the following content: " << std::endl 
            << json.toString() << std::endl;
```

Which then prints the following:

```console
$> The current 'ddml.varname4' value is: "varname4": {
    "sql": "SELECT * FROM table2 WHERE wordset = varname3;",
    "dstruct": "TableExampleUserElement"
  }
$> The whole document would have the following content: 
```

```json
{
    "ddml": {
        "varname1": {
            "sql": "SELECT * FROM table;",
            "dstruct": "TableExampleElementAll"
        },
        "varname2": {
            "sql": "SELECT * FROM table WHERE ID = 1000;",
            "cacao": "#varname1{$sql}{\s\w}#varname2",
            "dstruct": "TableExampleElement"
        },
        "varname3": "SELECT * FROM table WHERE wordset = varname3;",
        "varname4": {
            "sql": "SELECT * FROM table2 WHERE wordset = varname3;",
            "dstruct": "TableExampleUserElement"
        }
    }
}
```

Then we might want to have their values to `C++`, this is simply extensible with the in-library types or the default types of `C++`:

```c++
// Define the variables
std::string str1;
bool serialize1;
types::Bool json_bool;
std::string str3;

// Put values from Json into the variables
json["ddml.varname1.sql"] >> str;
json["ddml.varname1.serialize"] >> serialize1;
json["ddml.varname1.serialize"] >> json_bool;
json["ddml.varname3"] >> str;

#include <json_syntax.hpp>

// Do something with the values, e.g.:
std::list< std::string_view >& tokens
    = syntax::tokenize( str, " " );

std::string select = tokens[0] + " " + tokens[1];
std::string from = tokens[2] + " " + tokens[3];
std::string where;
if ( tokens.size() > 4 ) {
    size_t pos_where = str.find_first_of( "WHERE" );
    if ( pos_where != str.npos )
        where = str.substr( pos_where, str.length() - 2 );
}

if ( str1 != str3 ) {
    sample_custom_serialize( str3 );
    if ( serialize1 && serialize1 == json_bool ) {
        T& custom_type = stringToType<T>( str1 );
        std::cout << custom_type.toString();
        sample_custom_serialize_and_do( typeToType<C, std::string>(str1) );
    }
}
```

That's it! Everything above is all you need to know to live and develop JavaScriptObjectNotation with the `jsoncpp` library.

More expert users may want to define their own JsonSerializable type, with can be done like below.

```c++
#include <json_serializable.hpp>

// This should pull in and compile all function you need
class SQLQuery
    :   public types::JsonType< SQLQuery, std::string >
    ,   public RefList<std::string, std::string, std::string, int, bool>
{...}
```

All further function the compiler won't find are conversion functions, possibly:

```c++
#include <json_serializer.hpp>

template<>
SQLQuery& typeToType( const std::string& jsonstr );
template<>
std::string& typeToType( const SQLQuery& sqlq );
template<>
std::string typeToString( const SQLQuery& sqlq );
```

##### More easy own text-serializable and json-usable types

A type that includes and inherits function for transformation of e.g.
`struct YourStruct` with `YourStruct = BT` to a json-text serializable class
`class YourJsonType` with `YourJsonType = JT` that extensible and responsibly
enables you to serialized your struct-typed classes with `jsoncpp::types::JsonType<JT, BT>` to json and json strings/files.

For example inheriting a `jsoncpp::types::JsonType<YourType, BaseStructedType>` for your library/program could work like below.

```c++
struct CacaoExpression {
    std::string_view expr_str;
};

struct ddkml_obj {
    std::string_view var_name = "";
    std::string sql = "";
    CacaoExpression cacaoext;
    std::string dstruct = "";
};

#include <json_types.hpp>

class DdkmlObj
    :   public jsoncpp::types::JsonType< DdkmlObj, ddkml_obj >
{...}; // <- Your definitions there
```

so that you need only a few functions to define to be string-transformable and usable with json:

```c++
namespace jsoncpp {
template<>
ddkml::ddkml_obj& stringToType( const char* str ) {...}
template<>
const char* typeToString( const ddkml::ddkml_obj& obj ) {...}
template<>
ddkml::ddkml_obj& typeToType( const jsoncpp::JsonValue& json ) {...}
template<>
jsoncpp::JsonValue& typeToType( const ddkml::ddkml_obj& obj ) {...}
} // namespace jsoncpp
```

All functions you need for the inherited `JsonTextSerializableType<T>` and its value `T& value` are described there and above. The class `JsonType<JT, BT>` also needs (additional to the string-to-type functions the `typeToType<BT, JsonValue>` function in both transformation directions.)

##### More extended JavaScriptObjectNotation Development

An extension to JSON6 in `jsoncpp` is the value's key path extension, the serializable type extension classes and functions and a more modern way to use JSON is to use key variables in value strings to look them up and shortly replace them with the real value of a json object's key/value set. Briefly one may have a `kv_ext.json`file like this

```json
{
    "ext": "key_variables",
    "value_set": {
        "valueA": "A brief excerpt of a key variable's value",
        "valueB": "The extension #ext# will extend values with #valueA#"
    },
    "some_string": "This was a short demo of the vv extension namespace's extension #ext#.
        #value_set.valueB#. Thank you for using."
}
```

and with the keys-in-values extension would now use it in C++ like this

```c++
#include <iostream>
#include <json_objects.hpp>
#include <json_vv.hpp>

// Read the `.json` file
jsoncpp::Json* json = jsoncpp::Json::fromFile("kv_ext.json");
// Write all replacings in the value "some_string" and print it.
std::cout << jsoncpp::vv::variableValue(
    json->findKeyRef( "some_string" ).c_str(), json, true, '#'
) << std::endl;
// The function has the signature
// vv::variableValue( str_value, json_values, recusive_replace, var_delim );
```

to print with the use of a complete JSON7-like document's values

```console
$> This was a short demo of the vv extension namespace's extension key_variables.
        The extension key_variables will extend values with A brief excerpt of a key variable's 
        value. Thank you for using.
```

#### License

The project is licensed under the three licenses  to be read from the upper to the last:

* Academic Free License ("AFL") v. 3.0
* Blue Oak Model License
* ISC License

Copyright (c) 2025 Sebastian Lau

#### How to compile & use it

```sh
# Get the project from github
git clone https://github.com/onepointerT/jsoncpp.git
# Include it now in you `CMakeLists.txt`
include_directory( jsoncpp/include )
add_subdirectory( jsoncpp )
...
target_link_libraries( yourLib jsoncpp ... )
```
