
#pragma once

#include "json_definition.hpp"


#include "json_objects.hpp"


namespace jsoncpp {



JsonObjectDefinition::JsonObjectDefinition( const init_list_t il_name_keys )
    :   std::list< std::string >( il_name_keys )
{}


JsonObjectDefinition& JsonObjectDefinition::operator<<( JsonObjectDefinition& inst, const std::string name_key ) {
    inst.emplace_back( name_key );
    return inst;
}

JsonObjectDefinition& JsonObjectDefinition::operator<<( JsonObjectDefinition& inst, const init_list_t il_name_keys_addition ) {
    for ( std::string s : il_name_keys_addition ) {
        inst << s;
    }
    return inst;
}


JsonObjectView& JsonObjectDefinition::makeObject() const {
    JsonObjectView& jov = *new JsonObjectView();

    for ( const std::string sk : *this ) {
        jov << *new JsonValue{ sk, "" };
    }

    return jov;
}


typename std::list< std::string >::const_iterator cbeginIterator() const {
    return this->cbegin();
}


typename std::list< std::string >::const_iterator cendIterator() const {
    return this->cend();
}


JsonDefinition::JsonDefinition( init_list_t il_object_defs )
    :   std::map< std::string, JsonObjectDefinition* >()
{
    for ( object_definition_t od : il_object_defs ) {
        *this << od;
    }
}


JsonDefinition& JsonDefinition::operator<<( JsonDefinition& inst, const object_definition_t od ) {
    inst[od.first] = new JsonObjectDefinition( od.second );
    return inst;
}


JsonObjectView* JsonDefinition::makeObject( const char* key ) const {
    const JsonObjectDefinition* jodef = (*this)[key];
    if ( jodef != nullptr ) return &(jodef->makeObject());
    return nullptr;
}


} // namespace jsoncpp
