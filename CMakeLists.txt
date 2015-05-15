cmake_minimum_required(VERSION 2.6)

project(billiard)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include(modules/FindALUT.cmake)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(LIBRARY DrawFunc.cpp fpsMeasurer.cpp GlutHFunc.cpp glutRender.cpp quat.cpp sound.cpp table.cpp vec.cpp)
set(HEADER DrawFunc.h fpsMeasurer.h GlutHFunc.h glutRender.h quat.h sound.h table.h vec.h)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

add_library(libs STATIC ${LIBRARY} gui/Widget.cpp gui/Widget.h gui/Edit.cpp gui/Edit.h gui/EditFloat.cpp gui/EditFloat.h gui/Button.cpp gui/Button.h)
add_executable(main test.cpp gui/Widget.cpp gui/Widget.h gui/Edit.cpp gui/Edit.h gui/EditFloat.cpp gui/EditFloat.h gui/Button.cpp gui/Button.h)
target_link_libraries(main libs)

include_directories(${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ${ALUT_INCLUDE_DIRS})

set_target_properties(libs PROPERTIES LINK_SEARCH_START_STATIC 1)
set_target_properties(libs PROPERTIES LINK_SEARCH_END_STATIC 1)
set_target_properties(main PROPERTIES LINK_SEARCH_START_STATIC 1)
set_target_properties(main PROPERTIES LINK_SEARCH_END_STATIC 1)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
target_link_libraries(libs ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${ALUT_LIBRARIES})
target_link_libraries(main ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${ALUT_LIBRARIES})
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")