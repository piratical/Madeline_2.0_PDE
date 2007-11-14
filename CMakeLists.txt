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

#
# The Madeline2 version number:
# 
SET(MADELINE_VERSION_MAJOR 2)
SET(MADELINE_VERSION_MINOR 0)
SET(MADELINE_VERSION "${MINIMADELINE_VERSION_MAJOR}.${MINIMADELINE_VERSION_MINOR}")

#
# Recurse subdirectories:
#
SUBDIRS(3rdParty po doxygen src)

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


