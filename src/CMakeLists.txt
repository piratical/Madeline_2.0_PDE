#
# The Madeline-2.0 Project 
# 
# (c) 2006, 2007 by the Regents of the University of Michigan
# 
#     Released under the GNU GPL
#
SET( SOURCES 
	
	Affected.cpp 
	Affected.h 
	
	Boolean.cpp 
	Boolean.h 
	BufferSizes.h 
	
	Carrier.cpp
	CLP.cpp
	ColorSeries.cpp
	ColorSeriesStack.cpp
	ColumnClassifier.cpp 
	ColumnClassifier.h 
	ColumnOffset.h 
	Compressor.cpp 
	Compressor.h
	Consultand.cpp 
	
	Data.cpp 
	Data.h 
	DataColumn.cpp 
	DataColumn.h 
	DataTable.cpp 
	DataTable.h 
	DataTypes.h 
	Date.cpp 
	Date.h
	Debug.cpp
	DelimitedTableParser.cpp 
	DelimitedTableParser.h 
	DescentTree.cpp
	DigitConverter.cpp 
	DigitConverter.h
	Donor.cpp
	DrawingColor.cpp
	DrawingLabelManager.cpp
	DrawingMetrics.cpp
	DrawingCanvas.cpp
	
	Exception.h
	Exception.cpp 
	
	FieldLabels.h
	FieldLabels.cpp 
	FileTypeClassifier.h
	FileTypeClassifier.cpp 
	
	Gender.cpp 
	Gender.h 
	Genotype.cpp 
	Genotype.h 
	Grid.h
	
	Haplotype.cpp 
	Haplotype.h 
	HexavigesimalConverter.h
	HexavigesimalConverter.cpp
	
	IconLegend.h
	Individual.h
	Individual.cpp
	Infertility.h
	Infertility.cpp

	LabelLegend.h
	LabelSet.cpp 
	LASiWrapper.h
	LASiWrapper.cpp
	Legend.h
	LineInformation.h 
	LivingDead.cpp
	
	MadelineTable.cpp 
	MadelineTable.h
	Message.h
	Message.cpp
	
	Network.cpp 
	Network.h 
	NuclearFamily.cpp
	Number.cpp 
	Number.h 
	
	ObligateCarrier.cpp
	
	Parser.cpp 
	Parser.h
	PedigreeSet.cpp
	Pedigree.cpp
	Pregnancy.cpp
	Proband.cpp
	
	RandomId.cpp
	RandomSampleSizes.h 
	RandomGenerator.cpp 
	RandomGenerator.h 
	RelationshipEnded.h
	RelationshipEnded.cpp
	
	Sampled.cpp
	SampleQuantity.cpp
	ScriptCodes.h 
	Sterility.h
	Sterility.cpp
	String.cpp 
	String.h 
	SVG.cpp
	
	TableParser.h
	Twin.cpp
	
	UnicodeScript.cpp
	UniqueList.cpp
	Utf8String.h
	Utf8String.cpp
	Utf8String.h
	Utility.cpp
	
	VT100.cpp
	
	Warning.cpp 
	Warning.h 
	
	XMLTableParser.cpp 
	XMLTableParser.h 
	XMLTagManager.cpp
	
	main.cpp 
	
)

#
# Where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked:
# 
SET(CMAKE_MODULE_PATH "${MADELINE_SOURCE_DIR}/cmake/modules")

#
# Include FindPkgConfig.cmake
#
INCLUDE( UsePkgConfig )
FIND_PACKAGE( LibXml2 REQUIRED )
IF( LIBXML2_FOUND )
	INCLUDE_DIRECTORIES(${LIBXML2_INCLUDE_DIR})
	ADD_DEFINITIONS(${LIBXML2_DEFINITIONS})
ENDIF( LIBXML2_FOUND )


#
# LibIntl support:
#
INCLUDE( FindLibintl )
IF( LIBINTL_FOUND )
	INCLUDE_DIRECTORIES(${LIBINTL_INCLUDE_DIR})
	SET(optionalLibs ${optionalLibs} ${LIBINTL_LIBRARIES})
ENDIF( LIBINTL_FOUND )

#
# Libcurl support:
#
INCLUDE( FindLibcurl )
IF( LIBCURL_FOUND )
	INCLUDE_DIRECTORIES(${LIBCURL_INCLUDE_DIR})
	SET(requiredLibs ${requiredLibs} ${LIBCURL_LIBRARIES})
ENDIF( LIBCURL_FOUND )

#
# Optional MySQL++ support:
#
OPTION( WITH_MYSQLPP_SUPPORT "Build Madeline with MySQL database support" OFF )

IF( WITH_MYSQLPP_SUPPORT )
	
	FIND_PACKAGE( MySQL REQUIRED)
	FIND_PACKAGE( MySQL++ REQUIRED)	
	IF (MYSQL_FOUND AND MYSQLPP_FOUND)
		MESSAGE(STATUS "=== NOTE BENE: === Madeline will be built with MySQL support.")
		SET_SOURCE_FILES_PROPERTIES( Database.h DataTable.h Parser.h  Parser.cpp Database.cpp DataTable.cpp main.cpp COMPILE_FLAGS -DMYSQLPP )
		INCLUDE_DIRECTORIES(${MYSQL_INCLUDE_DIR} ${MYSQLPP_INCLUDE_DIR})
		SET(optionalLibs ${optionalLibs} ${MYSQL_LIBRARIES} ${MYSQLPP_LIBRARIES})
		SET( SOURCES ${SOURCES} Database.cpp )
	ENDIF(MYSQL_FOUND AND MYSQLPP_FOUND)
	
