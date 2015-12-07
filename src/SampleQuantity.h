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
// 2015.12.07.ET
//

//
// SampleQuantity.h
//

#ifndef SampleQuantity_INCLUDED
#define SampleQuantity_INCLUDED

#include "String.h"
#include "Warning.h"

class SampleQuantity : public String
{
public:
	
	
private:
	
	std::string _marker;
	
public:
	//
	// Constructors:
	//
	SampleQuantity() { _isMissing = true; }
	SampleQuantity( const char *sampleQuantityMarker) { set(sampleQuantityMarker); }
	SampleQuantity( const std::string& sampleQuantityMarker) { set(sampleQuantityMarker); }
	
	//
	// Setters:
	//
	void set(const char *sampleQuantityMarker){ 
		
		_marker=sampleQuantityMarker;
		if(_marker=="."){
			_isMissing=true;
		}
	}
	
	void set(const std::string& sampleQuantityMarker){ 
		
		_marker=sampleQuantityMarker;
		if(_marker=="."){
			_isMissing=true;
		}
	}
	
	// get():  Returns the original marker set by the user
	const std::string get(void) const { 
		
		if(_isMissing) return ".";
		return _marker;
		
	}
	
	static bool isa(std::string inString);
	
};


#endif
