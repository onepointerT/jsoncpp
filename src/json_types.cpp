
#include "json_types.hpp"

#include "json_types_cmp.hpp"

namespace jsoncpp {
namespace types {

Bool::Bool( const std::string keystr, const bool value )
    :   JsonType< Bool, bool >( keystr, value, this )
{}



Null::Null( const std::string keystr, const std::nullptr_t value )
    :   JsonType< Null, std::nullptr_t >( keystr, nullptr, this )
{}

const std::string Null::str() const {
    return JSON_NULLSTR;
}



Object::Object( const std::string keystr, const std::string value )
    :   JsonType< Object, std::string >( keystr, value, this )
{}

bool Object::greater( const Object& lhs, const Object& rhs ) {
    return lhs.value < rhs.value;
}

std::strong_ordering Object::operator<=>( const Object& other ) {
    if ( Object::greater( *this, other ) ) return std::strong_ordering::greater;
    else if ( Object::greater( other, *this ) ) return std::strong_ordering::less;
    else if ( key == other.key && value == other.value ) return std::strong_ordering::equivalent;
    return std::strong_ordering::equal;
}



Array::Array( const std::string keystr )
    :   JsonType< Array, std::list< Object > >( keystr, *(new std::list< Object >()), this )
{}

Array::Array( const std::string keystr, std::list< Object > json_obj_lst )
    :   JsonType< Array, std::list< Object > >( keystr, json_obj_lst, this )
{}

Object& Array::operator[]( const size_t pos ) {
    size_t cpos = 0;
    for ( typename std::list< Object >::iterator json_obj = value.begin()
        ; json_obj != value.end(); ++json_obj
    ) {
        if ( cpos == pos ) {
            return *json_obj;
        } else if ( pos >= value.size() ) break;
        ++cpos;
    }
    return *(new Object( "Object", "undefined" ));
}

const Object& Array::operator[]( const size_t pos ) const {
    size_t cpos = 0;
    for ( typename std::list< Object >::const_iterator json_obj = value.cbegin()
        ; json_obj != value.end(); ++json_obj
    ) {
        if ( cpos == pos ) {
            return *json_obj;
        } else if ( pos >= value.size() ) break;
        ++cpos;
    }
    return *(new Object( "Object", "undefined" ));
}

void Array::operator>>( std::list< Object > json_obj_lst ) const {
    json_obj_lst = value;
}

void Array::operator>>( std::list< Object >& json_obj_lst ) const {
    json_obj_lst = value;
}

Array& Array::operator<<( Object& json_obj ) {
    value.push_back( json_obj );
    return *this;
}

Array& Array::operator<<( std::list< Object >& json_obj_lst ) {
    this->value.append_range(std::ranges::subrange{json_obj_lst.begin(), json_obj_lst.end()});
    this->value.sort( LessObject );
    return *this;
}



String::String( const std::string keystr )
    :   JsonType< String, std::string_view >( keystr, "", this )
{}

String::String( const std::string keystr, const std::string valuestr )
    :   JsonType< String, std::string_view >( keystr, valuestr.c_str(), this )
{}


} // namespace types
} // namespace jsoncpp