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
#include <string.h>
//
// This is required on some "legacy" platforms:
//
#ifndef WIN32
#include <unistd.h>
#endif

#include "Exception.h"

////////////////////////////////////////
//
// PUBLIC METHODS:
//
////////////////////////////////////////

///
/// getNetworkFile: Request the file from the specified URL using 'libcurl' library methods.
///
std::string Network::getNetworkFile(const char* url){
	
	/////////////////////////////////////////////////////////////////////////
	//
	// Code for temp file creation is based on CERT's recommendations.  See:
	// https://www.securecoding.cert.org/confluence/display/seccode/VOID+FI039-C.+Create+temporary+files+securely
	//
	/////////////////////////////////////////////////////////////////////////
	
	FILE *sfp=0; // Pointer to a secure temporary file
	
#ifdef WIN32
	
	//
	// WIN32 SYSTEMS:
	//
	
	//
	// Get a temp file name using tmpnam_s():
	//
	char tfname[L_tmpnam_s];
	errno_t err = tmpnam_s( tfname , L_tmpnam_s );
	if(err) throw Exception("Network::getNetworkFile()","Unable to create temporary file name");
	
	sfp = fopen(tfname,"w+");
	
	if(!sfp) throw Exception("Network::getNetworkFile()","Unable to open temporary file %s",tfname);
	
#else
	
	//
	// POSIX SYSTEMS:
	//
	
	char tfname[19] = "/tmp/m2_net.XXXXXX";
	int fd = -1;
	if ((fd = mkstemp(tfname)) == -1 || (sfp = fdopen(fd, "w+")) == NULL) {
		if (fd != -1) {
			unlink(tfname);
			close(fd);
		}
		
		throw Exception("Network::getNetworkFile()","Unable to create or open temporary file %s",tfname);
		
	}
	
#endif
	
	_fileName = tfname;
	//
	// CURL NETWORK CODE:
	//
	
	_fileName = tfname;
	curl_global_init(CURL_GLOBAL_ALL);
	_curl = curl_easy_init();
	curl_easy_setopt(_curl, CURLOPT_URL, url);
	curl_easy_setopt(_curl, CURLOPT_NOPROGRESS, 1);
	curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(_curl, CURLOPT_WRITEDATA, sfp );
	
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
	
	fclose( sfp );
	return _fileName;
	
}
