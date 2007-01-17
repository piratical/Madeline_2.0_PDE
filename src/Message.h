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
// 2005.03.14.ET -- Originally written as Warning class
// 2006.12.18.ET -- Reimplemented as Message base class

//
// Message.h
//


#ifndef MESSAGE_INCLUDED
#define MESSAGE_INCLUDED

#include <iostream>
#include <libintl.h>
#include <locale.h>
#include <stdarg.h>
#include "BufferSizes.h"
#include <string>

// Loader class:
class MessageInitializer;

class Message{
	
	friend class MessageInitializer;
	
private:
	
	static const char *_defaultSalutation;
	
	// initialize:
	static void _initialize( void );
	
protected:
	
	const char *_salutation;
	const char *_methodName;
	char _message[GENERAL_STRING_BUFFER_SIZE];
	bool _truncated;
	
public:
	
	// Constructors:
	Message(){};
	Message(const char *const methodName, const char *format,...);
	
	// print:
	void print(void);
	
	// get:
	std::string get(void) const;
};

//
// MessageInitializer loader class:
//
class MessageInitializer{
	
	static MessageInitializer messageInitializer;
	MessageInitializer(){
		Message::_initialize();
	}
	
};

#endif

