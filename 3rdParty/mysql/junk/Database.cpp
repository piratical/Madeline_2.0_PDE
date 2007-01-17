#include "Database.h"

void Database::readTable(const char* sqlQuery){
	
	try{
		//
		// Connect to the database
		//
		mysqlpp::Connection _con(mysqlpp::use_exceptions);
		_con.connect(_database,_host,_user,_password);
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
		//
		// Debugging:
		//std::cout << "Records Found: " << res.size() <<  std::endl;
		//std::cout << "Columns : " << res.num_fields() <<  std::endl;
		//
		_rows = res.size();
		_columns = res.num_fields();
		//
		// Store the field labels in the vector '_label'
		//
		for(int j=0;j<res.num_fields();j++){
			_label.push_back(res.names(j).c_str());
		}
		
		//
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
		std::cerr << "Error" << er.error << std::endl;
	}
	catch (mysqlpp::BadConversion& er) {
		std::cerr << "Error: Tried to convert \"" << er.data << "\" to a \""
			<< er.type_name << "\"." << std::endl;
	}
	
}

void Database::display(void){

	unsigned int j=0;
	std::cout << "Field Labels are:" << std::endl;
	while(j < _label.size()){
		std::cout << _label[j++] << std::endl;
	}
	j=0;
	std::cout << "The Vector is :" << std::endl;
	while(j < _element.size()){
		std::cout << _element[j++];
		std::cout << std::endl;
	}
}
