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
// Database.h
//

#ifndef DATABASE_INCLUDED
#define DATABASE_INCLUDED

#ifdef MYSQLPP

#include <mysql++.h>

#include <iostream>
#include <iomanip>
#include <vector>
#include <cstring>

#include "TableParser.h"

class Database : public TableParser{
private:
	const char* _host;
	const char* _user;
	const char* _password;
	const char* _database;
	unsigned _port;
	bool _connected;
	mysqlpp::Connection _con;        // Mysql connection object
	
public:
	
	Database() {_connected = false;}
	
	//
	// Constructor:
	//
	Database(const char* host,const char* user,const char* password,const char* database,unsigned port=0)
	{
		_host = host;
		_user = user;
		_password = password;
		_database = database;
		_connected = true;
		_port = port;
		try
		{
			_con.connect(_database,_host,_user,_password,_port);
		}
		catch(...)
		{
			std::cout << "ERROR" << std::endl;
		}
	}
	
	void setup(const char* host,const char* user,const char* password,const char* database,unsigned port=0)
	{
		_host = host;
		_user = user;
		_password = password;
		_database = database;
		_port = port;
		_connected = true;
		try
		{
			_con.connect(_database,_host,_user,_password,_port);
		}
		catch(...)
		{
			std::cout << "ERROR" << std::endl;
		}
	}
	//
	// Public Functions:
	//
	void readTable(const char* sqlQuery); // Store the values retrieved into '_element' vector
	void query(const char* sqlQuery);    // Execute a query
	// Debugging Function(): display
	void display(void);                  // Display the vector
};

#endif
#endif
