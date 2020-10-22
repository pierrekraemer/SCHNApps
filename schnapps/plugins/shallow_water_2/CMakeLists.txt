project(plugin_shallow_water_2
	LANGUAGES CXX
)

find_package(schnapps_core REQUIRED)
find_package(cgogn_io REQUIRED)
find_package(cgogn_modeling REQUIRED)
find_package(cgogn_topology REQUIRED)
find_package(cgogn_geometry REQUIRED)

add_library(${PROJECT_NAME} SHARED "")
add_library(schnapps::shallow_water_2 ALIAS plugin_shallow_water_2)

target_sources(${PROJECT_NAME}
	PRIVATE
		"${CMAKE_CURRENT_LIST_DIR}/dll.h"
		"${CMAKE_CURRENT_LIST_DIR}/shallow_water.h"
		"${CMAKE_CURRENT_LIST_DIR}/shallow_water.cpp"
		"${CMAKE_CURRENT_LIST_DIR}/dialog_shallow_water.h"
		"${CMAKE_CURRENT_LIST_DIR}/dialog_shallow_water.cpp"
)

set_target_properties(${PROJECT_NAME} PROPERTIES
										DEBUG_POSTFIX "_d"
										EXPORT_NAME shallow_water_2)

target_compile_definitions(${PROJECT_NAME} PRIVATE "-DSCHNAPPS_PLUGIN_SHALLOW_WATER_2_DLL_EXPORT")

target_include_directories(${PROJECT_NAME} PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
	$<BUILD_INTERFACE:${SCHNAPPS_SOURCE_DIR}>
	$<BUILD_INTERFACE:${CGOGN_SOURCE_DIR}>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(${PROJECT_NAME}
	schnapps::core
        schnapps::cmap_provider
	cgogn::io
	cgogn::modeling
	cgogn::topology
	cgogn::geometry
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/schnapps/plugins/shallow_water_2
	FILES_MATCHING PATTERN "*.h"
)

schnapps_create_package("${CMAKE_CURRENT_SOURCE_DIR}")

set_target_properties (${PROJECT_NAME} PROPERTIES FOLDER plugins)
