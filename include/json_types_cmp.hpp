
#pragma once

#include <functional>

#include "json_types.hpp"


namespace jsoncpp {
namespace types {
constexpr bool LessObject( const Object& lhs, const Object& rhs ) {
    return Object::greater( rhs, lhs );
}
} // namespace types
} // namespace jsoncpp
