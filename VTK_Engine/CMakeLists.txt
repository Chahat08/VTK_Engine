# CMakeList.txt : CMake project for VTK_Engine, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (VTK_Engine "VTK_Engine.cpp" "VTK_Engine.h")

target_link_libraries(VTK_Engine PRIVATE ${VTK_LIBRARIES})

vtk_module_autoinit(
	TARGETS VTK_Engine
	MODULES ${VTK_LIBRARIES}
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET VTK_Engine PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
