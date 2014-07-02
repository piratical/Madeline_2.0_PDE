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
// 2005.09.08.ET
//

//
// Twin.h
//

#ifndef Twin_INCLUDED
#define Twin_INCLUDED

#include "String.h"
#include "Warning.h"

class Twin : public String
{
public:
	
	enum TWIN_TYPE{ MONOZYGOTIC_TWIN, DIZYGOTIC_TWIN, ZYGOSITY_UNKNOWN_TWIN ,MISSING_TWIN};
	
private:
	
	TWIN_TYPE _twinType;
	bool _validateTwinMarker(std::string twinMarker);
	
public:
	//
	// Constructors:
	//
	Twin() { _isMissing = true; }
	Twin( const char *twinMarker, TWIN_TYPE twinType) { set(twinMarker, twinType); }
	Twin( const std::string twinMarker, TWIN_TYPE twinType) { set(twinMarker, twinType); }
	
	//
	// Setters:
	//
	void set(const char *twinMarker,TWIN_TYPE twinType){ 
		
		if(twinType == Twin::ZYGOSITY_UNKNOWN_TWIN || twinType == Twin::MISSING_TWIN || _validateTwinMarker(twinMarker)){
			 _twinType = twinType; 
			// To distinguish between twin types with the same marker a twin type character is appended to the  markers
			std::string temp = twinMarker;
			if(twinMarker == std::string("."));
			else{
				temp+= getTwinType();
				 String::set(temp);
				 _isMissing = false;
			}
		}else{
			_isMissing=true;
			_value = ".";
			Warning("Twin::set()",
			        "%s is an invalid TwinMarker and has been set to Missing. Allowed TwinMarkers are [A-Za-z]",
			        twinMarker
			);
		}
		
	}
	
	void set(const std::string &twinMarker, TWIN_TYPE twinType){ 
		
		if(twinType == Twin::ZYGOSITY_UNKNOWN_TWIN || twinType == Twin::MISSING_TWIN || _validateTwinMarker(twinMarker)){
			std::string temp = twinMarker;
			
			_twinType = twinType; 
			// To distinguish between twin types with the same marker a twin type character is appended to the markers
			if(twinMarker == "."); 
			else{ 
				_isMissing = false;
				temp += getTwinType();
				
			}
			String::set(temp);
		}else{
			_isMissing=true;
			_value=".";
			
			Warning("Twin::set()",
			        "%s is an invalid TwinMarker and has been set to Missing. Allowed TwinMarkers are [A-Za-z]",
			        twinMarker.c_str()
			);
		}
		
	}
	
	// These are required to satisfy the virtual functions of the base class:
	void set(const std::string &twinMarker){
		set(twinMarker,ZYGOSITY_UNKNOWN_TWIN);
	}
	
	void set(const char*twinMarker){
		set(twinMarker,ZYGOSITY_UNKNOWN_TWIN);
	}
	
	
	// get():  Returns the original marker set by the user
	const std::string get(void) const { 
		
		if(_isMissing) return ".";
		// Removes the twin type character that is appended to the marker
		std::string temp = String::get();
		return temp.erase(1);
		
	}
	
	//
	// Get methods not present in base "String" class:
	//
	const std::string getTwinType( void ) const;
	const TWIN_TYPE getTwinTypeEnum( void ) const { return _twinType; }
	// Returns the marker that includes the distinguishing twin type character between twin types:
	const std::string getMarker( void ) const { return String::get(); }
	
	static bool isa(std::string inString);
	
};




#endif
