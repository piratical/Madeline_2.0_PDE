/////////////////////////////////////////////////////////
//
// This file is part of the MADELINE 2 program 
// written by Edward H. Trager, Ritu Khanna and Adrian Marrs
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
// CLP.cpp
// 

#include "CLP.h"
#include "Warning.h"
#include <iomanip>

///////////////////////////////////
//
// CONST STATIC MEMBERS:
//
///////////////////////////////////

const std::string CLP::_NETWORK_TYPE = "http://";
const std::string CLP::_NETWORK_TYPE_SECURE = "https://";
const std::string CLP::_MYSQL_TYPE = "mysql://";


///////////////////////////////////
//
// PRIVATE METHODS:
//
///////////////////////////////////

//
// _getShortSwitchNameMapping: returns the long  name corresponding to the short switch name
//
std::string CLP::_getShortSwitchNameMapping(const std::string& shortName){
	
	std::string switchName=".";
	std::map<std::string,std::string>::iterator it = _shortSwitchMapping.find(shortName);
	if(it != _shortSwitchMapping.end()) switchName= it->second;
	return switchName;
	
}

//
// _shortNameExists:
//
bool CLP::_shortNameExists(const std::string& shortName){
	
	std::map<std::string,std::string>::iterator it = _shortSwitchMapping.find(shortName);
	if(it != _shortSwitchMapping.end()) return true;
	return false;
	
}

///
/// _setSwitchArguments: Some of the switches require arguments. This method saves the arguments passed to a switch in a map.
///
void CLP::_setSwitchArguments(std::string currentSwitch,int argc,char* argv[],int& currentIndex, CLS& cls){
	
	std::vector<std::string> tempArgs;
	unsigned switchArgumentSize =cls.getNumberOfSwitchArguments();
	for(unsigned k=0;k<switchArgumentSize;k++){
		if((currentIndex+k+1) < argc && argv[currentIndex+k+1][0] != '-'){
			std::string temp = argv[currentIndex+k+1];
			tempArgs.push_back(temp);
		}else{
			
			Warning("CLP::_setSwitchArguments()",
			        "Insufficient arguments provided for switch %1$s. The switch will be ignored",
			        currentSwitch.c_str()
			);
			currentIndex += k;
			cls.resetSwitch();
			return;
		}
	}
	if(tempArgs.size() == switchArgumentSize){
		for(unsigned cnt=0;cnt<tempArgs.size();cnt++) cls.addSwitchArgument(tempArgs[cnt]);
		currentIndex += switchArgumentSize;
		cls.setSwitch();
	}
	
}

//
// _processMysqlArguments:
//
void CLP::_processMysqlArguments(std::string argument){
	
	// for warnings:
	const char *methodName="CLP::_processMysqlArguments()";
	
	std::string::size_type loc;
	std::string::size_type subloc;
	
	loc = argument.find("/");
	if(loc != std::string::npos){
		// User specified the host
		std::string hostString = argument.substr(0,loc-1);
		// Check if the port is specified too
		subloc = hostString.find(":");
		if(subloc != std::string::npos){
			// Both host and port specified:
			_mysqlArguments.push_back(hostString.substr(0,subloc-1));
			_mysqlArguments.push_back(hostString.substr(subloc+1));
		}else{
			// Only the host specified
			_mysqlArguments.push_back(hostString);
			_mysqlArguments.push_back("");
		}
		argument = argument.erase(0,loc+1);
	}else{
		// Default values for host and port
		_mysqlArguments.push_back("localhost");
		_mysqlArguments.push_back("");
	}
	
	loc = argument.find("@");
	if(loc != std::string::npos){
		// Split the string 
		std::string userString = argument.substr(0,loc);
		subloc = userString.find(":");
		if(subloc != std::string::npos){
			_mysqlArguments.push_back(userString.substr(0,subloc));
			_mysqlArguments.push_back(userString.substr(subloc+1));
		}else{
			Warning(methodName,"Invalid Mysql Argument. 'username:passwd' is missing.\n Specify the argument in the following format 'mysql://[hostname:port/]username:passwd@database:table'");
			_mysqlArguments.clear();
			return;
		}
		std::string databaseString = argument.substr(loc+1);
		subloc = databaseString.find(":");
		if(subloc != std::string::npos){
			_mysqlArguments.push_back(databaseString.substr(0,subloc));
			_mysqlArguments.push_back(databaseString.substr(subloc+1));
		}else{
			Warning(methodName,"Invalid Mysql Argument. 'database:table' is missing.\n Specify the argument in the following format 'mysql://[hostname:port/]username:passwd@database:table'");
			_mysqlArguments.clear();
			return;
		}
		
	}else{
		Warning(methodName,"Invalid Mysql Argument.\n Specify the argument in the following format 'mysql://[hostname:port/]username:passwd@database:table'");
		_mysqlArguments.clear();
		return;
	}
	// Debug:
	//for(unsigned i=0;i<_mysqlArguments.size();i++) std::cout << _mysqlArguments[i] << std::endl;
	
}


///////////////////////////////////
//
// PUBLIC METHODS:
//
///////////////////////////////////

