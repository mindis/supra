cmake_minimum_required( VERSION 3.0.0 FATAL_ERROR )
MESSAGE(STATUS "Building SUPRA Graphic Interface")

# if ITK is used we need to include this for each executable
include(supraIncludeITK)

# Widgets finds its own dependencies.
find_package(Qt5Widgets REQUIRED)

OPTION(SUPRA_USE_CAMPVIS "Use CAMPVis for data previews" OFF)
if(${SUPRA_USE_CAMPVIS})
	# The CAMPVis package should be found automatically if you built CAMPVis through CMake
	FIND_PACKAGE(CAMPVis REQUIRED)
	SET(CAMPVIS_SOURCES 
		CampvisPreviewReciever.cpp)
	SET(CAMPVIS_HEADERS
		CampvisPreviewReciever.h)
ELSE()
	SET(CAMPVIS_SOURCES)
	SET(CAMPVIS_HEADERS)
ENDIF(${SUPRA_USE_CAMPVIS})

set ( SOURCES
 main.cpp
 mainwindow.cpp
 parameterWidget.cpp
 parametersWidget.cpp
 previewBuilderQt.cpp
 QImagePreviewReciever.cpp
 QTrackerPreviewReciever.cpp
 ${CAMPVIS_SOURCES}
)

set ( HEADERS
 parameterWidget.h
 previewBuilderQt.h
 QImagePreviewReciever.h
 QTrackerPreviewReciever.h
 ${CAMPVIS_HEADERS}
)

set ( MOC_HEADERS
 mainwindow.h
 parametersWidget.h
)

set ( UIS
 mainwindow.ui
 parametersWidget.ui
)

#set ( RESOURCES
# main.qrc
#)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

QT5_WRAP_CPP(HEADER_MOC ${MOC_HEADERS})
QT5_WRAP_UI(FORMS ${UIS})

IF(${SUPRA_USE_CAMPVIS})
	# The available CAMPVis linking targets are campvis-core, campvis-modules, or campvis-application-lib
	SET(CAMPVIS_LIB campvis-modules)
	SET(CAMPVIS_DEFINITIONS "${CAMPVIS_DEFINITIONS};-DHAVE_CAMPVIS")
ELSE(${SUPRA_USE_CAMPVIS})
	SET(CAMPVIS_LIB)
	SET(CAMPVIS_INCLUDE_DIRS)
	SET(CAMPVIS_DEFINITIONS)
ENDIF(${SUPRA_USE_CAMPVIS})

INCLUDE_DIRECTORIES(SUPRA_GUI
	${SUPRA_Lib_INCLUDEDIRS}
	${CAMPVIS_INCLUDE_DIRS}
)
LINK_DIRECTORIES(SUPRA_GUI
	${SUPRA_Lib_LIBDIRS}
)

add_executable(SUPRA_GUI ${SOURCES} ${HEADERS} ${MOC_HEADERS} ${FORMS} ${UIS})

TARGET_COMPILE_DEFINITIONS(SUPRA_GUI
	PRIVATE ${SUPRA_Lib_DEFINES}
	${CAMPVIS_DEFINITIONS})

if(NOT SUPRA_INTERFACE_GRAPHIC_CMD)
	# generate proper GUI program on specified platform
	if(WIN32) # Check if we are on Windows
		if(MSVC) # Check if we are using the Visual Studio compiler
			set_target_properties(SUPRA_GUI PROPERTIES
				WIN32_EXECUTABLE YES
				LINK_FLAGS "/ENTRY:mainCRTStartup"
			)
		elseif(CMAKE_COMPILER_IS_GNUCXX)
				# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows") # Not tested
		else()
			message(SEND_ERROR "You are using an unsupported Windows compiler! (Not MSVC or GCC)")
		endif(MSVC)
	elseif(APPLE)
		set_target_properties(SUPRA_GUI PROPERTIES
				MACOSX_BUNDLE YES
		)
	elseif(UNIX)
		# Nothing special required
	else()
		message(SEND_ERROR "You are on an unsupported platform! (Not Win32, Mac OS X or Unix)")
	endif(WIN32)
endif(NOT SUPRA_INTERFACE_GRAPHIC_CMD)

TARGET_LINK_LIBRARIES(SUPRA_GUI
	${SUPRA_Lib_LIBRARIES}
	${CAMPVIS_LIB}
)
set_property(TARGET SUPRA_GUI PROPERTY CXX_STANDARD 11)
set_property(TARGET SUPRA_GUI PROPERTY CXX_STANDARD_REQUIRED ON)

add_dependencies(SUPRA_GUI SUPRA_Lib)

qt5_use_modules(SUPRA_GUI Widgets)
