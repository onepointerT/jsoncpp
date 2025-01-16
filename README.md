# jsoncpp
##### A data handling and serialization library for JSON
###### v1.6.0


The library allows to read and write files in JavaScriptObjectNotation (`*.json`), to access the values and define them in a ObjectView model with commonly useful C++ operators (see examples below).

It is possible to serialize the values into the standard C/C++ types and even customly developed types or classes via inheritance.

###### TODO: documentation
###### TODO: value reference extension

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



