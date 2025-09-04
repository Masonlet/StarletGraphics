cmake_minimum_required(VERSION 3.23)
project(StarletGraphics LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_SUPPRESS_REGENERATION ON)

add_library(StarletGraphics STATIC)

file(GLOB_RECURSE GRAPHICS_SRC CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE GRAPHICS_HEADERS CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/inc/StarletGraphics/*.hpp)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "Source Files" FILES ${GRAPHICS_SRC})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/inc PREFIX "Header Files" FILES ${GRAPHICS_HEADERS})

target_sources(StarletGraphics
  PRIVATE ${GRAPHICS_SRC}
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/inc
    FILES ${GRAPHICS_HEADERS}
)

target_include_directories(StarletGraphics PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
  $<INSTALL_INTERFACE:include>
)

include(FetchContent)

FetchContent_Declare(StarletParsers GIT_REPOSITORY https://github.com/masonlet/starletparsers.git GIT_TAG main)
FetchContent_MakeAvailable(StarletParsers)

target_link_libraries(StarletGraphics PUBLIC StarletParsers)
if(TARGET glad)
  target_link_libraries(StarletGraphics PRIVATE glad)
  set_target_properties(glad PROPERTIES FOLDER "External")
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "External")
set_target_properties(StarletParsers PROPERTIES FOLDER "External")
