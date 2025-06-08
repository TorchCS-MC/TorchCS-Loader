cmake_minimum_required(VERSION 3.15)
project(torchcs_loader)

include(FetchContent)
include(cmake/torchcs_properties.cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(WIN32 AND MSVC)

    add_library(torchcs_loader SHARED
        src/dllmain.cpp
    )
    target_compile_definitions(torchcs_loader PRIVATE _CRT_SECURE_NO_WARNINGS)
    set_target_properties(torchcs_loader PROPERTIES LINK_FLAGS /EXPORT:DllMain,@1,NONAME)
    target_link_libraries(torchcs_loader PRIVATE torchcs::properties)
    
endif()
