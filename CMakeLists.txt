cmake_minimum_required(VERSION 3.11)
project(noizSFML)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

set(BUILD_SHARED_LIBS OFF)

FetchContent_Declare(sfml
  GIT_REPOSITORY https://github.com/SFML/SFML
  GIT_TAG 2f11710abc5aa478503a7ff3f9e654bd2078ebab
)
FetchContent_MakeAvailable(sfml)

add_executable(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics)
target_include_directories(${PROJECT_NAME} PRIVATE src)
target_include_directories(${PROJECT_NAME} PRIVATE ext)

target_sources(${PROJECT_NAME} PRIVATE src/)

if(CMAKE_CXX_COMPILER_ID STREQUAL Clang OR CMAKE_CXX_COMPILER_ID STREQUAL GNU)
  target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall -Wextra -Wpedantic -Wconversion -Werror=return-type
  )
endif()

if(CMAKE_GENERATOR MATCHES "^(Visual Studio)")
  target_compile_options(${PROJECT_NAME} INTERFACE /MP)
endif()