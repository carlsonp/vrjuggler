
if(__juggler_suite_default_options)
	return()
endif()
set(__juggler_suite_default_options YES)

get_filename_component(MODULE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
list(APPEND CMAKE_MODULE_PATH "${MODULE_DIR}")

if(NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
endif()
if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
endif()
if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()
if(NOT SHARE_OUTPUT_DIRECTORY)
	set(SHARE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/share)
endif()

if(WIN32)
	set(DEFAULT_VERSIONED OFF)
else()
	set(DEFAULT_VERSIONED ON)
endif()
option(BUILD_VERSIONED_DIRECTORIES "Should we version the directories for plugins and data?" ${DEFAULT_VERSIONED})
