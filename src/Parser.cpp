/////////////////////////////////////////////////////////
//
// This file is part of the MADELINE 2 program 
// written by Edward H. Trager, Ritu Khanna and Adrian Marrs
// Copyright (c) 2005, 2006 by the
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
// Parser.cpp
//

#include "Parser.h"
#include "Compressor.h"
#include "Network.h"
#include "XMLTagManager.h"


#ifdef MYSQLPP
#include "Database.h"
#endif

#include "DelimitedTableParser.h"
#include "MadelineTable.h"

#include <fstream>

// DEBUG:
#include <iostream>

Parser::Parser(){};

//
// ~Parser destructor:
//
Parser::~Parser()
{
	
	
	std::vector<DataTable *>::iterator it = _tables.begin();
	
	for(it = _tables.begin() ; it != _tables.end() ; ++it ){
		
		delete *it;
		
	}
	_tables.clear();
	return;
	
}


///
/// readFile: Reads a local file on disk
///
void Parser::readFile(const std::string &fileName){
	
	_fileName = fileName;
	
	//////////////////////////////////////////////////////////////////////////////
	//
	// First find out what kind of file it is using the fileTypeClassifier class:
	//
	//////////////////////////////////////////////////////////////////////////////
	
	//
	// FileTypeClassifier is now a local instance only:
	//
	FileTypeClassifier fileTypeClassifier;
	
	if(!fileTypeClassifier.setFileToTest(_fileName.c_str())){
		Warning("In Parser::readFile(), File %s cannot be located or read and will be ignored",
		        _fileName.c_str() );
		return;
	}
	
	_fileType     = fileTypeClassifier.classify();
	_fileTypeName = fileTypeClassifier.classifyByName();
	
	std::string fileData;
	Compressor newCompressor;
	
	//
	// Message for user:
	//
	std::cout << "Parser::readFile(): Opening a(n) " << _fileTypeName << " file ...\n";
	
	//
	// If the file is compressed, decompress it into a std::string. 
	// Otherwise, read it into a std::string.
	//
	if( _fileType==FileTypeClassifier::PKZIP || 
	    _fileType==FileTypeClassifier::GZIP  || 
	    _fileType==FileTypeClassifier::BZIP2
	)
	{
		//
		// Decompress into a std::string :
		//
		std::cout << "Parser::readFile(): Decompressing " << _fileTypeName << " file ...\n";
		fileData = newCompressor.decompress(_fileName);
	}
	else
	{
		//
		// Read into a std::string :
		//
		fileData = _stringify(_fileName);
	}
	
	////////////////////////////////////////
	//
	// Now process the fileData std::string:
	//
	////////////////////////////////////////
	
	if( _xmlTagManager.discoverFormat(fileData) )
	{
		//
		// fileData are in a recognized XML format:
		//
		std::cout << "Reading file data in " << _xmlTagManager.getFormatName() << " format ...\n";
		_readXML(fileData);
		
	}
	else
	{
		// 
		// If it is not an XML file, then it is a flat file.
		// Two flat file formats are recognized:
		//  
		//  (1) Original Madeline format    (2) Delimited format
		//
		// If there is only 1 block, then it is a tab delimited file. 
		// If there are 2 blocks, it is in Madeline format:
		//
		int blockCount = _determineNumberOfBlocks( fileData );
		if( blockCount == 1)
		{
			
			std::cout << "Reading file data in delimited format ...\n";
			_readDelimited(fileData);
		}
		else if( blockCount == 2)
		{
			
			std::cout << "Reading file data in Madeline flat file format ...\n";
			_readMadeline(fileData);
			
		}
		else
		{
			
			Warning("Parser::readFile(): Sorry, the format of the file %s could not be determined and will be ignored.",
			        _fileName.c_str()
			);
			
		}
	}
}


