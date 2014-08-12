/////////////////////////////////////////////////////////
//
// This file is part of the MADELINE 2 program 
// written by Edward H. Trager, Adrian Marrs and Ritu Khanna
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
// XMLTableParser.cpp
//

#include <iostream>
#include "XMLTableParser.h"

#include "FileTypeClassifier.h"

#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
//#include <dirent.h>
#include <fstream>
#include <sstream>
#include <string.h>


//const char* XMLTableParser::XMLFILENAME="content.xml";
///
/// Parse the XML file and retrieve all the data into a vector
/// node is the root node of the XML document
///

///////////////////////////////////
//
// CONSTRUCTORS/DESTRUCTORS:
//
///////////////////////////////////

//
// XMLTableParser Constructor
//
XMLTableParser::XMLTableParser(const XMLTagManager* p)
{
	_pXMLTagManager = p;
}


///////////////////////////////////
//
// PRIVATE METHODS:
//
///////////////////////////////////

//
// _tagNameMatches
//
bool XMLTableParser::_tagNameMatches(const xmlChar *xmlTagName,std::string referenceTagName){
	
	return !strcmp( (const char *) xmlTagName , referenceTagName.c_str() );
	
}

//
// _getRepeatCount
//
unsigned XMLTableParser::_getRepeatCount( xmlNode* node ){
	
	unsigned value=1;
	xmlChar * attributeValue;
	attributeValue = xmlGetProp( node, (xmlChar*) _pXMLTagManager->getRepeatAttributeName().c_str() );
	
	if(attributeValue)
	{
		value = (int) strtol((char*)attributeValue,NULL,10);
	}
	free(attributeValue);
	// Returns either 1 or the repeat count:
	
	// DEBUG:
	// if(value>1) std::cout << "DEBUG>>>>> In _getRepeatCount() with count=" << value << "\n";
		
	return value;
	
}

//
// _getNodeContents
//
std::string XMLTableParser::_getNodeContents( xmlNode * node ){
	
	char *temp;
	temp = (char *) xmlNodeGetContent( node );
	if(!temp){
		std::cerr << "Node is empty! [1]" << std::endl;
		return std::string("");
	}
	
	std::string data(temp);
	free(temp);
	_trimWhiteSpace(data);
	if(data.empty()){
		std::cerr << "Node is empty! [2]" << std::endl;
	}	
	return data;
	
}

//
// storeColumnTitle
//
void XMLTableParser::_storeColumnTitle(const std::string & title){
	//
	// If the column title is something like "DiagnosisAge N"
	// Where the "N" indicates the type of the column ...
	//
	if(title[title.size()-2] == ' '){
		_columnType.push_back( title[title.size()-1] ); // Push the column type character
		_title.push_back( title.substr(0,title.size()-2) );
	}else{
		_columnType.push_back('\0'); // Leave the column unclassified: Let DataTable figure it out.
		_title.push_back(title);
	}
}

