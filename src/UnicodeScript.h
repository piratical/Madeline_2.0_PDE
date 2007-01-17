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

#ifndef UNICODESCRIPT_INCLUDED
#define UNICODESCRIPT_INCLUDED

#include "ScriptCodes.h"
#include <string>

/////////////////////
//
// UnicodeScript
//
// NOTE: The sample string is to be used with font metrics 
//       classes and should therefore have sample characters
//       with maximum ascent and maximum descent.  Characters with 
//       maximum width and minimum width might also be relevant.
//       
/////////////////////
class UnicodeScript{
public:
	
	UnicodeScript();
	UnicodeScript(SCRIPTCODE scriptCode,const std::string englishName, std::string nativeName, const std::string sampleFontMetricsString);
	
	std::string getSample(void) const;
	std::string getEnglishName(void) const;
	std::string getNativeName(void) const;
	
	//
	// Comparator:
	//
	struct compare
	{
		bool operator()(const UnicodeScript & m1, const UnicodeScript & m2) const
		{
			return m1._scriptCode < m2._scriptCode ;
		}
	};
	
	
private:
	
	SCRIPTCODE _scriptCode;
	std::string _englishName;
	std::string _nativeName;
	std::string _sampleFontMetricsString;
	
};

#endif
