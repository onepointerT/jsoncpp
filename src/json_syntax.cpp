
#include "json_syntax.hpp"

#include <stdexcept>

namespace jsoncpp {


JsonObjectView* findByKeyPath( const char* key_path, JsonObjectView* jsonobj ) {
    std::list< std::string_view >& tokens
        = syntax::tokenize( key_path, "." );
    if ( tokens.size() == 0 ) tokens = { key_path };
    
    std::string str = key_path;
    std::size_t pos_last_dot = str.rfind( '.' );
    std::string last_key = str;
    if ( pos_last_dot != str.npos )
        last_key = str.substr( pos_last_dot + 1 );
    

    try {
        JsonObjectView* cjov = jsonobj;

        for ( std::string_view key_token : tokens ) {
            cjov = cjov->children()->findKey(key_token.data());
            if ( cjov == nullptr ) return nullptr;
            else if ( cjov->key() == last_key ) return cjov;
        }

        return nullptr;
    } catch ( std::out_of_range& oor ) {
        return nullptr;
    }
}


JsonObjectView* findByKeyPath( const char* key_path, JsonListView* jsonlist ) {

    std::string str = key_path;
    std::size_t pos_first_dot = str.find_first_of( '.' );
    std::string first_key = str;
    if ( pos_first_dot != str.npos ) {
        first_key = str.substr( 0, pos_first_dot - 1 );
        std::string rest_str = str.substr( pos_first_dot + 1 );
    
        JsonObjectView* jov = jsonlist->findKey( first_key.c_str() );
        if ( jov != nullptr && rest_str.size() > 0 )
            return findByKeyPath( rest_str.c_str(), jov );
        else if ( jov == nullptr ) {
            jov = new JsonObjectView( first_key.c_str(), "" );
            jsonlist->push_back( jov );
        }
        return jov;
    }

    return jsonlist->findKey( key_path );
}


JsonObjectView* findByKeyPath( const char* key_path, Json* json ) {
    return findByKeyPath( key_path, static_cast<JsonListView*>(json) );
}

namespace syntax {


std::string make_indent( const unsigned int indent_size ) {
    std::string indentstr;
    
    for ( unsigned int i = 0; i <= indent_size; i++ ) {
        indentstr += " ";
    }
    
    return indentstr;
}

unsigned int count_indent( const char* str, size_t pos ) {
    std::string_view istr = str;
    std::string nlstr = str;
    istr = istr.substr( pos );
    unsigned int indent = 0;
    
    // Check newline. If no indent at start, find the previous.
    if ( !istr.starts_with(" ") ) {
        size_t pos_nl_before = nlstr.rfind( "\n", pos );
        if ( pos_nl_before != nlstr.npos ) {
            istr = nlstr.substr( pos_nl_before + 1 );
        } else if ( nlstr[0] == ' ' ) { // Starts with indent
            istr = nlstr;
        } else { // Earlies indent at next lane
            size_t pos_nl_behind = nlstr.find_first_of( "\n", pos );
            istr = nlstr.substr( pos_nl_behind + 1 );
        }
    }

    // Now count the indent
    size_t cpos = 0;
    while ( istr.find_first_of( " ", cpos, 1 ) != istr.npos ) ++indent;

    return indent;
}

std::string_view of_indent( const char* str, const unsigned int indent_size, size_t pos ) {
    const std::string istr = str;

    std::string cstr;
    size_t cpos = pos;
    while ( count_indent( istr.c_str(), cpos ) >= indent_size ) {
        cpos = istr.find_first_of( '\n', cpos );
        if ( cpos == istr.length()-1 ) break;
        else if ( cpos == istr.npos ) return istr.substr( pos, istr.length() - 1 - pos );
    }

    return istr.substr( pos, cpos - pos );
}

std::string_view of_until( const char* str, const char* delim, size_t pos ) {
    const std::string istr = str;
    size_t pos_delim = istr.find_first_of( delim, pos );

    if ( pos_delim == istr.npos ) return istr.substr( pos );

    return istr.substr( pos, pos_delim - pos );
}

std::string_view of_since( const char* str, const char* delim, size_t pos ) {
    const std::string istr = str;
    size_t pos_delim = istr.find_first_of( delim, pos );

    if ( pos_delim == istr.npos ) return "";

    return istr.substr( pos_delim );
}


std::list< std::string_view >& tokenize( const char* str, const char* delim, size_t pos ) {
    std::list< std::string_view >* lst = new std::list< std::string_view >();

    std::string istr = str;
    std::string str_delim = delim;
    size_t pos_next_delim = pos;
    size_t cpos = pos;
    while ( (pos_next_delim = istr.find_first_of( delim, cpos )) != istr.npos ) {
        std::string_view strv = istr.substr( cpos, pos_next_delim - 1 );
        lst->push_front( strv );
        cpos = pos_next_delim + str_delim.length() + 1;
        if ( cpos >= istr.length() ) break;
    }

    return *lst;
}


JsonValue* parseJsonKeyValue( const std::string jsonkvstr ) {
    std::string_view strv = jsonkvstr;
    JsonValue* jval = new JsonValue( "", "" );

    // Find key
    Match( jsonkvstr.c_str(), keystr, regex::jsonstr )
    
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
        jval->first = keystr.data();
    }
    if ( valuestr.size() > 0 ) {
        if ( valuestr[0] == '\"' ) valuestr.remove_prefix(1);
        if ( valuestr[valuestr.length()-1] == '\"' ) valuestr.remove_suffix(1);
        jval->second = valuestr.data();
    }

