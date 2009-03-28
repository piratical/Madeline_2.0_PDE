#ifndef NETWORK_INCLUDED
#define NETWORK_INCLUDED

#include <iostream>
#include <curl/curl.h>

/*!
 * This class is used to read in a file over the network. After it has read in the file, it then saves it to a temp file.
*/

class Network{
private:
	std::string _fileName;
	CURL *_curl;
	CURLcode _res;
	
public:
	//
	// Getters:
	//
	std::string getNetworkFile(const char*);
	std::string getFileName() {return _fileName;}
	
};

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream){
	int written = fwrite(ptr, size, nmemb, (FILE *)stream);
	return written;
}

#endif
