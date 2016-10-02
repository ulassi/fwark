file (GLOB GLAD_SOURCES
	"src/*.c" )

file (GLOB INCLUDES
	"include/glad/*.h")

include_directories(. "include" )

add_library(glad STATIC ${GLAD_SOURCES} ${INCLUDES} )

source_group(source FILES ${GLAD_SOURCES} )
source_group(include FILES ${INCLUDES} )

target_include_directories(glad PUBLIC "include")


set_property(TARGET glad PROPERTY FOLDER "lib")

