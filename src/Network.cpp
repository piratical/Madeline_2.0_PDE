/////////////////////////////////////////////////////////
//
// This file is part of the MADELINE 2 program 
// written by Edward H. Trager, Adrian Marrs and Ritu Khanna
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
// Network.cpp
//

#include "Network.h"
#include <stdio.h>
#include <stdlib.h>

//
// This is required on some platforms:
//
#include <unistd.h>

////////////////////////////////////////
//
// PRIVATE METHODS:
//
////////////////////////////////////////

//
// _requestServerResource:
//
void Network::_requestServerResource(const char* url){

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
  
	char sfn[15] = "";
	FILE *sfp;
	int fd = -1;
	strcpy(sfn, "/tmp/netXXXXXX");
	if ((fd = mkstemp(sfn)) == -1 || (sfp = fdopen(fd, "w+")) == NULL)
	{
		if (fd != -1) {
		unlink(sfn);
		close(fd);
		}
	}
	_fileName = sfn;
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
			
			fwrite (tempBuffer , 1 , length-headerOffset , sfp);
			_networkFileString.assign(tempBuffer,length-headerOffset);
			currentSize = length-headerOffset;
			
			// Copy the server response header:
			_serverResponseHeader.assign(buffer,headerOffset);
		}else{
			fwrite (buffer , 1 , length , sfp);
			_networkFileString.append(buffer,length);
			currentSize+=length;
		}
	}
	fclose(sfp);
	// Set the file size:
	_fileSize = currentSize;
	
}

//
// _requestSecureServerResource:
//
void Network::_requestSecureServerResource(const char* url){
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
	
	char sfn[15] = "";
	FILE *sfp;
	int fd = -1;
	strcpy(sfn, "/tmp/netXXXXXX");
	if ((fd = mkstemp(sfn)) == -1 || (sfp = fdopen(fd, "w+")) == NULL)
	{
		if (fd != -1) {
			unlink(sfn);
			close(fd);
		}
	}
	_fileName = sfn;
	
	Netxx::signed_size_type length;
	
	while ( (length = client.read(buffer, sizeof(buffer))) > 0){
		
		if(!headerFlag){
			//
			// Determine the location of the SERVER response header
			// in bytes. 
			// 
			
			start  = buffer;
			headerFlag=true;
			// The server response header terminates with an empty line.
			end = strstr(buffer,"\r\n\r\n");
			end+=4;
			headerOffset = (end - start);
			
			memcpy(tempBuffer,end,length-headerOffset);
			
			//
			// Copy the stream that follows after the header into a file.
			//
			fwrite (tempBuffer , 1 , length-headerOffset , sfp);
			_networkFileString.assign(tempBuffer,length-headerOffset);
			currentSize = length-headerOffset;
			// Copy the server response header:
			_serverResponseHeader.assign(buffer,headerOffset);
		}else{
			_networkFileString.append(buffer,length);
			currentSize+=length;
			fwrite (buffer , 1 , length , sfp);
		}
	}
	fclose(sfp);
	// Set the file size:
	_fileSize = currentSize;
	return;
	
}

//
// _constructRequestHeader:
//
void Network::_constructRequestHeader(std::string& request,const char* addressString){
	
	// setup the request line and headers
	request += " HTTP/1.0\r\n";
	
	// add the Host header
	request += "Host: ";
	request += addressString;
	request += "\r\n";
	request += "Connection: close\r\n\r\n";
	
}

////////////////////////////////////////
//
// PUBLIC METHODS:
//
////////////////////////////////////////

///
/// getNetworkFile: Request the file from the specified URL using 'netxx' library methods.
///
std::string Network::getNetworkFile(const char* url){
	
	if(strncmp(url,"https://",8)==0){
		_requestSecureServerResource(url);
	}else{
		_requestServerResource(url);
	}
	return _networkFileString;
	
}
