/////////////////////////////////////////////////////////
//
// This file is part of the MADELINE 2 program 
// written by Edward H. Trager, Ritu Khanna and Adrian Marrs
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
// Compressor.h
//

#ifndef COMPRESSOR_INCLUDED
#define COMPRESSOR_INCLUDED

#include <string>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/debugXML.h>

/*!
 * This class is used to decompress files, and then return a string with the decompressed file. It can decompress PKZIP, GZIP, and BZIP2 formats.
*/
class Compressor
{
public:
	enum COMPRESSORFORMAT { ZIP, GZIP, BZIP2 };
	enum DECOMPRESSORTYPE { INFILE, INSTREAM };
	
	const static int CASESENSITIVITY = 1;   // Casesensitivity of XML filename
	const static int WRITEBUFFERSIZE = 8192;// Buffer size to extract the XML file
	
	Compressor(){}
	
//	void compress(const std::string& file, COMPRESSORFORMAT format = GZIP) {}
	std::string decompress(const std::string& file, const std::string& contents= "content.xml", DECOMPRESSORTYPE type = INFILE);
//	void decompressToFile(const std::string& file, const std::string& outFile= "unzippedconent.xml", const std::string& contents= "content.xml", DECOMPRESSORTYPE type = INFILE){}
	
//	void getTableOfContents() {}

private:

	std::string _decompressPKZIP(const std::string& file, const std::string& contents= "content.xml");
	std::string _decompressGZIP(const std::string& file);
	std::string _decompressBZIP2(const std::string& file);

};
#endif

