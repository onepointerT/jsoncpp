
#pragma once

#include <any>
#include <map>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace jsoncpp {
namespace detail {


/**
 * @brief A short class to handle bivariate data types within a keyed map
 */
class AnyMap
    :   private std::map< std::string, std::any >
{
protected:
    /**
     * @brief Constructor
     */
    AnyMap();

public:
    /**
     * @brief Get a bivariate element by its key
     * @param key The key where the element is stored at
     */
    auto& operator[]( const char* key );
    /**
     * @brief Get a bivariate element by its key
     * @param key The key where the element is stored at
     */
    const auto& operator[]( const char* key ) const;
    /**
     * @brief Get the keystrings of all elements
     */
    const std::vector< std::string_view > keys() const;
    
    /**
     * @brief Add an element of the type `T` to the bivariate map
     * @param key The key where the element will be stored at
     * @param obj A pointer to the element which is to be stored
     */
    template< typename T >
    void emplace( const char* key, T* obj ) {
        std::any a = std::make_any( obj );
        std::map< std::string, std::any >::operator[](key) = a;
    }
};


/** 
 * @brief A simple index structure to describe a index by template parameter
 *      Access its value with `index<I>::value`
 */
template <size_t I> struct index { static constexpr size_t value = I; };

/**
 * @brief The forwarding definition of a `RefList< Args... >`
 */
template <typename... Args> class RefList { };

/**
 * @brief The trivial forwarding declaration with a `RefList<>` with no templating parameter
 * @note Latestly here the recursive an `N-1` template parameter inheriting recursion stops with `N=0`
 */
template <> class RefList<> { };


/**
 * @brief A not-so-cute reference list with bivariate and varible template types stored.
 *      Even a benefit in storage capacity with many elements. Less storage-expensive than `AnyMap`
 * @tparam Head The first template parameter type
 * @tparam Tail... All other template parameter types
 * @note The recursive inheritance stops, when `sizeof...(Tail) == 0` with `RefList<>`.
 */
template< typename Head, typename... Tail >
class RefList<Head, Tail...>
    :   public RefList< Tail... >
{
public:
    /** @brief The type of ther inheriting `RefList< Tail... >` */
    using deriving_t = RefList< Tail... >;

    /** @brief The current univariate value at this position of the list */
    Head& value;

    /** @brief The length of the list */
    size_t length = sizeof...( Tail ) + 1;

    /**
     * @brief Get the bivariate reference at the position `I`
     * @tparam I The index to look up
     * @returns The reference to the value at the current position
     */
    template< size_t I >
    auto& get( index<I> idx ) const {
        if ( index<I>::value == 0 ) return value;
        return deriving_t::get( index< I - 1 >::value );
    }
    /**
     * @brief Get the bivariate reference at the position `I`
     * @returns The reference to the value at the position 0
     */
    Head& get( index<0> idx ) const {
        return value;
    }

    /**
     * @brief Get the bivariate reference at the position `I`
     * @tparam I The index to look up
     * @returns The reference to the value at the current position
     */
    template< size_t I >
    auto& get( index<I> idx ) {
        if ( index<I>::value == 0 ) return value;
        return deriving_t::get( index< I - 1 >::value );
    }
    /**
     * @brief Get the bivariate reference at the position `I`
     * @returns The reference to the value at the position 0
     */
    Head& get( index<0> idx ) {
        return value;
    }

    /**
     * @brief Constructor
     * @param head The element at the current position
     * @note This constructor only inherits an empty `RefList<>`
     */
    RefList( Head& head )
        :   RefList<>()
        ,   value( head )
    {}

    /**
     * @brief Constructor
     * @param head The element at the current position
     * @note This constructor inherits an one `RefList< Tail... >` as long as there are
     *      values left.
     */
    RefList( Head& head, Tail&... tail )
        :   RefList< Tail... >( tail... )
        ,   value( head )
    {}
};


template< typename... Types >
class RefListNamed
    :   public RefList< Types... >
{
protected:
    std::map< std::string, unsigned int > idx_variables;

public:
    typedef typename std::map< std::string, unsigned int > index_map;
    typename index_map::const_iterator cbegin() const { return idx_variables.cbegin(); }
    typename index_map::const_iterator cend() const { return idx_variables.cend(); }

    RefListNamed( Types... )
        :   RefList< Types... >()
    {}

    /**
     * @brief Get a bivariate element by its key
     * @param key The key where the element is stored at
     */
    auto& get( const char* key ) const {
        try {
            std::string keystr = key;
            const unsigned int idx = idx_variables.at(keystr);
            return get<idx>(idx);
        } catch ( std::out_of_range& oor ) {
            return nullptr;
        }
    }

    /**
     * @brief Get a bivariate element by its key
     * @param key The key where the element is stored at
     */
    auto& get( const char* key ) {
        try {
            const unsigned int idx = idx_variables[key];
            return get<idx>(idx);
        } catch ( std::out_of_range& oor ) {
            return nullptr;
        }
    }

    const std::type_info& type( const char* key ) const noexcept {
        std::any* a = new std::any( this->get(key) );
        return a->type();
    }

    /**
     * @brief Get a bivariate element by its key
     * @param key The key where the element is stored at
     */
    auto& operator[]( const char* key ) const {
        return get(key);
    }

    /**
     * @brief Get a bivariate element by its key
     * @param key The key where the element is stored at
     */
    auto& operator[]( const char* key ) {
        return get(key);
    }

    /**
     * @brief Add an element of the type `T` to the bivariate map
     * @param key The key where the element will be stored at
     * @param obj A reference to the element which is to be stored
     */
    template< typename T >
    void emplace( const char* key, T& value ) {
        (*this)[key] = value;
    }
};

template< typename... Types >
RefListNamed< Types... >* newRefList( std::array<std::string, sizeof...(Types)> names ) {
    RefListNamed< Types... >* rln = new RefListNamed< Types... >();
    unsigned int idx = 0;
    for ( std::string s : names ) {
        rln->idx_variables[s] = idx;
        ++idx;
    }
    return rln;
}

} // namespace detail
} // namespace jsoncpp