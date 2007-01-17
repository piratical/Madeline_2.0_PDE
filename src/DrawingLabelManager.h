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

#ifndef DRAWINGLABELMANAGER_INCLUDED
#define DRAWINGLABELMANAGER_INCLUDED

#include "ScriptCodes.h"
#include "UnicodeScript.h"
#include "Utf8String.h"
#include "LabelSet.h"
#include <string>
#include <map>

//
// For font metrics
// and Postscript output:
// 
#include <LASi.h>

/////////////////////
//
// DrawingLabelManager
//
/////////////////////
class DrawingLabelManager{
	
	
public:
	//
	// Constructor:
	//
	DrawingLabelManager();
	
	//
	// setPostscriptDocument
	//
	void setPostscriptDocument(LASi::PostscriptDocument* doc);
	//
	// setLabelSet
	//
	void setLabelSet(const LabelSet* labelSet);
	//
	// fitStringToLabelWidth():
	//
	std::string fitStringToLabelWidth(UTF8String label);
	
private:
	friend class DrawingLabelManagerLoader;
	
	//
	// The _latinBlock is basic Latin (ASCII):
	//
	static UnicodeScript _latinScript;
	
	//
	// The _aggregateBlock sample is sufficient for determining
	// good line spacing metrics for most Unicode blocks beyond basic Latin:
	// 
	static UnicodeScript _aggregatedScripts;
	//
	// The following set of _unicodeBlocks consists primarily of Indic and Indic-derived scripts
	// which have vowels or other diacritical marks both above and below the consonants
	// which requires greater inter-line spacing:
	//
	static std::map<SCRIPTCODE,UnicodeScript> _unicodeScripts;
	
	bool _hasUnicode;
	bool _hasIndicOrArabic;
	
	double _yMaximum;
	double _yMinimum;
	double _lineHeight;
	
	unsigned _labelCount;
	
	LASi::PostscriptDocument* _doc;
};


///////////////////////////////////
//
// DrawingLabelManagerLoader 
// 
// is the Friend class that 
// initializes the DrawingLabel 
// class:
//
///////////////////////////////////
class DrawingLabelManagerLoader
{
public:
	static DrawingLabelManagerLoader drawingLabelManagerLoader;
	
	DrawingLabelManagerLoader();
	
};


#endif
