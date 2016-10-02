

file (GLOB WIN32_GAME_SOURCES 
	"*.h" 
	"*.cpp" )

file (GLOB RENDER_SOURCES
	"render/*.h"
	"render/*.cpp" )

file (GLOB SYSTEM_SOURCES
	"systems/*.h"
	"systems/*.cpp" )

file (GLOB INCLUDES
	"include/win32_game/*.h")

include_directories(. "include" "${PROJECT_SOURCE_DIR}/source/core/include")

add_library(win32_game STATIC ${WIN32_GAME_SOURCES} ${RENDER_SOURCES} ${SYSTEM_SOURCES} ${INCLUDES} )

source_group(source\\render FILES ${RENDER_SOURCES} )
source_group(source\\system FILES ${SYSTEM_SOURCES} )
source_group(source FILES ${WIN32_GAME_SOURCES} )
source_group(include FILES ${INCLUDES})

target_include_directories(win32_game PUBLIC "include")

target_link_libraries(win32_game PUBLIC glad)

set_property(TARGET win32_game PROPERTY FOLDER "lib")