cmake_minimum_required(VERSION 3.23)

# Config
set (GRAPHICS_NAME starlet_graphics)
set (GRAPHICS_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set (GRAPHICS_INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/inc")

if(NOT TARGET ${GRAPHICS_NAME})
  add_library(${GRAPHICS_NAME} STATIC)

  file(GLOB_RECURSE GRAPHICS_SRC CONFIGURE_DEPENDS ${GRAPHICS_SRC_DIR}/*.cpp)
  file(GLOB_RECURSE GRAPHICS_HEADERS CONFIGURE_DEPENDS ${GRAPHICS_INC_DIR}/starlet-graphics/*.hpp)

  target_sources(${GRAPHICS_NAME} PRIVATE ${GRAPHICS_SRC} ${GRAPHICS_HEADERS})

  target_include_directories(${GRAPHICS_NAME} 
    PUBLIC 
      $<BUILD_INTERFACE:${GRAPHICS_INC_DIR}> 
      $<INSTALL_INTERFACE:include>
  )

  target_link_libraries(${GRAPHICS_NAME} 
    PUBLIC 
      starlet_serializer
      starlet_scene
      glad
  )

  # IDE organization
  source_group(TREE ${GRAPHICS_SRC_DIR} PREFIX "Source Files" FILES ${GRAPHICS_SRC})
  source_group(TREE ${GRAPHICS_INC_DIR} PREFIX "Header Files" FILES ${GRAPHICS_HEADERS})
endif()
