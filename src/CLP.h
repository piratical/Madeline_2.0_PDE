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

#ifndef CLP_INCLUDED
#define CLP_INCLUDED

#include <iostream>
#include <map>
#include <set>
#include <vector>

class CLS{
private: 
	std::string _name;
	std::string _shortName;
	std::string _description;
	unsigned _numberOfSwitchArguments;
	std::vector<std::string> _switchArguments;
	bool _isSet;
	
public: 
	CLS(std::string name, std::string shortName, std::string description, unsigned numberOfSwitchArguments);
	void setSwitch(void) { _isSet = true; }
	void addSwitchArgument(std::string argument) { _switchArguments.push_back(argument); }
	void resetSwitch() { _isSet = false; }
	unsigned getNumberOfSwitchArguments(void) { return _numberOfSwitchArguments; }
	bool isSet(void){ return _isSet; }
	std::string getSwitchArgument(unsigned index){ return _switchArguments[index]; }
	const std::string getDescription(void){ return _description; }
	const std::string getShortName(void){ return _shortName; }
};

class CLP{
	
private:
	std::string _usage;
	std::map<std::string,std::string> _shortSwitchMapping;
	std::map<std::string, CLS> _switches;
	std::vector<std::string> _arguments;
	std::vector<std::string> _networkArguments;
	std::vector<std::string> _mysqlArguments;
	const static std::string _NETWORK_TYPE;
	const static std::string _NETWORK_TYPE_SECURE;
	const static std::string _MYSQL_TYPE;
	void _setSwitchArguments(std::string currentSwitch,int argc,char* argv[],int& currentIndex,CLS& cls);
	void _processMysqlArguments(std::string argument);
	std::string _getShortSwitchNameMapping(const std::string& shortName);
	bool _shortNameExists(const std::string& shortName);
	
public:
	void addUsage(const std::string usage);
	void addSwitch(std::string name,std::string shortName,std::string description,unsigned numberOfArguments=0);
	std::string getSwitchArgument(std::string name,unsigned index);
	bool parse(int argc,char* argv[]);
	bool hasSwitchSet(std::string name);
	const std::vector<std::string>& getArguments();
	const std::vector<std::string>& getNetworkArguments();
	const std::vector<std::string>& getMysqlArguments();
	bool hasMysqlArguments(){ if(_mysqlArguments.size()) return true; return false; }
	
	void printHelp();
	void printArguments();
	
};

#endif
