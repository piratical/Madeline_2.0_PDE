#include "Network.h"


std::string Network::getNetworkFile(std::string url){
	
	if(url.find("https://")==0){
		_requestSecureServerResource(url);
	}else{
		_requestServerResource(url);
	}
	return _networkFileString;
	
}

void Network::_requestServerResource(std::string url){
	
	
	Netxx::Address addr(url.c_str(), 80 );
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
	unsigned int headerOffset,currentSize;
	bool headerFlag = false;
	char *start,*end,tempBuffer[1024],size[20];
	char buffer[1024];
	std::ofstream outfile ("net",std::ofstream::binary);
	Netxx::signed_size_type length;
	
	while ( (length = client.read(buffer, sizeof(buffer))) > 0){
		//std::cout << "Length is " << length << std::endl;
		if(!headerFlag){
			//end = strstr(buffer,"Content-Length:");
			//std::cout << "End is " << end << std::endl;
			//end+=strlen("Content-Length:");
			//int i=0;
			//while(*end != '\n'){
			//	size[i++] = *end++;
			//}
			
			//_fileSize = atoi(size);
			//_fileSize = length;
			//std::cout << "Filesize is " << _fileSize << std::endl;
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
			_networkFileString.assign(tempBuffer,length-headerOffset);
			currentSize = length-headerOffset;
			//std::cout.write(tempBuffer,length-headerOffset);
		}else{
			outfile.write(buffer,length);
			//_networkFileString+=buffer;
			_networkFileString.append(buffer,length);
			currentSize+=length;
			//std::cout.write(buffer,length);
		}
	}
	outfile.close();
	_fileSize = currentSize;
	std::cout << "CurrentSize is  " << currentSize << std::endl;
}


void Network::_requestSecureServerResource(std::string url){
	
	Netxx::Address addr(url.c_str(), 443);
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
	
	char buffer[1024],tempBuffer[1024],size[20];
	unsigned int headerOffset,currentSize;
	bool headerFlag = false;
	char *start,*end;
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
			_fileSize = atoi(size);
			//std::cout << "Filesize is:" << _fileSize << std::endl;
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
			outfile.write(tempBuffer,length-headerOffset);
			
			//std::cout.write(tempBuffer,length-headerOffset);
			//
			// Copy the stream that follows after the header into a file.
			//
			_networkFileString.assign(tempBuffer,length-headerOffset);
			currentSize = length-headerOffset;
		}else{
			_networkFileString.assign(buffer,currentSize,length);
			
			currentSize+=length;
			outfile.write(buffer,length);
			//std::cout.write(buffer,length);
		}
	}
	outfile.close();
	return;
	
}