//
// printHelp:
//
void CLP::printHelp(){
	
	std::cout << "Usage: " << _usage << std::endl;
	std::map<std::string,CLS>::iterator mit = _switches.begin();
	while(mit != _switches.end()){
		std::cout << std::endl << std::setw(16) << mit->first << " " << mit->second.getDescription();
		std::cout << std::endl << std::setw(16) << mit->second.getShortName();
		mit++;
	}
	std::cout << std::endl;
	
}

//
// addUsage:
//
void CLP::addUsage(const std::string usage){
	
	_usage = usage;
	
}

//
// addSwitch:
//
void CLP::addSwitch(std::string name,std::string shortName,std::string description,unsigned numberOfArguments){
	
	if(_shortNameExists(shortName)){
		Warning("CLP::addSwitch()","The shortName %s for %s is being used by another switch. Specify an alternate short name.",shortName.c_str(),name.c_str());
		return;
	}
	std::pair<std::map<std::string,CLS>::iterator,bool> p;
	p = _switches.insert(std::pair<std::string,CLS>(name,CLS(name,shortName,description,numberOfArguments)));
	if(!p.second){
		Warning("CLP::addSwitch()",
		        "Duplicate Switch %1$s not allowed.",
		        name.c_str()
		);
	}else{
		_shortSwitchMapping.insert(std::pair<std::string,std::string>(shortName,name));
	}
}

//
// hasSwitchSet:
//
bool CLP::hasSwitchSet(std::string name){
	
	std::string switchName;
	if(name[1] != '-'){
		switchName = _getShortSwitchNameMapping(name);
		if(switchName==".") return false;
	}else switchName=name;
	std::map<std::string,CLS>::iterator mit = _switches.find(switchName);
	if(mit != _switches.end()) return mit->second.isSet();
	return false;
	
}

///
/// parse: Parses the command line arguments. Returns false if there was an error encountered. 
///
bool CLP::parse(int argc,char* argv[]){
	
	if(argc < 2){
		printHelp();
		return false;
	}
	
	const char *methodName="CLP::parse()"; // used for warnings
	
	for(int i=1;i<argc;i++){
		if(argv[i][0] == '-'){ 
			if(argv[i][1] == '-'){
				// Check for switches 
				std::map<std::string,CLS>::iterator mit = _switches.find(argv[i]);
				if(mit != _switches.end()){
					// Insert this switch into the list of switches that are set
					_setSwitchArguments(argv[i],argc,argv,i,mit->second);
				}else{
					
					Warning(methodName,
					        "Undefined switch %1$s.",
					        argv[i]
					);
					
					return false;
				}
			}else{
				// Check for the switches associated with '-'
				std::string temp=argv[i];
				unsigned j=1;
				// For multiple short switches specified together
				while(j < temp.length()){
					std::string shortSwitch="-"+temp.substr(j,1);
					// Find the equivalent name of the shortSwitch 
					std::string switchName = _getShortSwitchNameMapping(shortSwitch);
					if(switchName != "."){
						std::map<std::string,CLS>::iterator mit = _switches.find(switchName);
						_setSwitchArguments(shortSwitch,argc,argv,i,mit->second);
					}else{
						
						Warning(methodName,
						        "Undefined switch %1$s.",
						        shortSwitch.c_str()
						);
						
						return false;
					}
					j++;
					
				}
			}
		}else{
			std::string temp=argv[i];
			
			if((temp.size() > 7 && temp.substr(0,7) == _NETWORK_TYPE) || (temp.size() > 8 && temp.substr(0,8) == _NETWORK_TYPE_SECURE)){
				_networkArguments.push_back(temp);
			}else if(temp.size() > 8 && temp.substr(0,8) == _MYSQL_TYPE){
				_processMysqlArguments(temp.substr(8));
			}else{
				_arguments.push_back(temp);
			}
		}
	}
	return true;
	
}

//
// getArguments:
//
const std::vector<std::string>& CLP::getArguments(){
	
	return _arguments;
	
}

//
// getNetworkArguments:
//
const std::vector<std::string>& CLP::getNetworkArguments(){
	
	return _networkArguments;
	
}

//
// getMysqlArguments:
//
const std::vector<std::string>& CLP::getMysqlArguments(){
	
	return _mysqlArguments;
	
}

///
/// getSwitchArgument: Some of the command line switches require arguments.
/// This method returns the index'th argument specified by the user for a switch.
///
std::string CLP::getSwitchArgument(std::string name,unsigned index){
	
	std::string switchName=name;
	if(name[1] != '-') switchName = _getShortSwitchNameMapping(name);
	
	std::map<std::string,CLS>::iterator mit = _switches.find(switchName);
	std::string temp=".";
	if(mit != _switches.end()){
		if(index > mit->second.getNumberOfSwitchArguments() || index <= 0){
			Warning("CLP::getSwitchArgument()","No command line argument exists for '%s' with index %d.",switchName.c_str(),index);
			return temp;
		}
		return mit->second.getSwitchArgument(index-1);
	}
	return temp;
	
}

//
// DEBUG: printArguments:
//
void CLP::printArguments(void){
	
	for(unsigned i=0;i<_arguments.size();i++) std::cout << _arguments[i] << std::endl;
	
}

//
// CLS (Command Line Switch)
//

//
// Constructor:
//
CLS::CLS(std::string name,std::string shortName,std::string description,unsigned numberOfSwitchArguments){
	
	_name=name;
	_shortName = shortName;
	_description = description;
	_numberOfSwitchArguments = numberOfSwitchArguments;
	_isSet = false;
}


