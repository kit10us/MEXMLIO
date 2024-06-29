cmake_minimum_required (VERSION 3.29.0)


option(TESTS "Include unit tests" FALSE)
set(CMAKE_SUPPRESS_REGENERATION ON)

if(${CMAKE_CXX_COMPILER})
	message("CMAKE_CXX_COMPILER:" ${CMAKE_CXX_COMPILER})
else()
	message("CMAKE_CXX_COMPILER not set. Can impact project settings.")
endif()
message("")

project(qxmlio CXX)

set(CMAKE_CONFIGURATION_TYPES 
	"debug;release"
	CACHE STRING "" FORCE
)


function(GenerateMain)

	set(TARGET_NAME xmlio)
	string(TOUPPER ${TARGET_NAME} UPPER_TARGET_NAME)
	set(TARGET_EXT dll)
	set(NAMESPACE xmlio)

	# Specify output directories
	foreach(CONFIG ${CMAKE_CONFIGURATION_TYPES})
		string(TOUPPER ${CONFIG} CONFIG)
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG} "lib/$(Platform)/$(Configuration)/")
		set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONFIG} "lib/$(Platform)/$(Configuration)/${TARGET_NAME}/")
		set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONFIG} "lib/$(Platform)/$(Configuration)/")
		#set(
	endforeach(CONFIG CMAKE_CONFIGURATION_TYPES)
	
	if(WIN32)
		set(CMAKE_EXECUTABLE_SUFFIX_CXX "lib")
	
		# Enable C++17 support.
		add_compile_options(/std:c++17)

		# See https://learn.microsoft.com/en-us/cpp/build/reference/eh-exception-handling-model?view=msvc-170
		add_compile_options(/EHsc)
		
		#add_compile_options(/OUT $(OutDir)$($TARGET_NAME)$(TARGET_EXT))
		
		add_compile_definitions(
			_UNICODE
			UNICODE
			_USRDLL
		)
	elseif(APPLE)
		# Enable C++17 support.
		add_compile_options(-std=c++17)
		# Older flag for Xcode: add_compile_options(-std=c++1z)
	endif()
	

	# Prepare sources
	file (GLOB MAIN_SOURCES "${NAMESPACE}/*.h" "${NAMESPACE}/*.cpp" "${NAMESPACE}/*.inl")
	
	add_library(${TARGET_NAME} MODULE ${MAIN_SOURCES})

	#... file (GLOB MAIN_SOURCES "${NAMESPACE}/*.h" "${NAMESPACE}/*.cpp" "${NAMESPACE}/*.inl")

	#... add_library(${TARGET_NAME} MODULE ${MAIN_SOURCES})
	
	target_include_directories(${TARGET_NAME} PUBLIC ../interop)
	target_include_directories(${TARGET_NAME} PUBLIC ../MercuryEngine)
	target_include_directories(${TARGET_NAME} PUBLIC ../qxml)
	target_include_directories(${TARGET_NAME} PUBLIC ../resourcemanager)
	target_include_directories(${TARGET_NAME} PUBLIC ../unify)
	target_include_directories(${TARGET_NAME} PUBLIC ../kitlogger)
	
	# SAS_TOO Remove:
	target_include_directories(${TARGET_NAME} PUBLIC ../qjson)


	# Remove unwanted compiler defintions
	remove_definitions(
		NDEBUG
		${UPPER_TARGET_NAME}_EXPORTS
	)
	
	#set(OUTPUT_NAME_DEBUG "DebugName")
	
	# Add compiler defintions
	target_compile_definitions(${TARGET_NAME} PRIVATE
		$<$<CONFIG:Debug>:_DEBUG>
		$<$<CONFIG:Release>:NDEBUG>
	)
	
	if(WIN32)
		set_target_properties(${TARGET_NAME} PROPERTIES 
			PREFIX ""
			SUFFIX ".lib"
			DEFINE_SYMBOL ${UPPER_TARGET_NAME}_EXPORTS
		)
	endif()

	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${MAIN_SOURCES})

	include_directories(.)
endfunction()

function(GenerateUnitTests)
endfunction()


GenerateMain()


if(${TESTS})
	message("Including unit tests...")
	GenerateUnitTests()
endif()