//
// _parse:
//
void XMLTableParser::_parse(xmlNode *node)
{
	
	xmlNode *currentNode = NULL; // Current node
	static unsigned int repeatCount = 1;
	static unsigned int currentCol = 0;
	
	//std::cout << "DEBUG>>> ENTERING _parse\n";
	
	//
	// Recursively descend through the XML node tree
	//
	for(currentNode = node; currentNode; currentNode = currentNode->next){
	
			//std::cout << "DEBUG>>>> NODE TYPE is " <<
			//             (currentNode->type==XML_TEXT_NODE ? "TEXT" :
			//             currentNode->type==XML_ELEMENT_NODE ? "ELEMENT" :
			//             "UNKNOWN" ) <<
			//             " and NAME=" << (char *)currentNode->name << std::endl;
		
		if( _tagNameMatches( currentNode->name , _pXMLTagManager->getTableTagName() ) ){
			
			////////////////////
			//
			// MATCHES TABLE TAG
			//
			////////////////////
			
			_tableCount++;
			
			//
			// Process sheet one; ignore all others
			//
			if(_tableCount > 1) return;
			
		}else if( _tagNameMatches( currentNode->name , _pXMLTagManager->getRowTagName() ) ){
			
			////////////////////
			//
			// MATCHES ROW TAG:
			//
			////////////////////
			
			//
			// If there are no titles, assume that the first non-empty row
			// contains titles:
			//
			if( _title.empty() ){
				
				if( _pXMLTagManager->cellHasChildXMLNode() && currentNode->children->children ){
					// OASIS style XML:
					_readTitles = true;
					//std::cout << "DEBUG>>> READING OASIS STYLE titles\n";
				}else if( !_pXMLTagManager->cellHasChildXMLNode() && currentNode->children ){
					// XHTML style XML:
					_readTitles = true;
					//std::cout << "DEBUG>>> READING XHTML STYLE titles\n";
				}else{
					_readTitles = false;
				}
			}else{
				
				//if(_readTitles) std::cout << "DEBUG>>> FINISHED READING titles\n";
				_readTitles = false;
			}
			
			currentCol = 0;
		
		}
		else if( ( _pXMLTagManager->cellHasChildXMLNode() 
		           &&  
		           _tagNameMatches( currentNode->name , _pXMLTagManager->getTextTagName() ) 
		         )
		         || 
		         ( !_pXMLTagManager->cellHasChildXMLNode() 
		           &&  
		           _tagNameMatches( currentNode->parent->name , _pXMLTagManager->getCellTagName() )
		         )
		){
			
			///////////////////////////
			//
			// MATCHES CELL with data:
			//
			///////////////////////////
			
			std::string data = _getNodeContents( currentNode );
			
			//
			// In OASIS, the repeat counter attribute is on "table-cell" which 
			// is the parent of the "p" text node:
			// 
			if( _pXMLTagManager->cellHasChildXMLNode() ) repeatCount = _getRepeatCount( currentNode->parent );
			else                                         repeatCount = _getRepeatCount( currentNode );
			
			for(unsigned int i=0; i< repeatCount; i++){
				
				if(_readTitles){
					// increment column counter:
					_columns++;
					// store the column title:
					_storeColumnTitle(data);
					//
					// DEBUG:
					//
					//std::cout << "DEBUG: COLUMN is " << data << "\n";
				}else{
					// Add the current cell contents to the
					// table if we are still within the column
					// boundaries defined by the column titles:
					currentCol++;
					if(currentCol <= _columns) _element.push_back( data );
				}
			}
			
			//std::cout << "DEBUG: NODE TYPE is " <<
			//             (currentNode->type==XML_TEXT_NODE ? "TEXT" :
			//             currentNode->type==XML_ELEMENT_NODE ? "ELEMENT" :
			//             "UNKNOWN" ) <<
			//             " and NAME=" << (char *)currentNode->name <<
			//             " and VALUE=" << data << std::endl;
			
		}
		else if( _pXMLTagManager->cellHasChildXMLNode() 
		         &&  
		         _tagNameMatches( currentNode->name , _pXMLTagManager->getCellTagName() )
		         &&
		         ! currentNode->children
		){
			
			///////////////////////////////////////////////////
			//
			// MATCHES AN OASIS-STYLE EMPTY CELL WITH NO DATA!
			//
			///////////////////////////////////////////////////
			
			std::cout << "DEBUG: NODE TYPE is " <<
			             (currentNode->type==XML_TEXT_NODE ? "TEXT" :
			             currentNode->type==XML_ELEMENT_NODE ? "ELEMENT" :
			             "UNKNOWN" ) <<
			             " and NAME=" << (char *)currentNode->name <<
			             " and PARENT NAME=" << (char *) currentNode->parent->name <<
			             " and THERE IS NO CHILD NODE WITH A VALUE" << std::endl;

			// Add the default missing value, "." to the
			// table if we are still within the column
			// boundaries defined by the column titles:
			currentCol++;
			if(currentCol <= _columns) _element.push_back( std::string(".") );

		}		
		//
		// Recurse:
		//
		_parse( currentNode->children );
		
	}
	
}

//
// _readDocument:
//
bool XMLTableParser::_readDocument(void){
	
	// parse the XML file
	_parse(_rootElement);
	
	// if the table is empty, exit
	if(_columns < 1)
	{
		xmlFreeDoc(_document);
		xmlCleanupParser();
		
		return false;
	}
	
	//
	// fill remaining empty slots with missing values to obtain a rectangular block of data
	//
	while(_element.size() % _columns != 0)
	{
		_element.push_back(".");
	}
	_rows = _element.size() / _columns;
	
	xmlFreeDoc(_document);
	xmlCleanupParser();
	
	// DEBUG:
	// display();
	
	return true;
	
}


//
// _trimWhiteSpace():
//
// This implementation is based on "A trim implementation for std::string"
// By Rodrigo C. Dias: http://www.codeproject.com/vcpp/stl/stdstringtrim.asp.
// 
// This implementation is faster than what we had originally.
//
void XMLTableParser::_trimWhiteSpace(std::string &str)
{
	
	std::string::size_type pos = str.find_last_not_of(' ');
	if(pos != std::string::npos){
		str.erase(pos + 1);
		pos = str.find_first_not_of(' ');
		if(pos != std::string::npos) str.erase(0, pos);
	}
	else str.erase(str.begin(), str.end());
	
}

///////////////////////////////////
//
// PUBLIC METHODS:
//
///////////////////////////////////


//
// readString:
//
bool XMLTableParser::readString(const std::string &inString, xmlNode *node ){
	
	reset();
	
	// read document from string
	_document = xmlReadMemory(inString.c_str(), inString.size(), NULL, NULL, 0);
	
	if (_document == NULL)
	{
		throw Exception("XMLTableParser::readString()","Could not parse %s XML string",inString.c_str());
	}
	
	// get start node
	if(node) _rootElement = node;
	else _rootElement = xmlDocGetRootElement(_document);
	
	return _readDocument();
	
}

//
// reset:
//
void XMLTableParser::reset()
{
	_element.clear();
	_columnType.clear();
	_title.clear();
	_tableCount = 0;
	_rows = 0;
	_columns = 0;
	_readTitles = false;
}

//
// DEBUG: display:
//
void XMLTableParser::display()
{
	for(unsigned int i=0;i<_element.size();i++)
	{
		if(i % _columns == 0)
		{
			std::cout << std::endl;
		}
		std::cout << _element[i] << "\t";
	}
}

