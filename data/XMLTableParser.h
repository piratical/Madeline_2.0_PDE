/////////////////////////////////////////////////////////
///
/// This file is part of the MADELINE 2 program 
/// written by Edward H. Trager and Ritu Khanna
/// Copyright (c) 2005 by the
/// Regents of the University of Michigan.
/// All Rights Reserved.
/// 
/// The latest version of this program is available from:
/// 
///   http://eyegene.ophthy.med.umich.edu/madeline/
///   
/// Released under the GNU General Public License.
/// A copy of the GPL is included in the distribution
/// package of this software, or see:
/// 
///   http://www.gnu.org/copyleft/
///   
/// ... for licensing details.
/// 
/////////////////////////////////////////////////////////

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/debugXML.h>
#include <fstream>
#include <iostream>
#include <vector>
#include "../3rdParty/src/unzip/unzip.h"
#include "Exception.h"

class XMLTableParser{
	
	private:
		int _rows;                              // Number of rows in the table
		int _columns;                           // Number of columns in the table
		const char* _fileName;                  // Name of the file XML file to be extracted and/or parsed
		const char* _fileType;                  // Type of the file - plain XML or zipped
		std::vector<std::string> _element;      // Table cell values
		std::vector<int> _columnWidth;          // Maximum width of each table column
		
		xmlDoc *_document;                      // XML document
		xmlNode *_rootElement;                  // XML document root Element
		//
		// Constants
		//
		const static int CASESENSITIVITY = 1;   // Casesensitivity of XML filename
		const static int WRITEBUFFERSIZE = 8192;// Buffer size to extract the XML file
		static const char* XMLFILENAME;         // Name of XML file to extract
		//
		// Functions
		//
		void _parseFile(xmlNode *node);         // Parse the XML file
		void _parseMSFile(xmlNode *node);       // Parse MS Excel XML file
		void _extractFile();                    // Extract the XML file to be parsed from the .zip file
	public:
		//
		// Constructors :
		//
		XMLTableParser(){}
		
		XMLTableParser(const char* fileName,const char* fileType){
			_fileName = fileName;
			_fileType = fileType;
		}
		
		void setFileName(const char* fileName){
			_fileName = fileName;
		}
		
		void setFileType(const char* fileType){
			_fileType = fileType;
		}
		
		void readFile(){
			if(_fileType == "PKZIP"){
				_extractFile();
				_document = xmlReadFile(XMLFILENAME, NULL, 0);
			}else{
				_document = xmlReadFile(_fileName, NULL, 0);
			}
			if (_document == NULL) {
				throw Exception("XMLTableParser::readFile()","Could not parse %s XML file",_fileName);  
			}
			_rootElement = xmlDocGetRootElement(_document);
			if(_fileType == "XML_OO" || _fileType == "PKZIP"){
				_parseFile(_rootElement);
			}else
			if(_fileType == "XML_MS"){
				_parseMSFile(_rootElement);
			}
			while(_element.size() % _columns != 0){
				_element.push_back(".");
			}
			_rows = _element.size() / _columns;        
			xmlFreeDoc(_document);
			xmlCleanupParser();
		}
		
		void display(void){
			for(unsigned int i=0;i<_element.size();i++){
				if(i % _columns == 0){
					std::cout << std::endl;
				}
				std::cout << _element[i] << "\t";
			}
		}
};

const char* XMLTableParser::XMLFILENAME="content.xml";
///
/// Parse the XML file and retrieve all the data into a vector
/// @param node is the root node of the XML document
///
void XMLTableParser::_parseFile(xmlNode *node){
	
	xmlNode *currentNode = NULL;   // Current node
	static int sheetCount = 0;     // Number of sheets in the file
	const char *ATTRIBUTE = "number-columns-repeated"; // Attribute that indicates the number of contiguous blank cells
	for(currentNode = node; currentNode; currentNode = currentNode->next) {
		if(currentNode->type == XML_ELEMENT_NODE) {
			// 
			// Contiguous blank cells in the table are represented as an attribute of table-cell element.
			// If the table-cell element has no children and no attributes, it represents a blank cell.
			// 
			if(!strcmp((char *)currentNode->name,"table-cell")){
				if(xmlLsCountNode(currentNode) == 0){ 
					if(currentNode->properties){
						xmlChar *attributeValue = xmlGetProp(currentNode,(xmlChar*)ATTRIBUTE);
						int value = (int) strtol((char*)attributeValue,NULL,10);
						//
						// If the number of blank cells of a row equals the number of
						// table columns then that row is blank; ignore it
						//
						if(value == _columns){
							continue;
						}else{
						//
						// If the table cell content is a blank value 
						// insert a '.' into the vector
						//
							for(int i=0;i<value;i++){
								_element.push_back(".");
							}
						}
					}else{
						//
						// Blank cell; insert a '.' into the vector
						//
						_element.push_back(".");
					}
				}
			}else
			if(!strcmp((char *)currentNode->name,"table")){
				sheetCount++;
				//
				// Process sheet one; ignore all others
				//
				if(sheetCount > 1){
					return;
				}
			}else
			if(!strcmp((char *)currentNode->name,"table-column")){
				//
				// Attribute of element table-column gives the number of columns in the table
				//
				xmlChar *attributeValue = xmlGetProp(currentNode,(xmlChar*)ATTRIBUTE);
				_columns = (int) strtol((char*)attributeValue,NULL,10);
			}
		}else
		if(currentNode->type == XML_TEXT_NODE){
			//
			// Store the non-blank value of the cell into the vector
			//
			if(!strcmp((char *)currentNode->parent->name,"p")){
				_element.push_back((std::string)(char *)xmlNodeGetContent(currentNode));
			}
		}
		_parseFile(currentNode->children);
	}
}


