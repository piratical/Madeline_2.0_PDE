//  #################
//  #
//  # testParser.cpp
//  #
//  #################


#include "Parser.h"
#include "DataTable.h"
#include "DataColumn.h"

int main(int argc, char *argv[])
{
	
	DataColumn nc("NewC",STRING,3);
	nc.setRow(0,"One");
	nc.setRow(1,"Two");
	nc.setRow(2,"Three");
	nc.printData();
	Parser newpar;
	// Tab Delimited Typed
	newpar.readFile("testfamily.txt");
	// Display all Tables
	newpar.display();
	// Get the DataTable
 	DataTable newtable = newpar.getTable(0); 
	// Get the number of tables:
	std::cout << "Number of Tables " << newpar.getNumberOfTables() << std::endl;
	// Get the number of Columns
	std::cout << "Columns: " << newtable.getNumberOfColumns() << std::endl;
	// Get the type of Table:
	std::cout << "Type of Table: " << newtable.getTableType() << std::endl;
	std::cout << "Column 4 is "  <<  newtable.getColumnName(4) << std::endl;
	std::cout << "Column 0 is " << newtable.getColumnName(0) << std::endl;
	std::cout << "Contents of Column Father" << std::endl;
	DataColumn* newcolumn  = newtable.getColumn("Father"); 
	std::cout << newcolumn->getColumnType() << std::endl;	 
	std::cout << newcolumn->getNumberOfRows() << std::endl;
	std::cout << newcolumn->getOrdinal() << std::endl;
	newcolumn->printData();
	std::cout << "Ordinal of Column DOB is " << newtable.getColumnOrdinal("DOB") << std::endl;
	std::cout << "Contents of Column no 3 " << std::endl;
	DataColumn* dobcolumn = newtable.getColumn(3);
	std::cout << dobcolumn->getColumnType() << std::endl;
	dobcolumn->printData();
	
	return 1;
}


