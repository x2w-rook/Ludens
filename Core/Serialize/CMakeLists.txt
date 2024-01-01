add_library(LDSerialize STATIC
	"Include/INI.h"
	"Lib/INI.cpp"
)

target_include_directories(LDSerialize PRIVATE "Include")