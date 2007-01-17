#ifndef DATABASE_INCLUDED
#define DATABASE_INCLUDED

#include <mysql++.h>

#include <iostream>
#include <iomanip>
#include <vector>

class Database{
private:
	unsigned int _rows;                         // Number of data rows
	unsigned int _columns;                      // Number of data columns
	const char* _host;
	const char* _user;
	const char* _password;
	const char* _database;
	std::vector<std::string> _element; // Vector containing data values 
	std::vector<std::string> _label;   // Vector containing the field labels
public:
	//
	// Constructor:
	//
	Database(const char* host,const char* user,const char* password,const char* database){
		_host = host;
		_user = user;
		_password = password;
		_database = database;
	}
	//
	// Getters:
	//
	std::vector<std::string> getFieldLabels() const { return _label; }
	std::vector<std::string> getTableValues() const { return _element; }
	unsigned int getRowCount() const { return _rows; }
	unsigned int getColumnCount() const { return _columns; }
	//
	// Public Functions:
	//
	void readTable(const char* sqlQuery);// Store the values retrieved into '_element' vector
	// Debugging Function(): display
	void display(void);                  // Display the vector
};

#endif
