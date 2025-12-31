cmake_minimum_required(VERSION 3.24)

option(FULL_RELEASE "Finalizes the build, compressing everything into a BSA." OFF)
option(OUTDATED "Applies 1.5.97 offsets/IDs." OFF)

# -------- Project ----------
project(
	CurrencySwapper
	VERSION 2.0.1
	LANGUAGES CXX
)

add_compile_definitions(SKYRIMVR)
add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
add_compile_definitions(SKSE_SUPPORT_XBYAK)
if (BUILD_TEST)
	add_compile_definitions(DEBUG)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Get Skyrim VR path from registry for VR build
get_filename_component(
	GAME_DIR
	"[HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Bethesda Softworks\\Skyrim VR;installed path]"
	ABSOLUTE CACHE
)

list(APPEND CMAKE_IGNORE_PATH "${PROJECT_BINARY_DIR}")
list(APPEND CMAKE_MODULE_PATH
	"${PROJECT_SOURCE_DIR}/Tools/SKSE-CMakeModules/Modules"
)
list(APPEND CMAKE_MODULE_PATH
	"${PROJECT_SOURCE_DIR}/Tools/Bethesda-CMakeModules/Modules"
)
include(SKSEPlugin)
set(SKSE_USE_XBYAK ON)
set(SKSE_SUPPORT_VR ON)
set(SKSE_COMMONLIBSSE_PATH "external/CommonLibVR")
add_compile_definitions(SKSE_SUPPORT_XBYAK)


SKSEPlugin_Add(
	${PROJECT_NAME}
	SOURCE_DIR src
	SOURCES
		README.md
		vcpkg.json
	PRECOMPILE_HEADERS
		src/common/PCH.h
)

set_target_properties(CommonLibVR PROPERTIES
	FOLDER external
)

set(DATA_DIR "${PROJECT_SOURCE_DIR}/data")

include(components.cmake)

# ---- Post build ----
if(DEFINED ENV{SKYRIM_MODS_FOLDER} AND IS_DIRECTORY "$ENV{SKYRIM_MODS_FOLDER}")
	if(OUTDATED)
		set(OUTPUT_FOLDER "$ENV{SKYRIM_MODS_FOLDER}/${PROJECT_NAME} - Outdated")
	elseif (FULL_RELEASE)
		set(OUTPUT_FOLDER "$ENV{SKYRIM_MODS_FOLDER}/${PROJECT_NAME} - Full Release")
	else()
		set(OUTPUT_FOLDER "$ENV{SKYRIM_MODS_FOLDER}/${PROJECT_NAME} - Test")
	endif()
endif()

# ---- Compile Papyrus Scripts and Pack Into Archive ----
# NOTE: Papyrus compilation disabled for VR development builds
# The .psc script is platform-agnostic - compile manually with Creation Kit when needed
# TODO: For releases, either:
#   1. Compile the script manually and include the .pex
#   2. Point GAME path to Skyrim SE installation for automated compilation
message(STATUS "Papyrus compilation skipped - compile Data/Source/Scripts/SEA_BarterFunctions.psc manually if needed")

# ---- Automatically copy artifacts to MO2 ----
if(DEFINED OUTPUT_FOLDER)
    set(DLL_FOLDER "${OUTPUT_FOLDER}/SKSE/Plugins")

    add_custom_command(
        TARGET "${PROJECT_NAME}"
        POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:${PROJECT_NAME}>" "${DLL_FOLDER}/$<TARGET_FILE_NAME:${PROJECT_NAME}>"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_PDB_FILE:${PROJECT_NAME}>" "${DLL_FOLDER}/$<TARGET_PDB_FILE_NAME:${PROJECT_NAME}>"

		COMMAND "${CMAKE_COMMAND}"
			-DSRC_DIR="${PROJECT_SOURCE_DIR}/Data"
			-DDST_DIR="${OUTPUT_FOLDER}"
			-DPROJECT_NAME="${PROJECT_NAME}"
			-P "${PROJECT_SOURCE_DIR}/CMake/Scripts/CMake-Copy-Artifacts.cmake"
    )
	add_custom_command(
        TARGET "${PROJECT_NAME}"
        POST_BUILD
        COMMAND "${CMAKE_COMMAND}"
            -DSOURCE_SCRIPTS="${PROJECT_SOURCE_DIR}/Data/Source/Scripts"
            -DCOMPILED_SCRIPTS="${CMAKE_BINARY_DIR}/Scripts"
            -DOUTPUT_FOLDER="${OUTPUT_FOLDER}"
            -P "${PROJECT_SOURCE_DIR}/CMake/Scripts/CMake-Copy-Scripts.cmake"
        VERBATIM
    )
endif()