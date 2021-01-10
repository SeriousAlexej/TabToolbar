cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 11)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

if(WIN32)
    set(INSTALL_MISC_DIR .)
else()
    set(INSTALL_MISC_DIR share/TabToolbar)
endif()

project(TabToolbar)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(TT_BUILD_EXAMPLES FALSE CACHE BOOL "Build example projects")

add_subdirectory(src/TabToolbar)

if(TT_BUILD_EXAMPLES)
    add_subdirectory(examples/Test)
endif()

install(DIRECTORY include
        DESTINATION .
        COMPONENT devel
        FILES_MATCHING PATTERN "*.h")
install(FILES LICENSE DESTINATION ${INSTALL_MISC_DIR})
install(FILES COPYING DESTINATION ${INSTALL_MISC_DIR})
install(FILES COPYING.LESSER DESTINATION ${INSTALL_MISC_DIR})
install(FILES cmake-modules/FindTabToolbar.cmake DESTINATION ${INSTALL_MISC_DIR}/cmake-modules)
