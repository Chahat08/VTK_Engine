﻿# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
#if (POLICY CMP0141)
#  cmake_policy(SET CMP0141 NEW)
#  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
#endif()

project ("VTK_Engine")

find_package(VTK COMPONENTS
	CommonColor
	CommonCore
	CommonDataModel
	CommonMisc
	FiltersCore
	FiltersModeling
	FiltersSources
	IOImage
	IOLegacy
	ImagingColor
	RenderingContextOpenGL2
	RenderingCore
	RenderingFreeType
	RenderingGL2PSOpenGL2
	RenderingOpenGL2
	RenderingVolumeOpenGL2
	RenderingExternal	
)

# Include sub-projects.
add_subdirectory ("VTK_Engine")
