
#########################################################
# Source Files
#########################################################

# ----TODO------------------- #
# list your source files here #
# --------------------------- #
SET(sources
	"application.hpp"
	"application.cpp"

	"Branch.h"
	"Branch.cpp"
	"BranchFactory.h"
	"BranchFactory.cpp"
	"Circle.h"
	"Circle.cpp"
	"Include.h"
	"ProjectMath.h"
	"ProjectMath.cpp"
	"Rule.h"
	"Rule.cpp"
	"TreeFactory.h"
	"TreeFactory.cpp"
	"Turtle.h"
	"Turtle.cpp"
	"Variable.h"
	"Variable.cpp"
	"Vertex.h"
	"Vertex.cpp"
	
	"opengl.hpp"

	"main.cpp"

	"CMakeLists.txt"
)

# Add executable target and link libraries
add_executable(${CGRA_PROJECT} ${sources})

# ----TODO--------------------- #
# list your subdirectories here #
# ----------------------------- #
add_subdirectory(cgra)



#########################################################
# Link and Build Executable
#########################################################

# Set source groups (helper method)
target_source_group_tree(${CGRA_PROJECT})

# Set working folder
target_compile_definitions(${CGRA_PROJECT} PRIVATE "-DCGRA_SRCDIR=\"${PROJECT_SOURCE_DIR}\"")

# Link usage requirements
target_link_libraries(${CGRA_PROJECT} PRIVATE glew glfw ${GLFW_LIBRARIES})
target_link_libraries(${CGRA_PROJECT} PRIVATE stb imgui)

# For experimental <filesystem>
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	target_link_libraries(${CGRA_PROJECT} PRIVATE -lstdc++fs)
endif()
