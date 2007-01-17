#ifndef NETWORK_INCLUDED
#define NETWORK_INCLUDED

#include <iostream>
#include <fstream>
#include "netxx/netxx.h"
#include "netxx/tls/netxx.h"


class Network{
private:
	std::string _networkFileString;
	unsigned int _fileSize;
	//
	// Private Methods:
	//
	void _requestServerResource(std::string);
	void _requestSecureServerResource(std::string);
	
public:
	//
	// Public Methods:
	//
	std::string getNetworkFile(std::string);
	unsigned int getFileSize() const{ return _fileSize; } 
};
#endif
