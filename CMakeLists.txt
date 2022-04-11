cmake_minimum_required(VERSION 2.6)

# Name of project
set(PROJ_NAME bnk)
project(${PROJ_NAME})


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
SET(COMPILER_FLAGS "")

# Specify project files: header files and source files
set(HDRS
	include/path_config.h
	include/view.hh
	include/game.hh
	include/controller.hh
	include/shader.hh
	include/game_object.hh
	include/file_utils.hh
	include/data.hh
	include/entity.hh
	include/enemy.hh
	include/hitbox.hh
	include/blade.hh
	include/projectile.hh
	include/effect.hh
	include/collectible.hh
	include/collectibles.hh
	include/powerup.hh
	include/buoy.hh
	include/weapon.hh
	include/item.hh
	include/items.hh
)
 
set(SRCS
	src/main.cc
	src/engine/view.cc
	src/engine/controller.cc
	src/engine/game.cc
	src/engine/shader.cc
	src/util/file_utils.cc
	src/objects/game_object.cc
	src/objects/hitbox.cc
	src/objects/blade.cc
	src/objects/projectile.cc
	src/objects/effect.cc
	src/objects/collectibles/collectible.cc
	src/objects/collectibles/shield_collectible.cc
	src/objects/collectibles/powerup.cc
	src/objects/weapons/weapon.cc
	src/entities/entity.cc
	src/entities/enemy.cc
	src/entities/buoy.cc
	src/objects/items/item.cc
	src/objects/items/shield.cc
)

# Add path name to configuration file
configure_file(src/util/path_config.h.in ../include/path_config.h)

# Add executable based on the source files
add_executable(${PROJ_NAME} ${HDRS} ${SRCS})
add_definitions(${COMPILER_FLAGS})

# Directories to include for header files, so that the compiler can find
# path_config.h
target_include_directories(${PROJ_NAME} PUBLIC include)
target_include_directories(${PROJ_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

# Require OpenGL library
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
target_link_libraries(${PROJ_NAME} ${OPENGL_gl_LIBRARY})

# Other libraries needed
set(LIBRARY_PATH "" CACHE PATH "Folder with GLEW, GLFW, GLM, and SOIL libraries")
include_directories(${LIBRARY_PATH}/include)
if(NOT WIN32)
    find_library(GLEW_LIBRARY GLEW)
    find_library(GLFW_LIBRARY glfw)
    find_library(SOIL_LIBRARY SOIL)
elseif(WIN32)
    find_library(GLEW_LIBRARY glew32s HINTS ${LIBRARY_PATH}/lib)
    find_library(GLFW_LIBRARY glfw3 HINTS ${LIBRARY_PATH}/lib)
    find_library(SOIL_LIBRARY SOIL HINTS ${LIBRARY_PATH}/lib)
endif(NOT WIN32)
target_link_libraries(${PROJ_NAME} ${GLEW_LIBRARY})
target_link_libraries(${PROJ_NAME} ${GLFW_LIBRARY})
target_link_libraries(${PROJ_NAME} ${SOIL_LIBRARY})

# The rules here are specific to Windows Systems
if(WIN32)
    # Avoid ZERO_CHECK target in Visual Studio
    set(CMAKE_SUPPRESS_REGENERATION TRUE)
 
    # This will use the proper libraries in debug mode in Visual Studio
    set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)
endif(WIN32)
