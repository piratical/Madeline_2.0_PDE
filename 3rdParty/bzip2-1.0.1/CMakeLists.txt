

SET(SOURCES
	blocksort.c 
	bzip2.c 
	bzip2recover.c 
	bzlib.c 
	compress.c 
	crctable.c 
	decompress.c 
	huffman.c 
	randtable.c  
)

ADD_LIBRARY(bzip2 STATIC ${SOURCES} )
SET(requiredLibs ${requiredLibs} bzip2)
