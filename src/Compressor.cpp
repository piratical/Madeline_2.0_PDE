/////////////////////////////////////////////////////////
//
// This file is part of the MADELINE 2 program 
// written by Edward H. Trager, Ritu Khanna, and Adrian Marrs
// Copyright (c) 2005 by the
// Regents of the University of Michigan.
// All Rights Reserved.
// 
// The latest version of this program is available from:
// 
//   http://eyegene.ophthy.med.umich.edu/madeline/
//   
// Released under the GNU General Public License.
// A copy of the GPL is included in the distribution
// package of this software, or see:
// 
//   http://www.gnu.org/copyleft/
//   
// ... for licensing details.
// 
/////////////////////////////////////////////////////////

//
// Compressor.cpp
//

#include "Compressor.h"
#include "FileTypeClassifier.h"


#include <fstream>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "../3rdParty/unzip/unzip.h"
//#include "../3rdParty/zlib-1.2.2/zlib.h"
#include <zlib.h>
#include "../3rdParty/bzip2-1.0.1/bzlib.h"
#include "Exception.h"


//////////////////////////////////////
//
// PRIVATE METHODS
//
//////////////////////////////////////

//
// _decompressPKZIP:
//
// NOTA BENE: "contents" will normally be "content.xml" for an OASIS PKZIP'd file.
//
std::string Compressor::_decompressPKZIP(const std::string& file, const std::string& contents)
{
	int bytesRead;
	void* buf;
	unzFile unzipFile=NULL;
	std::string result;
	int total = 0;
	
	//
	// Open the file
	//
	if( !file.empty()){
		unzipFile = unzOpen(file.c_str());
		if(unzipFile == NULL){
			throw Exception("Compressor::_decompressPKZIP()","Cannot open zip file");
		}
	}
	//
	// Locate the file to extract in the zip file
	// 
	if(unzLocateFile(unzipFile,contents.c_str(),CASESENSITIVITY) != UNZ_OK){
		//
		// Could not find the file specified in contents.c_str (usually "content.xml")
		// So ignore the file name and just try to open the first available file
		// instead of throwing an exception.  If the first file turns out to be an
		// XML file, then we should be able to read it:
		//
		unzGoToFirstFile(unzipFile);
		//
		// No, we don't want to do this, do we? 
		// No. ;-) :
		// 
		// throw Exception("Compressor::_decompressPKZIP()","Cannot find the requested zipfile");
		//
	}
	
	//
	// Allocate buffer:
	//
	buf = (void*)malloc(WRITEBUFFERSIZE);
	if (buf == NULL){
		throw Exception("Compressor::_decompressPKZIP()","Error allocating memory for the XML file buffer ");
	}
	
	//
	// Start reading the file:
	//
	bytesRead = unzOpenCurrentFile(unzipFile);
	if (bytesRead != 0){
		throw Exception("Compressor::_decompressPKZIP()","Error opening zip file");
	}
  
	do{
		//
		// Extract the file:
		//
		bytesRead = unzReadCurrentFile(unzipFile,buf,WRITEBUFFERSIZE);
		if (bytesRead < 0){
			bytesRead = unzCloseCurrentFile(unzipFile);
			free(buf);
			unzCloseCurrentFile(unzipFile);
			free(unzipFile);
			throw Exception("Compressor::_decompressPKZIP()","Error reading zip file");
		}
		if (bytesRead > 0){
			total += bytesRead;
			result.append((char*)buf, bytesRead);
		}
	}while (bytesRead!=0);
	
	result.resize(total);
	
	//
	// Close the opened files
	//
	
	if (bytesRead == 0){
		bytesRead = unzCloseCurrentFile (unzipFile);
		if (bytesRead!=0){
			free(buf);
			unzCloseCurrentFile(unzipFile);
			throw Exception("Compressor::_decompressPKZIP()","Error closing zip file");
		}
	}
	else{
		unzCloseCurrentFile(unzipFile); 
	}
	free(buf);
	unzCloseCurrentFile(unzipFile);
	free(unzipFile);  // unzClose does not free the memory
	
	return result;
	
}

//
// _decompressGZIP:
//
std::string Compressor::_decompressGZIP(const std::string& file)
{
	std::string result;
	int numBytes;
	Byte *uncompr;
	gzFile gfile;
	
	
	uLong uncomprLen = 10000*sizeof(int); /* don't overflow on MSDOS */
	uncompr = (Byte*)calloc((uInt)uncomprLen, 1);
	
	strcpy((char*)uncompr, "garbage");


	// open the file

	gfile = gzopen(file.c_str(), "rb");
	

	// decompress the file
	do
	{
		numBytes = gzread(gfile, uncompr, (unsigned)uncomprLen);
		if(numBytes > 0) result.append((char*)uncompr, numBytes);
		
		
	}while(numBytes > 0);
	
	free(uncompr);
//	free(gfile);

	return result;
}
 
//
// _decompressBZIP2:
//
std::string Compressor::_decompressBZIP2(const std::string& file)
{
	std::string result;
	int numBytes;
	int err;
	FILE* b_file = fopen(file.c_str(),"r");
	BZFILE* bz_file;
	
	
	void *uncompr;
	
	uLong uncomprLen = 10000*sizeof(int); /* don't overflow on MSDOS */
	uncompr = malloc((uInt)uncomprLen);
	

	// open the file
	bz_file =  BZ2_bzReadOpen(&err, b_file, 0, 0, NULL, 0);
	
	// decompress the file
	do
	{
		numBytes = BZ2_bzRead(&err, bz_file, uncompr, (unsigned)uncomprLen);
		if(numBytes > 0) result.append((char*)uncompr, numBytes);
		
		
	}while(numBytes > 0);

	free(uncompr);

	return result;
}

//////////////////////////////////////
//
// PUBLIC METHODS
//
//////////////////////////////////////

//
// decompress:
//
// NOTA BENE:
//
// 1. contents defaults to "content.xml" which only applies to OASIS format
// 2. DECOMPRESSORTYPE defaults to "INFILE"
//
std::string Compressor::decompress(const std::string& file, const std::string& contents, Compressor::DECOMPRESSORTYPE type)
{
	std::string result;
	FileTypeClassifier classifier(file.c_str());
	
	if     (classifier.classify() == FileTypeClassifier::PKZIP && type == INFILE) result = _decompressPKZIP(file, contents);
	else if(classifier.classify() == FileTypeClassifier::GZIP  && type == INFILE) result = _decompressGZIP(file);
	else if(classifier.classify() == FileTypeClassifier::BZIP2 && type == INFILE) result = _decompressBZIP2(file);
	
	return result;
	
}
