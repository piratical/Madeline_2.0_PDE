/////////////////////////////////////////////////////////
//
// This file is part of the MADELINE 2 program 
// written by Edward H. Trager and Ritu Khanna
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
// 2005.02.02.ET.晚上十一點鐘
//
// 2006.12.06.ET : Originally just an "h" file, I've now broken 
//                 this class into separate "h" and "cpp" files.
//

#include "FileTypeClassifier.h"

#include <fstream>
#include <iostream>
#include <string.h>

#include "Warning.h"

///////////////////
//
// PRIVATE METHODS:
//
///////////////////

//
// _readHeaderIntoBuffer():
//
bool FileTypeClassifier::_readHeaderIntoBuffer(const char *fileName){
	
	std::ifstream fileToTest; 
	fileToTest.open( fileName , std::ios::in);
	if(!fileToTest){
		Warning("FileTypeClassifier::_readHeaderIntoBuffer()",
		        "Failed to open file %1$s.",
		        fileName
		);
		fileToTest.close();
		return false;
	}
	
	fileToTest.seekg(0,std::ios::end);
	int length=fileToTest.tellg();
	fileToTest.seekg(0);
	fileToTest.read( (char *) _buffer , BUFFER_SIZE );
	_bytesToTest = (length < BUFFER_SIZE)?length:BUFFER_SIZE;
	fileToTest.seekg(length-XML_BUFFER_SIZE);
	fileToTest.read( (char *) _xmlBuffer , XML_BUFFER_SIZE );
	fileToTest.close();
	_buffer[BUFFER_SIZE]='\0';
	_xmlBuffer[XML_BUFFER_SIZE]='\0';
	return true;
	
}

//
// Private method testUTF8
//
enum FileTypeClassifier::FILE_TYPE FileTypeClassifier::testUTF8(){
	int i=0;
	int continuationBytes=3;
	bool utf8Flag;
	char mask=0xf0;
	
	
	while(i <= _bytesToTest){
		utf8Flag=false;
		//
		// ASCII characters
		//
		if((_buffer[i] & 0xff) <= 0x7f){
			i++;
			continue;
		}
		//
		// Upper limit
		//
		if((_buffer[i] & 0xf8) == 0xf8){
			return UNKNOWN_TYPE;
		}
		if((mask & _buffer[i]) == mask){
			if(i+continuationBytes >= _bytesToTest){
				return UTF8;
			}
			// Continuation bytes
			for(int j=0;j<continuationBytes;j++){
				if((_buffer[++i] & 0xc0) == 0xc0){
					return UNKNOWN_TYPE;
				}
			}
			utf8Flag=true;
		}
		if(!utf8Flag){
			mask <<= 1;
			continuationBytes--;
		}else{
			i++;
			mask=0xf0;
			continuationBytes=3;
		}
	}
	return UTF8;
}


///////////////////
//
// PUBLIC METHODS:
//
///////////////////

//
// Constructors:
//
FileTypeClassifier::FileTypeClassifier( const char *fileName ){
	// Prophylactically clear buffer:
	for(int i=0;i<BUFFER_SIZE;i++) _buffer[i]='\0'; 
	// Type unknown:
	_type=UNKNOWN_TYPE;
	// If provided, read file header into the buffer: 
	if( fileName != 0 ) _readHeaderIntoBuffer(fileName);
}

///
/// classify(): returns an enumerated value for the file type:
///
enum FileTypeClassifier::FILE_TYPE FileTypeClassifier::classify( void ){
	
	//
	// First check the file header bytes by table lookup:
	//
	if( strncmp( _buffer , "PK\x03\x04"   , 4 )==0 ) return (_type=PKZIP); 
	if( strncmp( _buffer , "BZh"          , 3 )==0 ) return (_type=BZIP2);
	if( strncmp( _buffer , "\x1f\x8b\x08" , 3 )==0 ) return (_type=GZIP);
	if( strncmp( _buffer , "<?xml"        , 5 )==0 ) return (_type=XML);
	if( strncmp( _buffer , "<html"        , 5 )==0 ) return (_type=XML);
	if( strncmp( _buffer , "<!DOCTYPE html",14)==0 ) return (_type=XML);
	
	//
	// Get here if cannot be classified by table lookup:
	//
	return ( _type = testUTF8());
	
}

///
/// classifyByName(): returns a string representation of the file type:
///
const char * FileTypeClassifier::classifyByName( void ){
	
	classify();
	switch(_type){
		case PKZIP: return "PKZIP";
		case BZIP2: return "BZIP2";
		case GZIP:  return "GZIP";
		case XML:   return "XML";
		case UTF8:  return "UTF-8";
		default:    return "UNKNOWN";
	}
}

//
// setFileToTest():
//
bool FileTypeClassifier::setFileToTest( const char *fileName ){
	
	if(fileName!=NULL) 
		return _readHeaderIntoBuffer(fileName);
	return false;
}

//
// OBSOLETE: DON'T USE
//
enum FileTypeClassifier::FILE_SUBTYPE FileTypeClassifier::getFileSubType(){
	
	if(_type==XML){
		if( strncmp( _xmlBuffer , "</office:document-content>"   , 26 )==0 ) return XML_OO;
		if( strstr( _xmlBuffer , "</Workbook>" )) return XML_MS;
	}
	return UNKNOWN_SUBTYPE;
	
}

//
// OBSOLETE: DON'T USE
//
bool FileTypeClassifier::checkFileSubType( const char *flag )
{
	if( strstr( _xmlBuffer , flag )) return true;
	
	return false;
}



