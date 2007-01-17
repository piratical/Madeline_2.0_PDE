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

#ifndef FILETYPECLASSIFIER_INCLUDED

//
// FileTypeClassifier
//
class FileTypeClassifier {
	
public:
	
	enum FILE_TYPE { BZIP2, GZIP, XML, PKZIP, UTF8, UNKNOWN_TYPE };
	enum FILE_SUBTYPE { XML_MS, XML_OO , XML_HTML, XML_MADELINE, UNKNOWN_SUBTYPE };
	
private:
	
	static const int BUFFER_SIZE=1024; // What buffer size ?
	static const int XML_BUFFER_SIZE=26; // Last word of the XML file
	static const char UTF8_CONTINUATION_BYTE_MASK=0xC0;
	static const char UTF8_CONTINUATION_BYTE_PATTERN=0x80;
	
	char _buffer[BUFFER_SIZE+1];
	char _xmlBuffer[XML_BUFFER_SIZE+1];
	
	int _bytesToTest;
	FILE_TYPE _type;
	
	bool _readHeaderIntoBuffer(const char *fileName);
	
	enum FILE_TYPE testUTF8();
	
	
public:
	
	//
	// Constructors:
	//
	FileTypeClassifier( const char *fileName=0 );
	
	//
	// classify(): returns an enumerated value for the file type:
	//
	enum FILE_TYPE classify( void );
	
	//
	// classifyByName(): returns a string representation of the file type:
	//
	const char *classifyByName( void );
	
	//
	// setFileToTest():
	//
	bool setFileToTest( const char *fileName );
	
	//
	// OBSOLETE: DON'T USE
	//
	enum FILE_SUBTYPE getFileSubType();
	
	//
	// OBSOLETE: DON'T USE
	//
	bool checkFileSubType( const char *flag );
	
};

#endif
