file (GLOB STB_SOURCES
	"*.cpp" )

file (GLOB INCLUDES
	"include/stb/*.h")

include_directories(. "include" )

add_library(stb STATIC ${STB_SOURCES} ${INCLUDES} )

source_group(source FILES ${STB_SOURCES} )
source_group(include FILES ${INCLUDES} )
target_include_directories(stb PUBLIC "include")
set_property(TARGET stb PROPERTY FOLDER "lib")

