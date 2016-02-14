#You could change this to anything >= 2.8 actually, I believe.
cmake_minimum_required(VERSION 3.3)
#Project Name
project(Project3)
# Project Name

#########################################################
# FIND GLUT
#########################################################
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if (NOT GLUT_FOUND)
    message(ERROR "GLUT not found! Did you install all the things you need to?")
endif (NOT GLUT_FOUND)
#########################################################
# FIND OPENGL
#########################################################
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if (NOT OPENGL_FOUND)
    message(ERROR "OpenGL not found! Did you install all the things you need to?")
endif (NOT OPENGL_FOUND)
########################################################
# Linking & stuff
#########################################################
set(SOURCE_FILES main.cpp Actor.cpp GameController.cpp StudentWorld.cpp GameWorld.cpp)
add_executable(Project3 ${SOURCE_FILES})
target_link_libraries(Project3 ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#Modified version of one created & posted by Igor Barbosa (igorbarbosa.com)