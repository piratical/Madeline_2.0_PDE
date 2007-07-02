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
// DelimitedTableParser.cpp
//

#include "DelimitedTableParser.h"

#include "FileTypeClassifier.h"
#include "BufferSizes.h"

#include <fstream>
#include <sstream>

///////////////////////////////////
//
// PUBLIC METHODS:
//
///////////////////////////////////

//
// readFile:
//
void DelimitedTableParser::readFile(const char* filename)
{
	std::string temp, total;
	char seeker;
	
	reset();
	
	std::ifstream readFile;
	
	readFile.open(filename);
	// Read In Column Headers
	while(!readFile.eof())
	{
		readFile >> temp;
		_title.push_back(temp);
		
		
		seeker = readFile.peek();
		if(seeker == ' ')
		{
			readFile >> seeker;
			_columnType.push_back(seeker);
		}else { 
			// Leave the column unclassified: Let DataTable figure it out:
			_columnType.push_back('\0'); 
		}
		
		
		_columns++;
		
		while(!readFile.eof())
		{
			seeker = readFile.peek();
			if(seeker != '\t' && seeker != ' ') break;
			readFile.get();
		}
		
		if(seeker == '\n') break;
	}
	
	//int i = 0;
	
	// Read in main body of information
	while(!readFile.eof())
	{
		total = "";
		seeker = '\t';
		
		do
		{
			if(seeker == ' ') total += ' ';
			readFile >> temp;
			total += temp;
			seeker = readFile.peek();
			
			if(readFile.eof()) break;
			
		}while(seeker != '\t' && seeker != '\n');
		
		if(readFile.eof()) break;
		_element.push_back(total);
	}
	
	_rows = _element.size() / _columns;
	
	readFile.close();
	
}

//
// readString:
//
void DelimitedTableParser::readString(const std::string &inString){
	
	std::string temp, total;
	char seeker/* = ' '*/;
	reset();
	
	std::istringstream str(inString,std::istringstream::in);
	
	// Read In Column Headers
	while(str.good())
	{
		str >> temp;
		_title.push_back(temp);
		seeker = str.peek();
		
		if(seeker == ' ')
		{
			str >> seeker;
			_columnType.push_back(seeker);
		}else { 
			// Leave the column unclassified: Let DataTable figure it out:
			_columnType.push_back('\0'); 
		}
		
		_columns++;
		
		while(!str.eof())
		{
			seeker = str.peek();
			if(seeker != '\t' && seeker != ' ') break;
			str.get();
		}
		if(seeker == '\n' || seeker == '\r') break;
	}
	
	// int i = 0;
	// Read in str body of information
	// This allows for tab delimited files that do not have a ' ' character for null values.
	char linestr[SMALL_STRING_BUFFER_SIZE];
	char columnstr[LARGE_STRING_BUFFER_SIZE];
	while(!str.eof()){
		if(str.eof()) break;
		// First obtain a line from the string stream 
		if(str.peek() == '\n' || str.peek() == '\r') str.ignore(1);
		str.getline(linestr,LARGE_STRING_BUFFER_SIZE);
		// Check for empty string:
		if(linestr[0] == '\0') continue;
		// Copy this line into a temporary stream
		std::istringstream strtemp(linestr,std::istringstream::in);
		unsigned count=0;
		// Split this line using '\t' as the delimiter
		while(!strtemp.eof()){
			if(strtemp.eof()) break;
			strtemp.getline(columnstr,1056,'\t');
			count++;
			// Ignore all extra columns in the line
			if(count > _columns) break;
			_element.push_back(columnstr);
		}
	}
	_rows = _element.size() / _columns;
	
}

//
// reset:
//
void DelimitedTableParser::reset()
{
	
	_element.clear();
	_columnType.clear();
	_rows = 0;
	_columns = 0;
	
}

//
// DEBUG: display:
// 
void DelimitedTableParser::display(void)
{
	
	unsigned int i;
	std::cout << "_columns is " << _columns << "\n";
	std::cout << "_rows    is " << _rows    << "\n";
	std::cout << "DelimitedTable has " << _title.size() << " titles.\n";
	for(i=0;i<_title.size();i++){
		std::cout << _title[i] << " ";
	}
	
	std::cout << "\n\nDelimitedTable has " << _columnType.size() << " _columnType declared types.\n";
	for(unsigned int i=0;i<_columnType.size();i++)
	{
		std::cout << _columnType[i] << "\t";
	}
	
	std::cout << "\n_element data follow:\n" << std::endl;
	
	for(unsigned int i=0;i<_element.size();i++)
	{
		if(i % _columns == 0)
		{
			std::cout << std::endl;
		}
		std::cout << _element[i] << "\t";
	}
}
