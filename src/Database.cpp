/////////////////////////////////////////////////////////
//
// This file is part of the MADELINE 2 program 
// written by Edward H. Trager,Ritu Khanna and Adrian Marrs
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
// Database.cpp
//
#ifdef MYSQLPP

#include "Database.h"

///////////////////////////////////
//
// PUBLIC METHODS:
//
///////////////////////////////////

///
/// readTable: Executes a mysql select query to read a data table from the database and stores the result in a vector.
///
void Database::readTable(const char* sqlQuery){
	
	if(!_connected) return;
	try{
		//
		// Create the query object
		//
		mysqlpp::Query query = _con.query();
		query.reset();
		query << sqlQuery;
		//
		// Store the query results
		//
		mysqlpp::Result res = query.store();
		//    std::cout << "SIZE: " << res.size() << std::endl;
		//
		// Debugging:
		//std::cout << "Records Found: " << res.size() <<  std::endl;
		//std::cout << "Columns : " << res.num_fields() <<  std::endl;
		//
		_rows = res.size();
		_columns = res.num_fields();
		//
		// Store the field labels in the vector '_title'
		//
		for(int j=0;j<res.num_fields();j++){
			_title.push_back(res.names(j).c_str());
			_columnType.push_back('\0'); // Leave the column unclassified: Let DataTable figure it out.
		}
		//
		// Iterate through the query results and store them in a vector
		//
		mysqlpp::Row row;
		mysqlpp::Result::iterator i;
		for(i=res.begin();i != res.end(); ++i){
			row = *i;
			for(int j=0;j<res.num_fields();j++){
				_element.push_back(row[j].c_str());
			}
		}
	}
	catch(mysqlpp::BadQuery& er){
		std::cerr << "Error: " << er.what() << std::endl;
	}
	catch (mysqlpp::BadConversion& er) {
		std::cerr << "Error: Tried to convert \"" << er.data << "\" to a \""
			<< er.type_name << "\"." << std::endl;
	}
  catch (...) {
		std::cerr << "Error " << std::endl;
	}
	
}

///
/// query: Executes any database query and displays the results. 
///
void Database::query(const char* sql){
	
	if(!_connected) return;
	try{
		mysqlpp::Query query = _con.query();
		//    if(!(_con.connected())) return;
		//    std::cout << "PAST IT :" << _con.connected() << ":" << std::endl;
		if(strncasecmp(sql,"select",6)==0 || strncasecmp(sql,"desc",4) == 0 || strncasecmp(sql,"show",4) == 0){
			query.reset();
			query << sql;
			//std::cout << "Query: " << query.preview() << std::endl;
			mysqlpp::Result res = query.store();
			//std::cout << "Records Found: " << res.size() <<  std::endl;
			// std::cout << "Columns : " << res.num_fields() <<  std::endl;
			//
			// Iterate through the query results:
			//
			mysqlpp::Row row;
			mysqlpp::Result::iterator i;
			// Display the column labels for a select query:
			if(strncasecmp(sql,"select",6)==0){
				for(int j=0;j<res.num_fields();j++){
					std::cout << res.names(j).c_str() << "\t";
				}
				std::cout << std::endl;
			}
			for (i = res.begin(); i != res.end(); ++i) {
				row = *i;
				for (int counter = 0; counter < res.num_fields(); counter++) {
					std::cout << row[counter] << "\t";
				}
				std::cout << std::endl;
			}
		}else if(strncasecmp(sql,"delete",6) == 0 || strncasecmp(sql,"update",6) == 0 || strncasecmp(sql,"insert",6) == 0){
			query.reset();
			query.exec(sql);
		}else if(strncasecmp(sql,"use",3) == 0){
      
			const char* temp = sql+4;
			_con.select_db(temp);
			
		}else{
			std::cout << "Warning: Query not supported" << std::endl;
		}
	}
	catch(mysqlpp::BadQuery& er){
		std::cerr << "Error:" << er.what() << std::endl;
	}
	catch (mysqlpp::BadConversion& er) {
		std::cerr << "Error: Tried to convert \"" << er.data << "\" to a \""
			<< er.type_name << "\"." << std::endl;
	}
	
}

//
// DEBUG: display:
//
void Database::display(void){
	
	if(!_connected) return;
	unsigned int j=0;
	std::cout << "Field Labels are:" << std::endl;
	while(j < _title.size()){
		std::cout << _title[j++] << std::endl;
	}
	j=0;
	std::cout << "The Vector is :" << std::endl;
	while(j < _element.size()){
		std::cout << _element[j++];
		std::cout << std::endl;
	}
}

#endif
