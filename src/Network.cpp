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

#include <stdlib.h>
#include <string.h>
//
// This is required on some platforms:
//
#include <unistd.h>

////////////////////////////////////////
//
// PUBLIC METHODS:
//
////////////////////////////////////////

///
/// getNetworkFile: Request the file from the specified URL using 'libcurl' library methods.
///
std::string Network::getNetworkFile(const char* url){
	
	char tempname[15] = "";
	int filedescriptor = -1;
	FILE *tempfile;
	strcpy(tempname, "/tmp/netXXXXXX");
	
	if ((filedescriptor = mkstemp(tempname)) == -1 || (tempfile = fdopen(filedescriptor, "w+")) == NULL){
		if (filedescriptor != -1) {
			unlink(tempname);
			close(filedescriptor);
		}
	}
	
	_fileName = tempname;
	curl_global_init(CURL_GLOBAL_ALL);
	_curl = curl_easy_init();
	curl_easy_setopt(_curl, CURLOPT_URL, url);
	curl_easy_setopt(_curl, CURLOPT_NOPROGRESS, 1);
	curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(_curl, CURLOPT_WRITEDATA, tempfile);
	
	if(strncmp(url,"https://",8)==0){
		#ifdef SKIP_PEER_VERIFICATION
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
		#endif
		#ifdef SKIP_HOSTNAME_VERFICATION
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
		#endif
	}
	
	curl_easy_perform(_curl);
	curl_easy_cleanup(_curl);
	
	fclose(tempfile);
	return _fileName;
}