///
/// readNetworkFile(): Reads a file across the network
///
void Parser::readNetworkFile(const std::string &url)
{
	
	_url = url;
	
	try
	{
	
		//
		// Message for user:
		//
		std::cout << "Parser::readNetworkFile(): Opening network file " << _url << " ...\n";
		
		Network net;
		//
		// The Network object reads the file and
		// places it in a local file in /tmp:
		//
		net.getNetworkFile( _url.c_str() );
		//
		// Then just read the /tmp file like any other:
		//
		readFile( net.getFileName() );
		
		// Set flag if read successfully:
		
		_isANetworkFile=true;
		
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	    
}

#ifdef MYSQLPP
void Parser::connectMysql(std::string host,std::string port, std::string user, std::string password, std::string database)
{
	
	if(port == std::string("")){
		_currentMysqlTable.setup(host.c_str(),user.c_str(),password.c_str(),database.c_str());
	}else{
		// Convert port to an unsigned
		unsigned mysqlport;
		std::istringstream str(host);
		str >> mysqlport;
		_currentMysqlTable.setup(host.c_str(),user.c_str(),password.c_str(),database.c_str(),mysqlport);
	}
	  
}

void Parser::readMysql(std::string tableName)
{
	
	//
	// Setup select statement using table name:
	//
	std::string SQLQuery = "select * from ";
	SQLQuery += tableName;
	
	//
	// Read in table using mysql parser
	//
	_currentMysqlTable.readTable(SQLQuery.c_str());
	
	// DEBUG:
	//_currentMysqlTable.display();
	
	//
	// Create a new DataTable:
	//
	DataTable *sqlTable = new DataTable( _currentMysqlTable );
	//
	// Push the data table pointer onto the _tables vector:
	//
	_tables.push_back(sqlTable);  
	
}

#endif

//
// _stringify
//
std::string Parser::_stringify(std::string fileName)
{
	std::ifstream dtdreader;
	std::string temp;
	int length = 1;
	char * buffer;
	
	dtdreader.open(fileName.c_str());
	
	//
	// get file length
	//
	dtdreader.seekg(0, std::ios::end);
	length = dtdreader.tellg();
	dtdreader.seekg(0, std::ios::beg);
	
	//
	// allocate memory
	//
	buffer = new char [length+1];
	
	// read data as a block
	dtdreader.read (buffer,length);
	dtdreader.close();
	buffer[length] = '\0';
	
	char *p;
	char *end;
	//
	// Loop to copy buffer and normalize all End-of-line characters:
	//
	for(p=buffer,end=buffer+length;p<end;p++){
		
		if (*p == '\r'){
			
			if(p<end-1 && *(p+1) == '\n'){
				//
				// DOS case : normalize to a single '\n':
				//
				temp += '\n';
				//
				// skip over the 2nd DOS "EOL" character:
				//
				p++;
			}else{
				//
				// Old MAC case : normalize to '\n':
				//
				temp += '\n';
			}
			
		}else if(*p == (char) 0xE2 && p<end-2 && *(p+1)== (char) 0x80 && *(p+2)== (char) 0xA9){
			//
			// Unicode U+2029 Paragraph Separator in UTF-8 case: normalize to '\n':
			//
			temp += '\n';
			//
			// Skip over remaining bytes of the UTF-8-encoded paragraph separator:
			//
			p+=2;
			
		}else{
			//
			// Default case: just copy byte by byte:
			//
			temp += *p;
		}
	}
	
	delete [] buffer;                                                                    
	
	return temp;
	
}

//
// _determineNumberOfBlocks()
//
// Used to determine the number of "blocks" in a flat text file
//
int Parser::_determineNumberOfBlocks( std::string inString)
{
	unsigned int i=0;
	bool firstBlockFound = false;
	bool charInCurLine = false;
	bool breakFound = false;
	
	// skip white space
	while(!firstBlockFound)
	{
		if(i == inString.length()) return 0;
		if(!(inString[i] == '\t' || inString[i] == ' ' || inString[i] == '\n')) 
		{
			firstBlockFound = true;
		}
		
		i++;
	}
	
	for(; i < inString.length() && inString[i] != '\n'; i++);
	
	if(i == inString.length()) return 1;
	else i++;
	
	// skip first block
	while(!breakFound)
	{
		if(i >= inString.length()) return 1;
		if(!(inString[i] == '\t' || inString[i] == ' ' || inString[i] == '\n')) 
		{
			for(; i < inString.length() && inString[i] != '\n'; i++);
			charInCurLine = true;
		}
		else if(inString[i] == '\n')
		{
			breakFound = true;
		}
		i++;
	}
	
	// skip white space
	while(inString[i] == ' ' || inString[i] == '\t' || inString[i] == '\n')
	{
		if(i == inString.length()) return 1;
		
		i++;
	}
	// return if 2nd block found
	return 2;
	
}

//
// _countTables:
//
void Parser::_countTables(xmlNode *node)
{
	
	xmlNode *currentNode = NULL;
	// Cycle through all nodes in the XML table, starting with node
	for(currentNode = node; currentNode; currentNode = currentNode->next) 
	{
		if(currentNode->type == XML_ELEMENT_NODE) 
		{
			if(!strcmp((char *)currentNode->name,_xmlTagManager.getTableTagName().c_str()))
			{
				// if a table node is found, put a pointer to it in _pointers
				_xmlTableTagPointers.push_back(currentNode);
			}
		}
		_countTables(currentNode->children);
	}
	
}

//
// _readXML:
//
void Parser::_readXML(std::string data)
{                              
	
//	xmlNode *currentNode = NULL;  
	
	XMLTableParser xmlParser( &_xmlTagManager );   // initialize the XML parser
	
	xmlDoc *document;                      // XML document
	xmlNode *rootElement; 
	
	//
	// VALGRIND ERROR --------- UNABLE TO FIX IT //
	//
	document = xmlReadMemory(data.c_str(), data.size(),NULL, NULL, 0);
	rootElement = xmlDocGetRootElement(document);
	//
	// Store pointers to all of the table nodes:
	//
	_countTables(rootElement);
	
	//
	// Message for user:
	//
	unsigned tableCount = _xmlTableTagPointers.size();
	std::cout << "There " <<  (tableCount==1?"is ":"are ") << tableCount ;
	std::cout << " table" << (tableCount==1?"":"s") << std::endl; 
	
	//
	// Read each table in the XML file into a DataTable:
	//
	for(unsigned int i=0; i < tableCount; i++){  
		if( xmlParser.readString(data,_xmlTableTagPointers[i]) ){
			
			DataTable *xmlTable = new DataTable(xmlParser);
			_tables.push_back( xmlTable );
			
		}
	}
	
	free(document);
	free(rootElement);
	
	_xmlTableTagPointers.clear();         
	std::cout << "End of Parser::_readXML() method." << std::endl;
	
}

//
// _readDelimited:
//
void Parser::_readDelimited(std::string data)
{ 
	
	
	DelimitedTableParser delimitedTableParser;
	delimitedTableParser.readString( data );
	
	//
	// DEBUG:
	//
	//delimitedTableParser.display();
	//
	
	DataTable *delimitedTable = new DataTable( delimitedTableParser );
	
	_tables.push_back( delimitedTable );
	
}

//
// _readmadeline
//
void Parser::_readMadeline(std::string data)
{
	
	MadelineTable madelineTableParser;
	madelineTableParser.setString(data);
	
	DataTable *madelineTable = new DataTable( madelineTableParser );
	_tables.push_back( madelineTable );
	
}

//
// display
//
void Parser::display(){
	
	for(unsigned int i=0; i<_tables.size(); i++) 
	{
		std::cout << "TABLE " << i << std::endl;
		_tables[i]->display();
		std::cout << std::endl << std::endl << std::endl;
	}
	
}


