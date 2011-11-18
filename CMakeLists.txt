#
# The Madeline-2.0 Project 
# 
# (c) 2006 by the Regents of the University of Michigan
# 
#     Released under the GNU GPL
#     
PROJECT(MADELINE)
CMAKE_MINIMUM_REQUIRED(VERSION 2.4.2)
MARK_AS_ADVANCED(CMAKE_BACKWARDS_COMPATIBILITY)

if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

#
# The Madeline2 version number:
# 
SET(MADELINE_VERSION_MAJOR 2)
SET(MADELINE_VERSION_MINOR 0)
SET(MADELINE_VERSION "${MADELINE_VERSION_MAJOR}.${MADELINE_VERSION_MINOR}")

#
# Enable CTest
#
ENABLE_TESTING()

#
# Check if there exists a system-provided BZip2
#
INCLUDE(FindBZip2)
IF(BZIP2_FOUND)
        INCLUDE_DIRECTORIES(${BZIP2_INCLUDE_DIR})
        ADD_DEFINITIONS(${BZIP2_DEFINITIONS})
        SET(requiredLibs ${requiredLibs} ${BZIP2_LIBRARIES})
ELSE(BZIP2_FOUND)
	#Add bzip2 built from local copy to the requiredLibs
	INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/3rdParty/bzip2)
        SET(requiredLibs ${requiredLibs} bzip2 )
ENDIF(BZIP2_FOUND)


#
# Recurse subdirectories:
#
ADD_SUBDIRECTORY(3rdParty)
ADD_SUBDIRECTORY(po)
ADD_SUBDIRECTORY(doxygen)
ADD_SUBDIRECTORY(src)

INCLUDE_DIRECTORIES(${MADELINE_SOURCE_DIR} ${MADELINE_BINARY_DIR})
INCLUDE(${CMAKE_ROOT}/Modules/FindDoxygen.cmake)

CONFIGURE_FILE(${MADELINE_SOURCE_DIR}/doxygen/Doxyfile.in ${MADELINE_BINARY_DIR}/doxygen/Doxyfile)

INCLUDE( FindDoxygen )
IF(DOXYGEN) 
	ADD_CUSTOM_TARGET(api-docs ALL ${DOXYGEN} ${MADELINE_BINARY_DIR}/doxygen/Doxyfile DEPENDS ${MADELINE_SOURCE_DIR}/doxygen/Doxyfile.in)
ELSE(DOXYGEN)
	MESSAGE(STATUS "Unable to locate DOxygen : API Documentation will not be built!")
ENDIF(DOXYGEN)

#
# Put compiled libraries and executables in specified directories:
#
SET( LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/libs )
SET( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )


