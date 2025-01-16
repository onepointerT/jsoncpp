cmake_minimum_required( VERSION 3.28 )


project( jsoncpp LANGUAGES CXX )

set( PROJECT_NAME jsoncpp )
set( PROJECT_BRIEF "A data handling and serialization library for JSON" )
set( PROJECT_VERSION "1.6.1" )

set( CMAKE_C_STANDARD 23 )
set( CMAKE_CXX_STANDARD 23 )

if( UNIX )
    add_definitions( -fpermissive )
endif()

file( GLOB SRCFILES src/*.cpp )
add_library( jsoncpp STATIC ${SRCFILES} )

set( ENABLE_DOC_jsoncpp TRUE )
if( ENABLE_DOC_jsoncpp )
	find_package(Doxygen)
	if(DOXYGEN_FOUND)
		message("-- Enabled doc_jsoncpp target for make")
        # CMAKE_CURRENT_SOURCE_DIR is where the last call to CMake's project was,
        # CMAKE_CURRENT_BINARY_DIR is where the build of the binaries is performed
		configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
		add_custom_target(doc_jsoncpp ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMENT "Generating documentation with Doxygen")
	endif()
endif()