    return jval;
}


JsonObjectView* findJsonObject( const SerializableTextType* json ) {

    JsonObjectView* jov = new JsonObjectView( *json );
    std::string_view strv = json->second;

    size_t pos_first_newline = strv.find_first_of( "\n" );
    size_t pos_first_comma = strv.find_first_of( "," );
    size_t pos_first_brace = strv.find_first_of( "\{");
    size_t pos_closing_brace = strv.rfind( "\}" );
    size_t pos_first_qoute = strv.find_first_of( "\"" );
    size_t pos_second_quote = strv.find_first_of( "\"", pos_first_qoute );
    if ( pos_first_brace < pos_first_qoute ) { // Expression like `"name" : {...` here

        // `findJsonObject` does not handle braces, so trim them
        std::string_view strv_recursive
            = strv.substr( pos_first_brace + 1, pos_closing_brace - pos_first_brace );

        *jov->children() << findJsonObjects( strv_recursive.data() );


    } else if ( pos_first_brace > pos_first_comma ) { // Multi-commata-separated values here. Commatas found

        // Find everything of this indent
        std::string_view strv_recursive = syntax::of_since( strv.data() );

        std::list< std::string_view >& value_tokens
            = syntax::tokenize( strv_recursive.data() );

        for ( std::string_view value_str : value_tokens ) {
            *jov->children() << new JsonObjectView( value_str.data() );
        }

    } // In each other case, there was a single value found

    return jov;
}


std::list< JsonObjectView* >* findJsonObjects( const std::string jsonstr ) {

    std::list< JsonObjectView* >* lst
        = new std::list< JsonObjectView* >();

    Matchall( jsonstr.c_str(), jomatches, regex::jsonkv )
    for ( std::string_view strv : jomatches ) {
        size_t pos_delim = strv.find_first_of( ": " );
        
        std::string_view key_strv = strv.substr( 0, pos_delim - 1 );
        std::string_view value_strv = strv.substr( pos_delim + 2 );
        
        SerializableTextType* stt
            = new SerializableTextType( key_strv.data(), value_strv.data() );
        
        JsonObjectView* obj = findJsonObject( stt );

        lst->push_back( obj );
    }

    return lst;
}


Json* findJson( const std::string jsonstr ) {
    return new Json( jsonstr.c_str() );
}

} // namespace syntax
} // namespace jsoncpp