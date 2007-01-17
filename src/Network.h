#ifndef NETWORK_INCLUDED
#define NETWORK_INCLUDED

#include <iostream>
#include <fstream>
#include <cstring>
//#include <netxx/netxx.h>
//#include <netxx/tls/netxx.h>
#include "../3rdParty/netxx/netxx/netxx.h"
#include "../3rdParty/netxx/netxx/tls/netxx.h"

/*!
 * This class is used to read in a file over the network. After it has read in the file, it then saves it to a temp file.
*/

class Network{
private:
	std::string _networkFileString;
	std::string _serverResponseHeader;
	unsigned int _fileSize;
  std::string _fileName;
	//
	// Private Methods:
	//
	void _constructRequestHeader(std::string&,const char*);
	void _requestServerResource(const char*);
	void _requestSecureServerResource(const char*);
	
public:
	//
	// Getters:
	//
	std::string getNetworkFile(const char*);
	std::string getServerResponseHeader() const { return _serverResponseHeader; }
	unsigned int getFileSize() const{ return _fileSize; }
  std::string getFileName() {return _fileName;}
};
#endif