ENDIF( WITH_MYSQLPP_SUPPORT )

#
# Optional LASi support:
#
OPTION( WITH_LASi "Build Madeline with LASi support" OFF )

IF( WITH_LASi )
#
# Check for Pango: Pango and Pangoft2 are combined here.
#                  Pango is required for Unicode text layout support.
#                  
PKGCONFIG(pango  PANGO_INCLUDE_DIR PANGO_LINK_DIR PANGO_LINK_FLAGS PANGO_CFLAGS)
IF(PANGO_INCLUDE_DIR)
	INCLUDE_DIRECTORIES(${PANGO_INCLUDE_DIR})
	LINK_DIRECTORIES(${PANGO_LINK_DIR})
	ADD_DEFINITIONS(${PANGO_CFLAGS})
ELSE (PANGO_INCLUDE_DIR)
	MESSAGE(FATAL_ERROR "Pango (http://ftp.gnome.org/pub/GNOME/sources/pango/) is required to build Madeline.")
ENDIF(PANGO_INCLUDE_DIR)

PkGCONFIG(pangoft2 PANGOFT2_INCLUDE_DIR PANGOFT2_LINK_DIR PANGOFT2_LINK_FLAGS PANGOFT2_CFLAGS)
IF(PANGOFT2_INCLUDE_DIR)
	INCLUDE_DIRECTORIES(${PANGOFT2_INCLUDE_DIR})
	LINK_DIRECTORIES(${PANGOFT2_LINK_DIR})
	ADD_DEFINITIONS(${PANGOFT2_CFLAGS})
ELSE (PANGOFT2_INCLUDE_DIR)
	MESSAGE(FATAL_ERROR "Pangoft2 not found")
ENDIF(PANGOFT2_INCLUDE_DIR)

#
# Check for LASi: LASi is required for Unicode text layout and font metrics support.
#
PKGCONFIG(lasi LASI_INCLUDE_DIR LASI_LINK_DIR LASI_LINK_FLAGS LASI_CFLAGS)
IF(LASI_INCLUDE_DIR)
	INCLUDE_DIRECTORIES(${LASI_INCLUDE_DIR})
	LINK_DIRECTORIES(${LASI_LINK_DIR})
	ADD_DEFINITIONS(${LASI_CFLAGS})
ELSE (LASI_INCLUDE_DIR)
	MESSAGE(FATAL_ERROR "LASi (http://unifont.org/lasi/) is required to build Madeline.")
ENDIF(LASI_INCLUDE_DIR)
MESSAGE(STATUS "=== NOTA BENE: === Madeline will be built with LASi support.")
ADD_DEFINITIONS(-DLASI)
ENDIF(WITH_LASi)


#
# Check for SSL Library: SSL is required for network support.
#
FIND_LIBRARY(libssl NAMES ssl PATHS /usr/lib /usr/local/lib /usr/local/opt/openssl/lib )
IF (NOT libssl)
	MESSAGE(FATAL_ERROR "Missing LIBSSL library")
ENDIF (NOT libssl)

#
# Check for crypto library:
#
FIND_LIBRARY(libcrypto NAMES crypto PATHS /usr/lib /usr/local/lib /usr/local/opt/openssl/lib )
IF(NOT libcrypto)
	MESSAGE(FATAL_ERROR "Missing libcrypto library")
ELSE (libcrypto)
	SET(requiredLibs ${requiredLibs} ${libcrypto})
ENDIF(NOT libcrypto)

#
# Check for zlib:
#
FIND_PACKAGE(ZLIB REQUIRED)
IF(ZLIB_FOUND)
	SET(requiredLibs ${requiredLibs} ${ZLIB_LIBRARIES})
ELSE(ZLIB_FOUND)
	MESSAGE(FATAL_ERROR "Missing libz library")
ENDIF(ZLIB_FOUND)

#
# Config.h File generation:
#
INCLUDE (CheckIncludeFiles)
# usage: CHECK_INCLUDE_FILES (<header> <RESULT_VARIABLE> )
CHECK_INCLUDE_FILES (malloc.h HAVE_MALLOC_H)
CHECK_INCLUDE_FILES ("sys/param.h;sys/mount.h" HAVE_SYS_MOUNT_H)
SET( PACKAGE madeline2 )
SET( LOCALEDIR ${CMAKE_INSTALL_PREFIX}/share/locale )
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake ${MADELINE_BINARY_DIR}/config.h)
INCLUDE_DIRECTORIES( ${MADELINE_BINARY_DIR} )

#
# Executables:
#
ADD_EXECUTABLE(madeline2 ${SOURCES} )

#
# Link target:
#
TARGET_LINK_LIBRARIES(madeline2 
	unzip
	${requiredLibs} 
	${libssl} 
	${PANGO_LINK_FLAGS}
	${PANGOFT2_LINK_FLAGS} 
	${LASI_LINK_FLAGS}
	${optionalLibs}
	${LIBXML2_LIBRARIES} 
	stdc++	
)

#
# Installation directory:
#
MESSAGE(STATUS "=== NOTA BENE: === Madeline will be installed in ${CMAKE_INSTALL_PREFIX}")

#
# Installable items:
#
INSTALL_TARGETS( /bin madeline2 )

