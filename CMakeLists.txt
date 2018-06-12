# -----------------------------------------------------------------------------
# Project definition
# -----------------------------------------------------------------------------
project (DGtal)
cmake_minimum_required (VERSION 3.1)

# By default, do not warn when built on machines using only VS Express:
IF(NOT DEFINED CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_NO_WARNINGS)
    SET(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_NO_WARNINGS ON)
ENDIF()

# -----------------------------------------------------------------------------
# Define variables and cmake parameters, and display some information
# -----------------------------------------------------------------------------
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
SET(DGtal_VERSION_MAJOR 1)
SET(DGtal_VERSION_MINOR 0)
SET(DGtal_VERSION_PATCH beta)
SET(DGTAL_VERSION "${DGtal_VERSION_MAJOR}.${DGtal_VERSION_MINOR}.${DGtal_VERSION_PATCH}")
SET(PROJECT_VERSION "${DGtal_VERSION_MAJOR}.${DGtal_VERSION_MINOR}.${DGtal_VERSION_PATCH}")
SET(VERSION ${DGtal_VERSION_MAJOR}.${DGtal_VERSION_MINOR}.${DGtal_VERSION_PATCH})
# -----------------------------------------------------------------------------
# Common build options/settings
# -----------------------------------------------------------------------------
INCLUDE(Common)

# -----------------------------------------------------------------------------
# Common build options/settings
# -----------------------------------------------------------------------------
INCLUDE(OSDependentSettings)

# -----------------------------------------------------------------------------
# Check Mandatory Dependencies
# -----------------------------------------------------------------------------
INCLUDE(CheckDGtalDependencies)

# -----------------------------------------------------------------------------
# Check Optional Dependencies
# -----------------------------------------------------------------------------
INCLUDE(CheckDGtalOptionalDependencies)

# -----------------------------------------------------------------------------
# Debug, Cpack and Ctest settings
# -----------------------------------------------------------------------------
INCLUDE(CpackCtest)

# -----------------------------------------------------------------------------
# Code coverage
# -----------------------------------------------------------------------------
INCLUDE(CheckCoverage)

# -----------------------------------------------------------------------------
# Custom command/targets depending on the cmake generator
# -----------------------------------------------------------------------------
INCLUDE(GeneratorSpecific)

#------------------------------------------------------------------------------
# Add subdirectories
#------------------------------------------------------------------------------
add_subdirectory(${PROJECT_SOURCE_DIR}/src)

#------------------------------------------------------------------------------
# Examples
#------------------------------------------------------------------------------
INCLUDE(BuildExamples)

# -----------------------------------------------------------------------------
# Install settings
# -----------------------------------------------------------------------------
INCLUDE(Install)

# -----------------------------------------------------------------------------
# Unzip and install topology Look up Tables.
# -----------------------------------------------------------------------------
INCLUDE(NeighborhoodTablesConfig)

SET(CMAKE_CXX_FLAGS_RELNATIVE
    "${CMAKE_CXX_FLAGS_RELEASE} -march=native -mtune=native"
    CACHE STRING "Flags used by the CXX compiler during release builds for native architecture."
    FORCE )
SET(CMAKE_C_FLAGS_RELNATIVE
    "${CMAKE_C_FLAGS_RELEASE} -fprofile-arcs -ftest-coverage"
    CACHE STRING "Flags used by the C compiler during release builds for native architecture."
    FORCE )
SET(CMAKE_EXE_LINKER_FLAGS_RELNATIVE
    ""
    CACHE STRING "Flags used for linking binaries during release builds for native architecture."
    FORCE )
SET(CMAKE_SHARED_LINKER_FLAGS_RELNATIVE
    ""
    CACHE STRING "Flags used by the shared libraries linker during release builds for native architecture."
    FORCE )

MARK_AS_ADVANCED(
    CMAKE_CXX_FLAGS_RELNATIVE
    CMAKE_C_FLAGS_RELNATIVE
    CMAKE_EXE_LINKER_FLAGS_RELNATIVE
    CMAKE_SHARED_LINKER_FLAGS_RELNATIVE )

# Update the documentation string of CMAKE_BUILD_TYPE for GUIs
SET( CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING
    "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel RelNative."
    FORCE )

