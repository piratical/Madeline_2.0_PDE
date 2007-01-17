/////////////////////////////////////////////////////////
//
// This file is part of the MADELINE 2 program 
// written by Edward H. Trager and Ritu Khanna
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
// 2006.12.18.ET Restructured as a class derived from Message
//

//
// Exception.cpp
//

#include "Exception.h"

const char *Exception::_exceptionSalutation="Unrecoverable Error";

//
// Exception() constructor:
//
Exception::Exception( const char *const methodName , const char *format , ... ){

	//
	// The Exception class uses the Exception salutation:
	//
	_salutation = gettext(_exceptionSalutation);
	_methodName = methodName;
	int n;
	va_list args;
	if(strlen(format) > 0){
		va_start(args,format);
		n=vsnprintf(_message,GENERAL_STRING_BUFFER_SIZE,gettext(format),args);
		va_end(args);
		_truncated=(n==GENERAL_STRING_BUFFER_SIZE);
	}
	
	Message::print();
	
}
