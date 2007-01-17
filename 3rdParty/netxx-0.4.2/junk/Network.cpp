#include "Network.h"


std::string Network::getNetworkFile(char* url){
	
	if(strncmp(url,"https://",8)==0){
		_requestSecureServerResource(url);
	}else{
		_requestServerResource(url);
	}
	return _networkFileString;
	
}

void Network::_requestServerResource(char* url){
	
	
	Netxx::Address addr(url, 80 );
	Netxx::Stream client(addr, Netxx::Timeout(10));
	//
	// Construct the GET Request header
	//
	std::string request("GET ");
	if (addr.get_path()) {
	    request += addr.get_path();
	} else {
	    request += "/";
	}
	_constructRequestHeader(request,addr.get_name());
	// Send the request to the server
	client.write(request.c_str(), request.size());
	
	unsigned int headerOffset,currentSize;
	bool headerFlag = false;
	char *start,*end,buffer[1024],tempBuffer[1024];
	std::ofstream outfile ("net",std::ofstream::binary);
	Netxx::signed_size_type length;
	
	while ( (length = client.read(buffer, sizeof(buffer))) > 0){
		
		if(!headerFlag){
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
			// Debugging: Write to a file:
			outfile.write(tempBuffer,length-headerOffset);
			_networkFileString.assign(tempBuffer,length-headerOffset);
			currentSize = length-headerOffset;
			
			// Debugging:
			// std::cout.write(buffer,headerOffset);
			
			// Copy the server response header:
			_serverResponseHeader.assign(buffer,headerOffset);
		}else{
			outfile.write(buffer,length);
			_networkFileString.append(buffer,length);
			currentSize+=length;
		}
	}
	outfile.close();
	// Set the file size:
	_fileSize = currentSize;
	
}


void Network::_requestSecureServerResource(char* url){
	
	Netxx::Address addr(url, 443);
	Netxx::TLS::Context context;
	Netxx::TLS::Stream client(context, addr, Netxx::Timeout(10));
	
	//
	// Construct the GET Request header
	//
	std::string request("GET ");
	if (addr.get_path()) {
	    request += addr.get_path();
	} else {
	    request += "/";
	}
	
	_constructRequestHeader(request,addr.get_name());
	
	// send the request
	client.write(request.c_str(), request.size());
	
	char buffer[1024],tempBuffer[1024];
	unsigned int headerOffset,currentSize;
	bool headerFlag = false;
	char *start,*end;
	std::ofstream outfile ("net",std::ofstream::binary);
	Netxx::signed_size_type length;
	
	while ( (length = client.read(buffer, sizeof(buffer))) > 0){
		
		if(!headerFlag){
			// Debugging:
			// std::cout << "Filesize is:" << _fileSize << std::endl;
			
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
			
			memcpy(tempBuffer,end,length-headerOffset);
			// Debugging: write to a file
			outfile.write(tempBuffer,length-headerOffset);
			
			// Debugging:
			// std::cout.write(tempBuffer,length-headerOffset);
			
			//
			// Copy the stream that follows after the header into a file.
			//
			_networkFileString.assign(tempBuffer,length-headerOffset);
			currentSize = length-headerOffset;
			// Copy the server response header:
			_serverResponseHeader.assign(buffer,headerOffset);
		}else{
			_networkFileString.append(buffer,length);
			currentSize+=length;
			outfile.write(buffer,length);
		}
	}
	outfile.close();
	// Set the file size:
	_fileSize = currentSize;
	return;
	
}

void Network::_constructRequestHeader(std::string& request,const char* addressString){
	
	// setup the request line and headers
	request += " HTTP/1.0\r\n";
	
	// add the Host header
	request += "Host: ";
	request += addressString;
	request += "\r\n";
	request += "Connection: close\r\n\r\n";
	
}

