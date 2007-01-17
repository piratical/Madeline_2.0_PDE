//  #################
//  #
//  # testParser.cpp
//  #
//  #################


#include "Parser.h"
#include "DataTable.h"
#include "PedigreeSet.h"
#include "LabelSet.h"

int main(int argc, char *argv[])
{
	
	Parser newpar;
	// Tab Delimited Typed
	newpar.readFile("../data/c2.txt");
//	newpar.readFile("../data/simplecase2.txt");
//	newpar.readFile("fg1.txt");
//	newpar.readFile("../data/testfamily.txt");
//	newpar.readFile("fourdt.txt");
//	newpar.readFile("testMulSpouseFamily.txt");
	// Display all Tables
	newpar.display();
	// Get the DataTable
 	DataTable newtable = newpar.getTable(0); 
	// Get the type of Table:
	//std::cout << "Type of Table: " << newtable.getTableType() << std::endl;
	std::vector<std::string> showColumns;
	showColumns.push_back("Father");
	showColumns.push_back("Mother");
	showColumns.push_back("Gender");
	//newtable.toggleColumnsForPedigree(showColumns);
	LabelSet labelset(newtable);
	std::cout << " No of labels " << labelset.getNumberOfLabels() << std::endl;
	PedigreeSet newpedset;
	newpedset.addPedigreesFromDataTable(newtable,0);

	newpedset.draw(labelset);
	//newpedset.display();
	//std::cout << "Initial iconInterval" << DrawingMetrics::getIconInterval() << std::endl;
	//DrawingMetrics::setIconInterval(8);
	//std::cout << DrawingMetrics::getIconInterval();
	return 1;
}


