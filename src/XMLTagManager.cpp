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
// 2006.08.16
//

//
// XMLTagManager.cpp
//

#include "XMLTagManager.h"

////////////////////////
//
// CLASS XMLTagNames
//
////////////////////////

//
// XMLTagNames Constructor:
//
XMLTagNames::XMLTagNames( const char *formatName,const char *id,const char *cellTag,const char *tableTag,const char *rowTag,const char *columnTag,const char *textTag,const char *repeat,bool cellHasChildXMLNode){
	
	_formatName = formatName;
	_id         = id;
	_cellTag    = cellTag;
	_tableTag   = tableTag;
	_rowTag     = rowTag;
	_columnTag  = columnTag;
	_textTag    = textTag;
	_repeat     = repeat;
	_cellHasChildXMLNode=cellHasChildXMLNode;
	
}

std::string XMLTagNames::getFormatName() const { return _formatName; }
std::string XMLTagNames::getId() const { return _id; }
std::string XMLTagNames::getCellTagName() const { return _cellTag; }
std::string XMLTagNames::getTableTagName() const { return _tableTag; }
std::string XMLTagNames::getRowTagName() const { return _rowTag; }
std::string XMLTagNames::getColumnTagName() const { return _columnTag; }
std::string XMLTagNames::getTextTagName() const { return _textTag; }
std::string XMLTagNames::getRepeatAttributeName() const { return _repeat; }
bool        XMLTagNames::cellHasChildXMLNode() const { return _cellHasChildXMLNode; }


////////////////////////
//
// CLASS XMLTagManager
//
////////////////////////

//
// XMLTagManager(data) constructor:
//
XMLTagManager::XMLTagManager(const std::string &data){
	//
	// Just call discoverFormat method:
	//
	discoverFormat(data);
	
}

//
//
// discoverFormat(): Returns true if XMLTagManager recognizes the XML
//                   format.  Else returns false.
//
bool XMLTagManager::discoverFormat(const std::string &data) {
	
	_formatId = -1;
	
	for(unsigned int i=0;i<_tags.size();i++){
		
		std::string::size_type found = data.find( _tags[i].getId(), 0);
		if(found != std::string::npos){
			_formatId = i;
			return true;
		}
	}
	
	return false;
	
}

    
//
// getFormatName(): Human name for the XML format
//
std::string XMLTagManager::getFormatName() const {
	
	if( _formatId < 0 ) return (std::string) "unknown";
	return  _tags[ _formatId ].getFormatName(); 
	
}

//
// getId(): Returns the "id" used to classify an XML format.
//          Normally this public method won't be used.
//
std::string XMLTagManager::getId() const {
	
	if( _formatId < 0 ) return (std::string) ".";
	return  _tags[ _formatId ].getId(); 
	
}

//
// getCellTag(): Returns the XML "cell" tag for the current format:
//
std::string XMLTagManager::getCellTagName() const {
	
	if( _formatId < 0 ) return (std::string) ".";
	return  _tags[ _formatId ].getCellTagName(); 
	
}

//
// getTableTag(): Returns the XML "table" tag for the current format:
//
std::string XMLTagManager::getTableTagName() const {
	
	if( _formatId < 0 ) return (std::string) ".";
	return  _tags[ _formatId ].getTableTagName(); 
	
}

//
// getRowTag(): Returns the XML "row" tag for the current format:
//
std::string XMLTagManager::getRowTagName() const {
	
	if( _formatId < 0 ) return (std::string) ".";
	return  _tags[ _formatId ].getRowTagName(); 
	
}

//
// getColumnTag(): Returns the XML "column" tag for the current format:
//                 Note that most XML table formats are row-first and
//                 don't use a column tag.
//
std::string XMLTagManager::getColumnTagName() const {
	
	if( _formatId < 0 ) return (std::string) ".";
	return  _tags[ _formatId ].getColumnTagName(); 
	
}

//
// getTextTag(): Returns the XML "text" tag for the current format.
//               Note that some XML table formats don't use a text tag
//               but rather have data directly inside the cell tags.
//
std::string XMLTagManager::getTextTagName() const {
	
	if( _formatId < 0 ) return (std::string) ".";
	return  _tags[ _formatId ].getTextTagName(); 
	
}

//
// getRepeat(): Returns the XML "repeat" attribute format for the current
//              XML format.  Some formats don't use this.
//
std::string XMLTagManager::getRepeatAttributeName() const {
	
	if( _formatId < 0 ) return (std::string) ".";
	return  _tags[ _formatId ].getRepeatAttributeName(); 
	
}

//
// cellHasChildXMLNode(): Returns true if the XML format is one that uses
//                        a child XML node to store the cell data instead of
//                        storing the data directly in the cell node.
//
bool XMLTagManager::cellHasChildXMLNode() const {
	
	if( _formatId < 0 ) return false;
	return  _tags[ _formatId ].cellHasChildXMLNode(); 
	
}

///////////////////////////////////
//
// STATIC AND CONST STATIC MEMBERS:
//
///////////////////////////////////

std::vector< XMLTagNames > XMLTagManager::_tags;
XMLTagLoader XMLTagLoader::xmlTagLoader;


