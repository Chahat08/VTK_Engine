﻿# CMakeList.txt : CMake project for VTK_Engine, include source and define
# project specific logic here.
#

# Add source to this project's executable.
set(APP
	"include/app/app.h"
	"src/app/app.cpp"
	"include/app/window.h"
	"src/app/window.cpp"
)

set(CAMERA
	"include/camera/camera.h"
	"src/camera/camera.cpp"
)

set(INTERACTION
	"include/interaction/client.h"
	"src/interaction/client.cpp"
	"include/interaction/frontendData.h"
	"src/interaction/frontendData.cpp"
	"include/interaction/interactor.h"
	"src/interaction/interactor.cpp"
)

set(READER
	"include/reader/reader.h"
	"src/reader/reader.cpp"
)

set(VOLUME
	"include/volume/mapper.h"
	"src/volume/mapper.cpp"
	"include/volume/volume.h"
	"src/volume/volume.cpp"
	"include/volume/property.h"
	"src/volume/property.cpp"
	"include/volume/slice.h"
	"src/volume/slice.cpp"
	"include/volume/outline.h"
	"src/volume/outline.cpp"
)

set(UTILS
	"include/utils/json/simdjson.h"
	"src/utils/json/simdjson.cpp"
	"include/utils/websocket/easywsclient.hpp"
	"src/utils/websocket/easywsclient.cpp"
)

set(INCLUDE_DIRS
	"include"
)

set(SOURCE_DIRS
	${APP}
	${CAMERA}
	${INTERACTION}
	${READER}
	${VOLUME}
	${UTILS}
	"src/main.cpp"
	"include/config.h"
)

if(WIN32)
    add_executable(VTK_Engine WIN32 ${SOURCE_DIRS})
else()
    add_executable(VTK_Engine ${SOURCE_DIRS})
endif()


target_include_directories(VTK_Engine PUBLIC ${INCLUDE_DIRS})

target_link_libraries(VTK_Engine PRIVATE ${VTK_LIBRARIES})

vtk_module_autoinit(
	TARGETS VTK_Engine
	MODULES ${VTK_LIBRARIES}
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET VTK_Engine PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
