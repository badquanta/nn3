cmake_minimum_required(VERSION 3.10)
project(nuscape-naviguesser-3)

set(CMAKE_CXX_STANDARD 20)
find_package(PkgConfig)
pkg_check_modules(GTK gtk+-3.0 REQUIRED)
pkg_check_modules(WEBKIT2 webkit2gtk-4.0 REQUIRED)


configure_file(naviguesser.glade naviguesser.glade COPYONLY)
add_subdirectory(icons)


add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/resources.c
    COMMAND glib-compile-resources --target=${CMAKE_CURRENT_BINARY_DIR}/resources.c --generate-source ${CMAKE_CURRENT_SOURCE_DIR}/resources.xml 
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/resources.xml icons/Open.png ${ICON_LIST} nuscape.glade
)
add_compile_options(-Wno-deprecated-declarations)
add_executable(naviguesser 
    src/main.cpp 
    src/nuscape.cpp 
    src/naviguesser.cpp 
    ${CMAKE_CURRENT_BINARY_DIR}/resources.c
)



target_link_libraries(naviguesser ${GTK_LIBRARIES} ${WEBKIT2_LIBRARIES})
include_directories(${GTK_INCLUDE_DIRS} ${WEBKIT2_INCLUDE_DIRS})

set(CPACK_PACKAGE_NAME "naviguesser")
set(CPACK_PACKAGE_VENDOR "badquanta")
set(CPACK_PACKAGE_VERSION "0.0.1")
set(CPACK_PACKAGE_CONTACT "BadQuanta badquanta@gmail.com")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Naviguesser")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libgtk-3-0, libwebkit2gtk-4.0-37")
set(CPACK_GENERATOR "DEB;ZIP;TGZ")

include(CPack)