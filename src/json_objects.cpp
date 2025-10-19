
#include "json_objects.hpp"

#include <string_view>

#include "json_syntax.hpp"
#include "json_types.hpp"

namespace jsoncpp {


JsonListView::JsonListView()
    :   std::list< JsonObjectView* >()
{}

JsonObjectView* JsonListView::findKey( const char* key ) {
    for ( JsonObjectView* obj : *this ) {
        if ( obj->key().compare(key) == 0 ) return obj;
    }
    
    JsonObjectView* jov = new JsonObjectView( key, "" );
    push_back( jov );
    return jov;
}

JsonObjectView& JsonListView::findKeyRef( const char* key ) {

    for ( JsonObjectView* obj : *this ) {
        if ( obj->key().compare(key) == 0 ) return *obj;
    }

    JsonObjectView* jov = new JsonObjectView( key, "" );
    push_back( jov );
    return *jov;
}

JsonObjectView* JsonListView::operator[]( const char* key ) {
    return findByKeyPath( key, this );
}

/*
const JsonObjectView* JsonListView::operator[]( const char* key ) const {
    return findByKeyPath( key, this );
}
*/


JsonListView& JsonListView::operator<<( JsonObjectView* json_obj ) {
    push_back( json_obj );
    return *this;
}


JsonListView& JsonListView::operator<<( JsonObjectView& json_obj ) {
    push_back( &json_obj );
    return *this;
}


JsonListView& JsonListView::operator<<( std::list< JsonObjectView* >* json_obj_lst ) {
    merge( *json_obj_lst );
    return *this;
}


const JsonListView& JsonListView::operator>>( std::list< JsonObjectView* >& other ) const {
    
    other.insert( other.begin(), begin(), end() );
    
    return *this;
}


const JsonListView& JsonListView::operator>>( types::Array& other ) const {
    for ( JsonObjectView* jov : *this ) {
        types::Object* obj = new types::Object( jov->key(), jov->value() );
        other << *obj;
    }
    return *this;
}


const JsonListView& JsonListView::operator>>( JsonValue& other ) const {
    other.second = toString();
    return *this;
}


std::string JsonListView::toString( const unsigned int base_indent ) const {

    std::string base_indent_str = syntax::make_indent( base_indent );
    std::string indent_str = syntax::make_indent( base_indent + 2 );

    std::string jsonstr = "\{\n";

    for ( JsonObjectView* jov : *this ) {
        jsonstr += indent_str;
        jsonstr += "\"" + jov->key() + "\": \"" + jov->value() + "\",\n";
    }
    jsonstr += base_indent_str + "\}\n";

    size_t pos_comma = jsonstr.rfind( "," );
    jsonstr.erase( pos_comma );

    return jsonstr;
}

const char* JsonListView::c_str( const unsigned int base_indent ) const {
    return toString().c_str();
}

JsonListView* JsonListView::fromString( const char* str ) {
    
    JsonListView* jlv = new JsonListView();

    std::list< JsonObjectView* >* jobjs
        = syntax::findJsonObjects( str );
    
    for ( JsonObjectView* jval : *jobjs ) {

        *jlv << jval;

    }

    return jlv;
}



JsonObjectView::JsonObjectView( const JsonValue& json )
    :   JsonValue( json.first, json.second )
    ,   m_children( new JsonListView() )
{}


JsonObjectView::JsonObjectView( const char* key, const char* value )
    :   JsonValue( key, value )
    ,   m_children( new JsonListView() )
{}


JsonObjectView::JsonObjectView( const char* keyvalue_pair )
    :   JsonValue( "", "" )
    ,   m_children( new JsonListView() )
{
    std::string_view strv = keyvalue_pair;

    // Find key
    Match( keyvalue_pair, keystr, regex::jsonstr )
    
    // Find value
    size_t pos_value_delimiter = strv.find_first_of(": ");
    std::string_view valuestr;
    if ( pos_value_delimiter != strv.npos ) {
        valuestr = strv.substr( pos_value_delimiter + 2 );
    }

    // Remove '\"' character and set value
    if ( keystr.size() > 0 ) {
        if ( keystr[0] == '\"' ) keystr.remove_prefix(1);
        if ( keystr[keystr.length()-1] == '\"' ) keystr.remove_suffix(1);
        this->first = keystr.data();
    }
    if ( valuestr.size() > 0 ) {
        if ( valuestr[0] == '\"' ) valuestr.remove_prefix(1);
        if ( valuestr[valuestr.length()-1] == '\"' ) valuestr.remove_suffix(1);
        this->second = valuestr.data();
    }
}

JsonObjectView* JsonObjectView::operator[]( const char* key ) {
    return findByKeyPath( key, m_children );
}

const JsonObjectView* JsonObjectView::operator[]( const char* key ) const {
    return findByKeyPath( key, m_children );
}


const std::string JsonObjectView::toString( const unsigned int base_indent ) const {
    std::string istr;
    std::string indent = syntax::make_indent( base_indent + 2 );

    istr += indent + "\"" + key() + "\": ";
    if ( m_children->size() == 0 ) {
        istr += "\"" + value() + "\"";
    } else {
        istr += "\{\n" + m_children->toString(base_indent+2);
        istr += base_indent + "\}";
    }

    return istr;
}


std::string JsonObjectView::key() const {
    return this->first;
}

std::string JsonObjectView::value() const {
    return this->second;
}

JsonListView* JsonObjectView::children() noexcept {
    return m_children;
}

const JsonListView* JsonObjectView::children() const noexcept {
    return m_children;
}

JsonObjectView::iterator JsonObjectView::begin() noexcept { return m_children->begin(); }
JsonObjectView::const_iterator JsonObjectView::begin() const noexcept { return m_children->begin(); }
JsonObjectView::iterator JsonObjectView::end() noexcept { return m_children->end(); }
JsonObjectView::const_iterator JsonObjectView::end() const noexcept { return m_children->end(); }
size_t JsonObjectView::size() const { return m_children->size(); }
bool JsonObjectView::empty() const noexcept { return m_children->empty(); }
void JsonObjectView::clear() noexcept { m_children->clear(); }


JsonObjectView& JsonObjectView::operator<<( const JsonValue& json_value ) {
    this->first = json_value.first;
    this->second = json_value.second;
    return *this;
}

JsonObjectView& JsonObjectView::operator<<( const JsonValue* json_value ) {
    this->first = json_value->first;
    this->second = json_value->second;
    return *this;
}


bool JsonObjectView::greater( const JsonObjectView& lhs, const JsonObjectView& rhs ) {
    return lhs.key() < rhs.key();
}

bool JsonObjectView::greater_ptr( const JsonObjectView* lhs, const JsonObjectView* rhs ) {
    return greater( *lhs, *rhs );
}

std::strong_ordering JsonObjectView::operator<=>( const JsonObjectView& other ) {
    if ( greater( *this, other ) ) return std::strong_ordering::greater;
    else if ( greater( other, *this ) ) return std::strong_ordering::less;
    else if ( value() == other.value() ) return std::strong_ordering::equivalent;
    return std::strong_ordering::equal;
}

std::strong_ordering JsonObjectView::operator<=>( const JsonObjectView* other ) {
    return this->operator<=>( *other );
}

JsonValue* JsonObjectView::fromStringJsonValue( const char* keyvalue_pair ) {
    return syntax::parseJsonKeyValue( keyvalue_pair );
}


JsonObjectView* JsonObjectView::fromString( const char* str ) {
    
    JsonObjectView* jov = new JsonObjectView( *fromStringJsonValue(str) );

    return jov;
}


Json::Json()
    :   JsonListView()
{}

Json::Json( const char* str )
    :   JsonListView()
{
    std::string_view strv = str;

    if ( strv.starts_with('{') ) strv.remove_prefix(1);
    if ( strv.ends_with('}')) strv.remove_suffix(1);

    *this << *(syntax::findJsonObjects( strv.data() ));
}


JsonObjectView& Json::at( const char* key ) {
    return findKeyRef( key );
}

/*
JsonObjectView& Json::at( const char* key ) const {
    return findKeyRef( key );
}
*/

JsonObjectView& Json::operator[]( const char* key ) {
    return *findByKeyPath( key, this );
}

/*
JsonObjectView& Json::operator[]( const char* key ) const {
    return *findByKeyPath( key, this );
}
*/

Json& Json::operator<<( const JsonValue& json_value ) {
    *this << new JsonObjectView( json_value );
    return *this;
}

Json& Json::operator<<( const JsonValue* json_value ) {
    *this << new JsonObjectView( *json_value );
    return *this;
}

Json& Json::operator<<( JsonObjectView* json_obj ) {
    this->push_back( json_obj );
    return *this;
}

Json& Json::operator<<( std::list< JsonObjectView* >& json_obj_lst ) {
    JsonListView::operator<<( &json_obj_lst );
    return *this;
}

Json* Json::fromString( const char* str ) {
    return syntax::findJson( str );
}

std::string Json::toString( const Json* json ) {

    std::string istr = "\{\n";
    std::string base_indent = "  ";

    for ( JsonObjectView* jov : *json ) {
        istr += jov->toString() + ",\n";
    }

    size_t pos_comma_last = istr.rfind(",");
    istr.erase( pos_comma_last );

    istr += "\}";

    return istr;
}


} // namespace jsoncpp