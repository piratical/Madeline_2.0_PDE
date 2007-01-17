/////////////////////////////////////////////////////////
//
// This file is part of the MADELINE 2 program 
// written by Edward H. Trager, Ritu Khanna, and Adrian Marrs
// Copyright (c) 2006 by the
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
// 2006.08.16.
//

//
// Individual.h
//
#ifndef XMLTagManager_INCLUDED
#define XMLTagManager_INCLUDED

#include <iostream>
#include <vector>

///////////////////////////////
//
// XMLTagNames: Hold information about
// the names of the XML tags used by
// various XML formats.
// 
///////////////////////////////
class XMLTagNames
{
	
private:
	
	std::string _formatName;
	std::string _id;
	std::string _cellTag;
	std::string _tableTag;
	std::string _rowTag;
	std::string _columnTag;
	std::string _textTag;
	std::string _repeat;
	bool        _cellHasChildXMLNode;
	
public:
	
	//
	// Constructors:
	//
	XMLTagNames(){}
	XMLTagNames( const char *formatName,const char *id,const char *cellTag,const char *tableTag,const char *rowTag,const char *columnTag,const char *textTag,const char *repeat,bool cellHasChildXMLNode);
	
	//
	// Get methods:
	//
	std::string getFormatName() const;
	std::string getId() const;
	std::string getCellTagName() const;
	std::string getTableTagName() const;
	std::string getRowTagName() const;
	std::string getColumnTagName() const;
	std::string getTextTagName() const;
	std::string getRepeatAttributeName() const;
	bool        cellHasChildXMLNode() const;
};


//////////////////////////
//
// XMLTagManager: Manages details about 
//                the type and tag names of a 
//                given XML format
//                
//////////////////////////
class XMLTagLoader;
class XMLTagManager{
	
	friend class XMLTagLoader;
	
private:
	
	int _formatId;
	
	static std::vector< XMLTagNames > _tags;
	
	
public:
	
	//
	// Constructors:
	//
	XMLTagManager(){ _formatId= -1; };
	XMLTagManager(const std::string &data);
	
	bool discoverFormat(const std::string &data);
	
	std::string getFormatName() const;
	std::string getId() const;
	std::string getCellTagName() const;
	std::string getTableTagName() const;
	std::string getRowTagName() const;
	std::string getColumnTagName() const;
	std::string getTextTagName() const;
	std::string getRepeatAttributeName() const;
	bool        cellHasChildXMLNode() const;
	
};

////////////////////////////
//
// XMLTagLoader(): Loader class to load
// the static data of the XMLTagManager class.
//
////////////////////////////
class XMLTagLoader{
	
public:
	static XMLTagLoader xmlTagLoader;
	XMLTagLoader(){
		// madeline
		XMLTagManager::_tags.push_back(XMLTagNames("Madeline-XML","</madeline-xml>","d","table","row","col","","",false));
		// xhtml:
		XMLTagManager::_tags.push_back(XMLTagNames("W3C XHTML","</html>","td","table","tr","","","",false));
		// OASIS:
		XMLTagManager::_tags.push_back(XMLTagNames("OpenOffice.org OASIS","</office:document-content>","table-cell","table","table-row","table-col","p","number-columns-repeated",true));
		// Microsoft
		XMLTagManager::_tags.push_back(XMLTagNames("Microsoft XML","</Workbook>","Cell","Table","Row","Column","Data","number-columns-repeated",true));
		
	}
	
};

#endif
