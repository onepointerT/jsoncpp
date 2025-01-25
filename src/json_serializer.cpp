
#include "json_serializer.hpp"

#include <string_view>

#include "json_syntax.hpp"

namespace jsoncpp {

JsonSerializableObject::JsonSerializableObject( const char* keystr )
    :   key( keystr )
{
    std::string_view kstr = keystr;
    if ( kstr.contains( ": " ) || kstr.starts_with( " " ) ) {
        size_t pos_qoutation_first = kstr.find_first_of( "\"" );
        if ( pos_qoutation_first != kstr.npos ) {
            size_t pos_qoutation_second = kstr.find_first_of( "\"", pos_qoutation_first );
            key = kstr.substr( pos_qoutation_first + 1, pos_qoutation_second - pos_qoutation_first - 2 ).data();
        }
    } else if ( kstr.starts_with( "\"" ) ) {
        size_t pos_qoutation_second = kstr.find_first_of( "\"", 1 );
        if ( pos_qoutation_second != kstr.npos )
            key = kstr.substr( 1, pos_qoutation_second - 2 ).data();
        else
            key = kstr.substr( 1 ).data();
    }
}

JsonValue* JsonSerializableObject::toJson() const {
    return new JsonValue( this->key, "" );
}

} // namespace jsoncpp