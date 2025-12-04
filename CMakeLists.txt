cmake_minimum_required(VERSION 3.16)
project(dirstat VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Optimisations pour release
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(MSVC)
        add_compile_options(/O2 /GL)
        add_link_options(/LTCG)
    else()
        add_compile_options(-O3 -flto)
        add_link_options(-flto)
    endif()
endif()

# Sources
set(SOURCES
    src/main.cpp
    src/scanner.cpp
    src/display.cpp
)

set(HEADERS
    src/scanner.hpp
    src/display.hpp
    src/stats.hpp
    src/colors.hpp
)

add_executable(dirstat ${SOURCES} ${HEADERS})

# Windows specific
if(WIN32)
    target_compile_definitions(dirstat PRIVATE _CRT_SECURE_NO_WARNINGS NOMINMAX)
endif()

# Install
install(TARGETS dirstat RUNTIME DESTINATION bin)
