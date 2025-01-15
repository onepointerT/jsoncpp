
#include "json_document.hpp"

#include <fstream>

#include "json_stringify.hpp"


namespace jsoncpp {

Json* JsonDocument::fromFile( const char* filepath ) {
    std::ifstream file( filepath, std::ios::binary | std::ios::ate );
    std::streamsize size = file.tellg();
    std::string* filecontent = new std::string(size, '\0');
    file.seekg(0);
    file.read( filecontent->data(), size );
    Json* json = rstringify( filecontent->c_str() );
    delete filecontent;
    file.close();
    return json;
}


void JsonDocument::toFile( const char* filepath, const Json* json ) {
    std::ofstream( filepath, std::ios::binary | std::ios::ate )
        << stringify( json );
}

} // namespace jsoncpp