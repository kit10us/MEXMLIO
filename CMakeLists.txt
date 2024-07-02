cmake_minimum_required (VERSION 3.30.0)

set(CMAKE_SUPPRESS_REGENERATION ON)

if(${CMAKE_CXX_COMPILER})
	message("CMAKE_CXX_COMPILER:" ${CMAKE_CXX_COMPILER})
else()
	message("CMAKE_CXX_COMPILER not set. Can impact project settings.")
endif()
message("")

include(cmakeutil/Functions.txt)


project(MEXMLIO CXX)

#set(CMAKE_CONFIGURATION_TYPES 
#	"Debug;Release"
#	CACHE STRING "" FORCE
#)


# TODO: Add resource manager lib.

function(GenerateMain)
	set(TARGET_NAME MEXMLIO)
	string(TOUPPER ${TARGET_NAME} UPPER_TARGET_NAME)
	set(TARGET_EXT dll)
	set(NAMESPACE xmlio)

	# Specify output directories
	foreach(CONFIG ${CMAKE_CONFIGURATION_TYPES})
		string(TOUPPER ${CONFIG} CONFIG)
		#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG} "lib/$(Platform)/$(Configuration)/")
		set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONFIG} "${CMAKE_CURRENT_SOURCE_DIR}../../extensions/$(Platform)/$(Configuration)/")
		#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONFIG} "lib/$(Platform)/$(Configuration)/")

		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG} "TEST_A")
		
		# Output for the lib and exp of the dll
		#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONFIG} "TEST_C")
	endforeach(CONFIG CMAKE_CONFIGURATION_TYPES)
	
	if(MSVC)
		set(CMAKE_EXECUTABLE_SUFFIX_CXX "lib")
	
		# Enable C++17 support.
		add_compile_options(/std:c++17)

		# See https://learn.microsoft.com/en-us/cpp/build/reference/eh-exception-handling-model?view=msvc-170
		add_compile_options(/EHsc)
		
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
	
	if(MSVC)
		AddTargetLibrary(${TARGET_NAME} unify ${CMAKE_CURRENT_SOURCE_DIR}/../unify)
		
		AddTargetLibraryOld(${TARGET_NAME} ResourceManager ${CMAKE_CURRENT_SOURCE_DIR}/../resourcemanager)
		AddTargetLibraryOld(${TARGET_NAME} qxml ${CMAKE_CURRENT_SOURCE_DIR}/../qxml)
		
		#set(RESOURCEMANAGER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../resourcemanager)
		#set(RESOURCEMANAGER_VCXPROJ_FILE ${RESOURCEMANAGER_DIR}/ResourceManager.vcxproj)
		#if(EXISTS ${RESOURCEMANAGER_VCXPROJ_FILE})
		#	message("Including Resource Manager project")
		#	include_external_msproject("ResourceManager" ${RESOURCEMANAGER_VCXPROJ_FILE})
		#	add_dependencies(${TARGET_NAME} "ResourceManager")
		#else()
		#	message("Cannot find unify project, skipping (${UNIFY_DIR}/build/unify.vcxproj)")
		#endif()		
		#target_include_directories(${TARGET_NAME} PUBLIC ../resourcemanager)
	endif()

	target_include_directories(${TARGET_NAME} PUBLIC ../interop)
	#target_include_directories(${TARGET_NAME} PUBLIC ../qxml)
	
	target_include_directories(${TARGET_NAME} PUBLIC ../MercuryEngine)
	target_include_directories(${TARGET_NAME} PUBLIC ../kitlogger)
	
	
	# SAS_TOO Remove:
	target_include_directories(${TARGET_NAME} PUBLIC ../qjson)
	
	
	target_link_directories(${TARGET_NAME}
		PUBLIC "$<$<CONFIG:Debug>:${CMAKE_SOURCE_DIR}/../lib/x64/DebugWindows>"
		PUBLIC "$<$<CONFIG:Debug>:${CMAKE_SOURCE_DIR}/../unify/build/Debug>"

		PUBLIC "$<$<CONFIG:Release>:${CMAKE_SOURCE_DIR}/../lib/x64/ReleaseWindows>"
		PUBLIC "$<$<CONFIG:Release>:${CMAKE_SOURCE_DIR}/../unify/build/Release>"
	)	
	

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
	
	if(MSVC)
		set_target_properties(${TARGET_NAME} PROPERTIES 
			PREFIX ""
			#SUFFIX ".lib"
			DEFINE_SYMBOL ${UPPER_TARGET_NAME}_EXPORTS
		)
	endif()
	
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${MAIN_SOURCES})

	include_directories(.)
	
	# Add link libraries
	target_link_libraries(${TARGET_NAME} unify.lib)	
	#target_link_libraries(${TARGET_NAME} ResourceManager.lib)
	#target_link_libraries(${TARGET_NAME} QXML.lib)
	
	target_link_libraries(${TARGET_NAME} QJson.lib)
	
endfunction()


GenerateMain()