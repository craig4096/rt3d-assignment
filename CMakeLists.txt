cmake_minimum_required(VERSION 3.27.3)

project(rt3d-assignment)

find_package(GLUT REQUIRED)
find_package(SFML COMPONENTS audio REQUIRED)

file(GLOB SRC_FILES RT3DAssignment/*.cpp RT3DAssignment/*.c RT3DAssignment/*.h)
add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} GLUT::GLUT sfml-audio)

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Game/ $<TARGET_FILE_DIR:rt3d-assignment>)
