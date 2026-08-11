
#pragma once

#include "json_definition.hpp"
#include "json_objects.hpp"
#include "json_syntax.hpp"

#include <map>
#include <string>

#ifdef JSONCPP_ENABLE_INICONFIG
namespace inicpp {
class IniSection;
} // namespace inicpp

#include <ini.hpp>
#endif

namespace jsoncpp {


class JsonTemplate
{
public:
#ifdef JSONCPP_ENABLE_INICONFIG
    JsonTemplate( const inicpp::IniSection* inimap_section );
#endif
    JsonTemplate( const JsonTemplate& tmpl );

    JsonObjectView& get() const;

    static std::list< JsonTemplate* >& loadFromFile( const std::string path );
};


inline static std::map< std::string, const JsonTemplate* >& templates
                    = *new std::map< std::string, const JsonTemplate* >();


} // namespace jsoncpp