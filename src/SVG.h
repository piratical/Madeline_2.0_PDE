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

#ifndef SVG_INCLUDED
#define SVG_INCLUDED

#define _USE_MATH_DEFINES

#include "DrawingMetrics.h"
#include <cmath>
#include <sstream>

class SVG{
	
private:
	
public:
	SVG() {}
	
	// Drawing Methods:
	void startGroup(std::ostringstream& os,const std::string& cssClass=std::string(""),const std::string& id=std::string(""));
	void endGroup(std::ostringstream& os);
	void drawTitle(std::ostringstream& os,const std::string title){
		os << title;
	}
	void drawLine(std::ostringstream& os,double x1,double y1,double x2,double y2,const std::string& cssClass=std::string(""),const std::string& id=std::string(""));
	void drawHorizontalLine(std::ostringstream& os,double y,double x1,double x2,const std::string& cssClass=std::string(""),const std::string& id=std::string(""));
	void drawVerticalLine(std::ostringstream& os,double x,double y1,double y2,const std::string & cssClass=std::string(""), const std::string & id=std::string(""));
	void drawPolyline(std::ostringstream& os,const std::string points,const std::string& cssClass=std::string(""),const std::string& id=std::string(""));
	void drawText(std::ostringstream& os,double x,double y,std::string text,const std::string&  cssClass=std::string(""),const std::string& id=std::string(""));
	void drawRectangle(std::ostringstream& os,double x,double y,double width,double height,const std::string& cssClass=std::string(""),const std::string& id=std::string(""),const std::string& style=std::string(""));
	
	//
	// Low level methods for drawing individuals:
	//
	void drawMaleIcon(std::ostringstream& os,double x, double y, const std::string &id = std::string(""), const std::string &cssClass = std::string(""));
	void drawFemaleIcon(std::ostringstream& os,double x, double y, const std::string &id = std::string(""), const std::string &cssClass = std::string(""));
	void drawGenderUnknownIcon(std::ostringstream& os,double x, double y, const std::string &id = std::string(""), const std::string &cssClass = std::string(""));
	void drawIconForNoChildren(std::ostringstream& os,double x, double y);
	void drawIconForInfertility(std::ostringstream& os,double x, double y);
	void drawIconForAbortedPregnancy(std::ostringstream& os,double x, double y, const std::string &id = std::string(""), const std::string &cssClass = std::string(""));
	
	void drawAdoptedIn(std::ostringstream& os,double x,double y,bool widerMargin=false);
	void drawAdoptedOut(std::ostringstream& os,double x,double y,bool widerMargin=false);
	void drawMiniMaleIcon(std::ostringstream& os,double x,double y);   // Only used for aborted/terminated pregnancies.
	void drawMiniFemaleIcon(std::ostringstream& os,double x,double y); // Only used for aborted/terminated pregnancies.
	void drawMiniCircle(std::ostringstream& os,double x,double y);     // Used to show carrier status
	void drawMiniOpenCircle(std::ostringstream& os,double x,double y); // Used to show obligate carrier status
	void drawHeart(std::ostringstream& os,double x,double y);
	void drawSquiggle(std::ostringstream& os,double x,double y);       // Used for RelationshipEnded ...
	void drawCurve(std::ostringstream& os, double x1, double y1, double x2, double y2);
	 
	void drawPregnancyLetterP(std::ostringstream& os,double x,double y, const std::string & inkClass="blackInk");   // Double-struck P to indicate a pregnancy
	void drawCollapsedCount(std::ostringstream& os,double x,double y,unsigned collapsedCount); // Large centered numerals to indicate number of individuals
	void drawTestTube(std::ostringstream& os,double x,double y);


};

#endif
