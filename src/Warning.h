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
// Warning.h
//

#ifndef WARNING_INCLUDED
#define WARNING_INCLUDED

#include "Message.h"

//#include <iostream>
//#include <libintl.h>
//#include <stdarg.h>
//#include "BufferSizes.h"

class Warning : public Message{
	
private:
	
	static bool _suppressWarnings;
	static const char *_warningSalutation;
public:
	
	Warning(const char *const methodName, const char *format,...);
	static void suppressWarnings(bool suppressWarnings);
	void print(void);
	
};

#endif

