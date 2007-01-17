/////////////////////////////////////////////////////////
///
/// This file is part of the MADELINE 2 program 
/// written by Edward H. Trager, Ritu Khanna and Adrian Marrs
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

//
// XMLTableParser.h
//

#ifndef XMLTABLEPARSER_INCLUDED
#define XMLTABLEPARSER_INCLUDED

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/debugXML.h>
#include <fstream>
#include <iostream>
//#include <vector>
//#include <string>
#include "Exception.h"
#include "ColumnClassifier.h"
#include "../3rdParty/unzip/unzip.h"
#include "XMLTagManager.h"

#include "TableParser.h"

/*!
 * This class is used to parse XML files and strings. The XML formats supported are determined by files stored in the /dtds/ folder
*/
    
class XMLTableParser : public TableParser {
	
	private:
		
		const XMLTagManager* _pXMLTagManager;
		
		//unsigned long _rows;                     // Number of rows in the table
		//unsigned _columns;                       // Number of columns in the table
		//std::vector<std::string> _element;       // Table cell values
		//std::vector<std::string> _title; 
		//std::vector<char> _columnType;
		
		
		xmlDoc *_document;                       // XML document
		xmlNode *_rootElement;                   // XML document root Element
		//
		// Constants
		//
		const static int WRITEBUFFERSIZE = 8192; // Buffer size to extract the XML file
		int _tableCount;
		bool _readTitles;
		
		std::string CELL, TABLE, ROW, COL, TEXT, REPEAT;
		//
		// Functions
		//
		bool _readDocument(void);
		void _parse(xmlNode *node ); //Parse the XML file
		
		void _trimWhiteSpace(std::string &str);
		// Wrapper to check for string equality:
		bool _tagNameMatches(const xmlChar *xmlTagName,std::string referenceTagName);
		unsigned _getRepeatCount( xmlNode* node );
		std::string _getNodeContents( xmlNode* node);
		void _storeColumnTitle(const std::string & title);
		
	public:
		//
		// Constructors :
		//
		XMLTableParser(const XMLTagManager *p);
		
		//
		// Read in the XML string starting with the given node. 
		// If node == NULL, start from the rootnode.
		// xmlTagManagerVectorIndex in effect tells us which type of XML
		// file to read.
		// 
		bool readString(const std::string &inString, xmlNode *node);
		
		void display(void);
		
		// reset all values for a clean read
		void reset();
		
		//bool isXMLFormat(const std::string &inString);
		
		//unsigned getColumnCount( void ){ return _columns; }
		//unsigned long getNumberOfRows( void ){ return _rows; }
		
		//std::vector<std::string> getContents() {return _element;}
		//std::vector<std::string> getTitles() {return _title;}
		//std::vector<char> getTypes() {return _columnType;}
		
};
#endif
