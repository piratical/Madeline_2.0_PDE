#include "server_request.h"
#include <iostream>
#include <fstream>

std::string requestServerResource(std::string);

std::string getNetworkFile(std::string url){
	
	std::string networkFile;
	
	if(url.find("https://")==0){
		networkFile = requestSecureServerResource(url);
	}else{
		networkFile = requestServerResource(url);
	}
	return networkFile;
	
}

std::string requestServerResource(std::string url){
	
	std::string temp;
	Netxx::Address addr(url.c_str(), 80 );
	Netxx::Stream client(addr, Netxx::Timeout(10));
	char buffer[1024];
	//
	// Construct the GET Request header
	//
	std::string request("GET ");
	if (addr.get_path()) {
	    request += addr.get_path();
	} else {
	    request += "/";
	}

	// setup the request line and headers
	request += " HTTP/1.0";
	request += 13; request += 10;

	// send the Host header
	request += "Host: ";
	request += addr.get_name();
	request += 13; request += 10;

	request += "Connection: close";
	request += 13; request += 10;
	request += 13; request += 10;
	
	// send the request
	client.write(request.c_str(), request.size());

	unsigned int headerOffset,fileSize;
	bool headerFlag = false;
	char *start,*end,tempBuffer[1024];
	char networkBuffer[3092];
	char size[20];
	std::ofstream outfile ("net",std::ofstream::binary);
	Netxx::signed_size_type length;
	while ( (length = client.read(buffer, sizeof(buffer))) > 0){
		if(!headerFlag){
			end = strstr(buffer,"Content-Length:");
			end+=strlen("Content-Length:");
			int i=0;
			while(*end != '\n'){
				size[i++] = *end++;
			}
			//temp = size;
			fileSize = atoi(size);
			//
			// Determine the location of the SERVER response header
			// in bytes. 
			// 
			start = buffer;
			headerFlag=true;
			// The server response header terminates with an empty line.
			end = strstr(buffer,"\r\n\r\n");
			end+=4;
			headerOffset = (end - start);
			//
			// Copy the stream that follows after the header into a file.
			//
			memcpy(tempBuffer,end,length-headerOffset);
			outfile.write(tempBuffer,length-headerOffset);
			memcpy(networkBuffer,tempBuffer,length-headerOffset);
			
			//std::cout.write(tempBuffer,length-headerOffset);
		}else{
			outfile.write(buffer,length);
			strncat(networkBuffer,buffer,length);
			//std::cout.write(buffer,length);
		}
	}
	outfile.close();
	std::cout << "FILESIZE:" << fileSize << std::endl;
	return temp;
	
}


std::string requestSecureServerResource(std::string url){
	
	Netxx::Address addr(url.c_str(), 443);
	Netxx::TLS::Context context;
	Netxx::TLS::Stream client(context, addr, Netxx::Timeout(10));

	char buffer[1024];
	std::string temp;
	//
	// Construct the GET Request header
	//
	std::string request("GET ");
	if (addr.get_path()) {
	    request += addr.get_path();
	} else {
	    request += "/";
	}

	// setup the request line and headers
	request += " HTTP/1.0";
	request += 13; request += 10;

	// send the Host header
	request += "Host: ";
	request += addr.get_name();
	request += 13; request += 10;

	request += "Connection: close";
	request += 13; request += 10;
	request += 13; request += 10;
	
	// send the request
	client.write(request.c_str(), request.size());

	unsigned int headerOffset;
	bool headerFlag = false;
	char *start,*end,tempBuffer[1024];
	std::ofstream outfile ("net",std::ofstream::binary);
	Netxx::signed_size_type length;
	while ( (length = client.read(buffer, sizeof(buffer))) > 0){
		if(!headerFlag){
			//
			// Determine the location of the SERVER response header
			// in bytes. 
			// 
			//std::cout.write(buffer,length);
			start = buffer;
			headerFlag=true;
			// The server response header terminates with an empty line.
			end = strstr(buffer,"\r\n\r\n");
			end+=4;
			headerOffset = (end - start);
			memcpy(tempBuffer,end,length-headerOffset);
			
			outfile.write(tempBuffer,length-headerOffset);
			
			//std::cout.write(tempBuffer,length-headerOffset);
			//
			// Copy the stream that follows after the header into a file.
			//
			//std::cout << tempBuffer << std::endl;
			temp = tempBuffer;
		}else{
			temp+=buffer;
			outfile.write(buffer,length);
			//std::cout.write(buffer,length);
		}
	}
	outfile.close();
	return temp;
	
}


