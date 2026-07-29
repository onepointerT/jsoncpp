
#pragma once

#include "json_objects.hpp"
#include "json_syntax.hpp"

#include "ini.hpp"

#include <map>
#include <string>

namespace jsoncpp {


class JsonTemplate
    :   protected inicpp::IniSection
{
public:
    JsonTemplate( const inicpp::IniSection& inimap_section );
    JsonTemplate( const JsonTemplate& tmpl );

    JsonObjectView& get() const;

    static std::list< JsonTemplate* >& loadFromFile( const std::string path );
};


inline static std::map< std::string, const JsonTemplate* >& templates
                    = *new std::map< std::string, const JsonTemplate* >();


} // namespace jsoncpp