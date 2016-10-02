# set the main entrypoint file and entrypoint type (WIN32)
add_executable(demo WIN32 win32_entrypoint.cpp)

target_link_libraries(demo win32_game core glad)


# Creates a folder "executables" and adds target 
# project (app.vcproj) under it
#set_property(TARGET demo PROPERTY FOLDER "bin")

