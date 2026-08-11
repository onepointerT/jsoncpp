
#pragma once

#include <initializer_list>
#include <list>
#include <map>
#include <string>
#include <utility>


namespace jsoncpp {

// Forward-declaration
class Json;


/**
 * @brief Setup some type `JT` from a `JsonObjectView` as it can be obtained by `JsonObjectDefinition::make()`
 * @tparam JT The own type
 * @param own_instance The own JsonType's instance
 * @param obj_def The json object obtained from `JsonObjectDefinition::make()`
 * @returns The update of `own_instance`
 */
template< class JT >
extern JT& setupJsonTypeFromObjView( JT& own_instance, const JsonObjectView& obj_def );

/**
 * @note Template specialization
 * @brief Setup some type inheriting `JsonObjectView` from a `JsonObjectView` as it can be obtained by `JsonObjectDefinition::make()`
 * @param own_instance The own JsonType's instance
 * @param obj_def The json object obtained from `JsonObjectDefinition::make()`
 * @returns The update of `own_instance`
 */
template<>
JsonObjectView& setupJsonTypeFromObjView( JsonObjectView& own_instance, const JsonObjectView& obj_def ) {
    for ( const std::string sk : obj_def.children()->keys() ) {
        const JsonObjectView* jobj = obj_def[sk.c_str()];

        if ( jobj == nullptr )
            own_instance << new JsonValue{ sk, "" };
        else {
            own_instance << { sk, jobj };
        }
    }

    return own_instance;
}



/**
 * @brief Define own objects with initializer lists
 */
namespace defs {

/**
 * @brief Define the value's name keys of an json object
 */
class JsonObjectDefinition final
    :   protected std::list< std::string >
{
public:
    /** @brief The type the initializer_list has to have */
    using init_list_t = std::initializer_list< std::string >;

    /**
     * @brief Constructor.
     * @param il_name_keys The names of all value's key referencers.
     */
    JsonObjectDefinition( const init_list_t il_name_keys );

    /**
     * @brief Add a key of a value to this list
     * @param inst A instance accepting the new key
     * @param name_key The key name to add
     * @returns A reference to `inst`
     */
    static inline JsonObjectDefinition& operator<<( JsonObjectDefinition& inst, const std::string name_key );
    /**
     * @brief Add a key of a value to this list
     * @param inst A instance accepting the new key
     * @param il_name_keys_addition The key name to add
     * @returns A reference to `inst`
     */
    static inline JsonObjectDefinition& operator<<( JsonObjectDefinition& inst, const init_list_t il_name_keys_addition );

    /**
     * @brief Make a new `JsonObjectView` out of the json object's member names of this instance
     * @returns A reference to a new `JsonObjectView` with all members, that have a reference key in this instance of 
     *      `JsonObjectDefinition`.
     */
    JsonObjectView& makeObject() const;

    /** @brief The iterator type of this class. Use 
     *          `for ( typename JsonObjectDefinition::const_iterator cit
     *                  : { cbeginIterator(), cendIterator() } ) {}` */
    using const_iterator = typename std::list< std::string >::const_iterator;

    /**
     * @brief 
     */
    typename const_iterator cbeginIterator() const;
    typename const_iterator cendIterator() const;

    template< class IH >
    class Class
        :   public JsonObjectView
    {
    protected:
        friend class IH;

    public:
        virtual extern static const std::list< std::string > IH::key_names;
    
        Class( const std::list< std::string > JsonObjectView* obj_def = nullptr )
            :   JsonObjectView()
        {
            if ( obj_def != nullptr ) {
                jsoncpp::setupJsonTypeFromObjView<JsonObjectView>( *this, obj_def );
            }
        }
        Class( JsonObjectDefinition& jobj_def )
            :   JsonObjectView( jobj_def.makeObject() )
        {}
        Class()
            :   JsonObjectView( (*new JsonDefinition( { Class<IH>::key_names, Class<IH>::key_names } )).makeObject() )

        operator std::string() const {
            return typeToString( *this );
        }
    };
};


class JsonDefinition final
    :   protected std::map< std::string, JsonObjectDefinition* >
{
public:
    using object_definition_t = std::pair< std::string, typename JsonObjectDefinition::init_list_t >;
    using init_list_t = std::initializer_list< object_definition_t >;

    JsonDefinition( init_list_t il_object_defs );

    static inline JsonDefinition& operator<<( JsonDefinition& inst, const object_definition_t od );

    JsonObjectView* makeObject( const char* key ) const;
};


} // namespace defs

static inline jsoncpp::defs::JsonDefinition& definitions = *new jsoncpp::defs::JsonDefinition( {} );

} // namespace jsoncpp

#define jsondef jsoncpp::definitions
#define JSONCPP_OBJDEF( obj_name, il_name_keys ) { #obj_name, { il_name_keys.begin(), il_name_keys.end() } }
