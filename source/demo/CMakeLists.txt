# set the main entrypoint file and entrypoint type (WIN32)
add_executable(demo WIN32 win32_entrypoint.cpp)

target_link_libraries(demo win32_game core glad)

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

target_link_libraries(demo opengl32)

# Creates a folder "executables" and adds target 
# project (app.vcproj) under it
#set_property(TARGET demo PROPERTY FOLDER "bin")

