
#include "json_detail.hpp"

namespace jsoncpp {
namespace detail {


AnyMap::AnyMap()
    :   std::map< std::string, std::any >()
{}


auto& AnyMap::operator[]( const char* key ) {
    std::any* a = new std::any( &this->at( key ) );
    return std::any_cast<decltype(a->type())&>( *a );
}


const auto& AnyMap::operator[]( const char* key ) const {
    const std::any* a = new std::any( &this->at( key ) );
    return std::any_cast<const decltype(a->type())&>( *a );
}

const std::vector< std::string_view > AnyMap::keys() const {
    std::vector< std::string_view > vec;

    for ( typename std::map< std::string, std::any >::const_iterator it
            = this->begin(); it != this->end(); ++it
    ) {
        vec.push_back( it->first.c_str() );
    }

    return vec;
}


} // namespace detail
} // namespace jsoncpp