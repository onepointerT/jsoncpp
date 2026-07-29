
#include "json_template.hpp"

#include "json_syntax.hpp"

#include <ini.hpp>

namespace jsoncpp {


JsonTemplate::JsonTemplate( const inicpp::IniSection& inimap_section )
    :   inicpp::IniSection( inimap_section )
{}


JsonObjectView& JsonTemplate::get() const {
    JsonObjectView* jov = new JsonObjectView( "", "" );
    
    for ( typename inicpp::IniSection::const_iterator cIt
            : { this->cbegin(), this->cend() }
    ) {
        jsoncpp::setKeyByPath( cIt->second->key.c_str(), jov, cIt->second->data() );
    }

    return *jov;
}


std::list< JsonTemplate* >& JsonTemplate::loadFromFile( const std::string path ) {
    inicpp::IniMap& imap = inicpp::parseFromFile( path.c_str() );

    std::list< JsonTemplate* >& lst = *new std::list< JsonTemplate* >();
    for ( typename inicpp::IniMap::const_iterator secIt
            : { imap.cbegin(), imap.cend() }
    ) {
        JsonTemplate* jt = new JsonTemplate( *(secIt->second) );
        lst.push_back( jt );
    }

    return lst;
}


} // namespace jsoncpp