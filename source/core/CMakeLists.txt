

file (GLOB CORE_SOURCES 
	"*.h" 
	"*.cpp" )

file (GLOB INCLUDES 
	"include/core/*.h"  )

include_directories(. "include")

add_library(core STATIC ${CORE_SOURCES} ${INCLUDES} )

source_group(source FILES ${CORE_SOURCES} )
source_group(include FILES ${INCLUDES})
target_include_directories(core PUBLIC "include")
set_property(TARGET core PROPERTY FOLDER "lib")