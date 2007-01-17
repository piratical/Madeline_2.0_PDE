#ifndef NETWORK_INCLUDED
#define NETWORK_INCLUDED

#include <iostream>
#include <fstream>
#include "netxx/netxx.h"
#include "netxx/tls/netxx.h"


class Network{
private:
	std::string _networkFileString;
	std::string _serverResponseHeader;
	unsigned int _fileSize;
	//
	// Private Methods:
	//
	void _constructRequestHeader(std::string&,const char*);
	void _requestServerResource(char*);
	void _requestSecureServerResource(char*);
	
public:
	//
	// Getters:
	//
	std::string getNetworkFile(char*);
	std::string getServerResponseHeader() const { return _serverResponseHeader; }
	unsigned int getFileSize() const{ return _fileSize; } 
};
#endif
