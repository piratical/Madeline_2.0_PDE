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
// 2005.03.14.ET
//

//
// Exception.h
//

#ifndef EXCEPTION_INCLUDED
#define EXCEPTION_INCLUDED

#include "Message.h"

class Exception : public Message{
	
private:
	
	static const char *_exceptionSalutation;
	
public:
	
	Exception(const char *const methodName, const char *format,...);
	
};

#endif

