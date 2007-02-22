#include <iostream>
#include "DataTable.h"
#include "Database.h"
#include "XMLTableParser.h"

#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <fstream>
#include <sstream>

/*!
 *
 * The Parser class is used to read files into a DataTables. 
 * The parser can read in 3 types of file: Local files, Network files, and Mysql files.
 * 
 * When the Parser class reads in a file, it first attempts to uncompress it. 
 * Compression types that are supported are GZIP, PKZIP, and BZIP2.
 * Then, it attempts to parse the file. Parsable formats are: 
 * 
 *  1. Madeline flat file format 
 *  2. Delimited format
 *  3. OASIS XML
 *  4. XHTML XML
 *  5. Madeline XML
 *  5. Microsoft's Excel XML format
 *  
 * After parsing the file, table data in the file are put into a DataTables.
 * Multiple tables in one file are supported for some formats, such as OASIS and
 * XHTML XML files.
 * 
*/
class Parser
{
	
private:
	
	std::string _url;                           // URL for network files
	std::string _fileName;                      // fileName for disk file (also for network local /tmp file) 
	bool _isANetworkFile;                       // true if a network file
	
	std::vector<DataTable *> _tables;            // List of all data tables that have been read in
	
	std::vector<xmlNode*> _xmlTableTagPointers; // Pointers to the start of tables in XML files
	
	//
	// FileTypeClassifier _fileTypeClassifier;     // The FileTypeClassifier class opens files to determine their type.  
	// 
	FileTypeClassifier::FILE_TYPE _fileType;
	const char* _fileTypeName;
	
	//
	// 2006.08.22.ET addendum:
	// 
	XMLTagManager _xmlTagManager;
	
	
	#ifdef MYSQLPP
	Database _currentMysqlTable;
	#endif
	
	// Read in the supported XML tags for use in parsing
	void _readXMLFormats();

	// Read file based on type
	void _readXML(std::string data);
	void _readDelimited(std::string data);
	void _readMadeline(std::string data);
	
	// Used to determine format of flat files
	int _determineNumberOfBlocks(std::string inString);

	// Used to determine number of tables in XML file. Fills in the _pointers variable with table tags
	void _countTables(xmlNode *node);

	// Used to convert a file into a string
	std::string _stringify(std::string name);
	
public:
	
	Parser();
	~Parser();
	void display();                                     /// Prints out all datatables that have been read in
	
	void readFile(const std::string &filename);        /// Read in a file, unzip it, parse it, and put the contents into a datatable
	void readNetworkFile(const std::string &url);      /// Read in a file over the internet, unzip it, parse it, and put the contents into a datatable
	
	#ifdef MYSQLPP
	void readMysql(std::string tableName);                  /// Read in a Mysql table, and put the contents into a datatable
	void connectMysql(std::string host,std::string port, std::string user, std::string password, std::string database);  /// connect to a mysql table for reading/queries
	void queryMysql(const char* sqlQuery) {_currentMysqlTable.query(sqlQuery);} /// query a mysql table with a select, insert, delete, or update command
	#endif
	
	DataTable * getTable(unsigned i) { return _tables[i];}      /// returns a specific datatable
	

	const int getNumberOfTables( void ) const { return _tables.size(); };
	
	std::string getName(void) const { return _fileName; };
	std::string getURL(void)  const { return _url;      };
	bool        isANetworkFile(void) const { return _isANetworkFile; };
	
};

