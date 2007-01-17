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
// 2006.12.04.ET
//

//
// TableParser.h
//

#ifndef TableParser_INCLUDED
#define TableParser_INCLUDED

#include <string>
#include <vector>

class TableParser{
	
private:
	
protected:
	
	unsigned      _columns;
	unsigned long _rows;
	
	std::vector<std::string> _element;
	std::vector<char>        _columnType;
	std::vector<std::string> _title;
public:
	
	unsigned                              getColumnCount ( void ) const { return _columns;    };
	unsigned long                         getNumberOfRows( void ) const { return _rows;       };
	const std::vector<std::string> *const getContents    ( void ) const { return &_element;    };
	const std::vector<char>        *const getTypes       ( void ) const { return &_columnType; };
	const std::vector<std::string> *const getTitles      ( void ) const { return &_title;      };
	
};

#endif