void XMLTableParser::_parseMSFile(xmlNode *node){
	
	xmlNode *currentNode = NULL;   // Current node
	static int sheetCount = 0;     // Number of sheets in the file
	const char *INDEX_ATTRIBUTE = "Index"; // Attribute that indicates the number of contiguous blank cells
	const char *COLUMN_ATTRIBUTE = "ExpandedColumnCount";
	for(currentNode = node; currentNode; currentNode = currentNode->next) {
		if(currentNode->type == XML_ELEMENT_NODE) {
			// 
			// Contiguous blank cells in the table are represented as an attribute of table-cell element.
			// If the table-cell element has no children and no attributes, it represents a blank cell.
			// 
			if(!strcmp((char *)currentNode->name,"Cell")){
				if(currentNode->properties){
					if(xmlHasProp(currentNode,(xmlChar*)INDEX_ATTRIBUTE)){
						xmlChar *attributeValue = xmlGetProp(currentNode,(xmlChar*)INDEX_ATTRIBUTE);
						int value = (int) strtol((char*)attributeValue,NULL,10);
						//
						// If the table cell content is a blank value 
						// insert a '.' into the vector
						//
						int blankIndex = _element.size() % _columns;
						for(int i=blankIndex+1;i<value;i++){
							_element.push_back(".");
						}
					}
				}
			}else
			if(!strcmp((char *)currentNode->name,"Worksheet")){
				sheetCount++;
				//
				// Process sheet one; ignore all others
				//
				if(sheetCount > 1){
					return;
				}
			}else
			if(!strcmp((char *)currentNode->name,"Table")){
				//
				// Attribute of element table gives the number of columns in the table
				//
				xmlChar *attributeValue = xmlGetProp(currentNode,(xmlChar*)COLUMN_ATTRIBUTE);
				_columns = (int) strtol((char*)attributeValue,NULL,10);
			}
		}else
		if(currentNode->type == XML_TEXT_NODE){
			//
			// Store the non-blank value of the cell into the vector
			//
			if(!strcmp((char *)currentNode->parent->name,"Data")){
				_element.push_back((std::string)(char *)xmlNodeGetContent(currentNode));
			}
		}
		_parseMSFile(currentNode->children);
	}
}




///
/// Extract the xml file from a zip archive
///
void XMLTableParser::_extractFile(){
	
	int err=UNZ_OK;
	void* buf;
	unzFile unzipFile=NULL;
	
	//
	// Open the Zip file
	//
	if(_fileName != NULL){
		unzipFile = unzOpen(_fileName);
		if(unzipFile == NULL){
			throw Exception("XMLTableParser::_extractFile()","Cannot open zip file");
		}
	}
	std::cout << "Opened " << _fileName << std::endl;
	//
	// Locate the XML file to extract in the zip file
	// 
	if(unzLocateFile(unzipFile,XMLFILENAME,CASESENSITIVITY) != UNZ_OK){
		throw Exception("Method","Cannot find the requested zipfile");
		//throw Exception("XMLTableParser::_extractFile()","%s not found in the zip archive %s",XMLFILENAME,_fileName);
	}
	
	
	buf = (void*)malloc(WRITEBUFFERSIZE);
	if (buf == NULL){
		throw Exception("XMLTableParser::_extractFile()","Error allocating memory for the XML file buffer ");
	}
	
	err = unzOpenCurrentFile(unzipFile);
	if (err != UNZ_OK){
		throw Exception("XMLTableParser::_extractFile()","Error opening zip file");
	}
	//
	// Extract the XML file
	//
	std:: cout << "Extracting: " << XMLFILENAME << std::endl;
	std::ofstream myFile(XMLFILENAME,std::ios::out|std::ios::binary);
	do{
		err = unzReadCurrentFile(unzipFile,buf,WRITEBUFFERSIZE);
		if (err < 0){
			err = unzCloseCurrentFile(unzipFile);
			free(buf);
			unzCloseCurrentFile(unzipFile);
			throw Exception("XMLTableParser::_extractFile()","Error reading zip file");
		}
		if (err > 0){
			myFile.write((char *)buf,err);
		}
	}while (err>0);
	//
	// Close the opened files
	//
	
	if (err == UNZ_OK){
		err = unzCloseCurrentFile (unzipFile);
		if (err!=UNZ_OK){
			free(buf);
			unzCloseCurrentFile(unzipFile);
			throw Exception("XMLTableParser::_extractFile()","Error closing zip file");
		}
	}
	else{
		unzCloseCurrentFile(unzipFile); 
	}
	free(buf);
	unzCloseCurrentFile(unzipFile);    
	return;
	
}
