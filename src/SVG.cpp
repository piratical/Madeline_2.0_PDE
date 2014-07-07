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
// 2006.12.29.RK
//
// Last updated 2011.11.18.ET
//

//
// SVG.cpp
//

#include "SVG.h"

//
// drawLine:
//
void SVG::drawLine(std::ostringstream& os, double x1,double y1,double x2,double y2,const std::string& cssClass,const std::string& id){
	
	os  << "  <line x1=\"" << x1 << "\" y1=\"" << y1 << "\" x2=\"" << x2 << "\" y2=\"" << y2 << "\" />\n";
	
}

//
// drawHorizontalLine:
//
void SVG::drawHorizontalLine(std::ostringstream& os,double y, double x1, double x2,const std::string& cssClass,const std::string& id){
	
	 os << "  <line x1=\"" << x1 << "\" y1=\"" << y << "\" x2=\"" << x2 << "\" y2=\"" << y << "\" />\n";
	
}

//
// drawVerticalLine:
//
void SVG::drawVerticalLine(std::ostringstream& os,double x, double y1, double y2,const std::string & cssClass, const std::string & id){
	
	os << "  <line ";
	if(cssClass != std::string("") ) os << "class=\"" << cssClass <<"\" ";
	if(id        != std::string("") ) os << "id=\""    << id    <<"\" ";
	os << "x1=\"" << x << "\" y1=\"" << y1 << "\" x2=\"" << x << "\" y2=\"" << y2 << "\" />\n";
	
}

//
// drawPolyline:
//
void SVG::drawPolyline(std::ostringstream& os,const std::string points,const std::string& cssClass,const std::string& id){
	
	os << "  <polyline points=\"" << points << "\" ";
	if(id        != std::string("") ) os << "id=\""    << id    <<"\" ";
	if(cssClass     != std::string("") ) os << "class=\""    << cssClass    <<"\" ";
	os << " />\n";
	
}

//
// drawText:
//
void SVG::drawText(std::ostringstream& os,double x,double y,std::string text,const std::string& cssClass,const std::string& id){
	
	os << "  <text x=\"" << x << "\" y=\"" << y << "\" text-anchor=\"middle\" ";
	if(cssClass!="") os << " class=\"" << cssClass << "\" ";
	os << ">" << text << "</text>\n";
	
}

//
// drawRectangle:
//
void SVG::drawRectangle(std::ostringstream& os,double x,double y,double width,double height,const std::string& cssClass,const std::string& id,const std::string& style){
	
	os  << "  <rect  x=\"" << x << "\" y=\"" << y << "\" width=\"" << width << "\" height=\"" << height << "\" ";
	if(style != std::string("")) os << "style=\"" << style << "\" ";
	if(cssClass != std::string("")) os << "class=\"" << cssClass << "\" ";
	os << " />\n";
	
}

//
// startGroup:
//
void SVG::startGroup(std::ostringstream& os,const std::string& cssClass,const std::string& id){
	
	os << "  <g"; 
	if(cssClass!=std::string("")) os << " class=\"" << cssClass << "\"";
	os << ">\n";
	
}

//
// endGroup:
//
void SVG::endGroup(std::ostringstream& os){
	
	os << "  </g>\n";
	
}

//
// drawMaleIcon()
//
void SVG::drawMaleIcon(std::ostringstream& os,double x, double y, const std::string &id, const std::string &cssClass){
	
	double d = DrawingMetrics::getIconDiameter();
	double r = 0.5 * d;
	x -= r;
	y -= r;
	os << "  <rect x=\"" << x << "\" y=\"" << y << "\" width=\"" << d << "\" height=\"" << d << "\"";
	if(id.size()   ) os << " id=\"" << id << "\"";
	if(cssClass.size()) os << " class=\"" << cssClass << "\"";
	
	if(id.size()){
		os << ">\n";
		os << "\t<title>" << id << "</title>\n";
		os << "</rect>\n";
	}else{
		
		os << "/>\n";
		
	}
	
}

//
// drawFemaleIcon()
//
void SVG::drawFemaleIcon(std::ostringstream& os,double x, double y, const std::string &id, const std::string &cssClass){
	
	os << "<circle cx=\"" << x << "\" cy=\"" << y << "\" r=\"" << DrawingMetrics::getIconRadius() << "\"";
	if(id.size()   ) os << " id=\"" << id << "\"";
	if(cssClass.size()) os << " class=\"" << cssClass << "\"";
	
	if(id.size()){
		os << ">\n";
		os << "\t<title>" << id << "</title>\n";
		os << "</circle>\n";
	}else{
		
		os << "/>\n";
		
	}
	
}


//
// drawGenderUnknownIcon()
//
void SVG::drawGenderUnknownIcon(std::ostringstream& os,double x, double y, const std::string &id, const std::string &cssClass){
	
	double r = DrawingMetrics::getIconRadius();
	double d = 2.0 * r / M_SQRT2;
	y -= r;
	os << "<path d=\"M " << x << " " << y;
	os << " l " << -d << " " << d;
	os << " " << d << " " << d;
	os << " " << d << " " << -d;
	os << " z\"";
	if(id.size()   ) os << " id=\"" << id << "\"";
	if(cssClass.size()) os << " class=\"" << cssClass << "\"";
	
	if(id.size()){
		os << ">\n";
		os << "\t<title>" << id << "</title>\n";
		os << "</path>\n";
	}else{
		
		os << "/>\n";
		
	}
	
}

//
// drawIconForNoChildren()
//
void SVG::drawIconForNoChildren(std::ostringstream& os,double x, double y){
	
	double d = DrawingMetrics::getIconDiameter();
	double r = 0.5 * d;
	x -= r;
	y -= r;
	double x2 = x+d;
	
	os << "<line x1=\"" << x << "\" y1=\"" << y << "\" x2=\"" << x2 << "\" y2=\"" << y << "\" class=\"solid\" />\n";
	
}

//
// drawIconForInfertility()
//
void SVG::drawIconForInfertility(std::ostringstream& os,double x, double y){
	
	double d = DrawingMetrics::getIconDiameter();
	double r = 0.5 * d;
	x -= r;
	y -= r;
	double x2 = x+d;
	
	os << "<line x1=\"" << x << "\" y1=\"" << y << "\" x2=\"" << x2 << "\" y2=\"" << y << "\" class=\"solid\" />\n";
	// Create 2nd line:
	y+= 1.0*DrawingMetrics::getScalingFactor();
	os << "<line x1=\"" << x << "\" y1=\"" << y << "\" x2=\"" << x2 << "\" y2=\"" << y << "\" class=\"solid\" />\n";
	
}

//
// drawIconForAbortedPregnancy
// 
void SVG::drawIconForAbortedPregnancy(std::ostringstream& os,double x, double y, const std::string &id, const std::string &cssClass){
	
	double r = DrawingMetrics::getIconRadius();
	y -= r;
	r*=M_SQRT2;
	
	os << "<path d=\"M " << x << " " << y;
	os << " l " << -r << " " << r;
	os << " " << 2.0*r << " " << 0;
	os << " z\"";
	if(id.size()   ) os << " id=\"" << id << "\"";
	if(cssClass.size()) os << " class=\"" << cssClass << "\"";
	
	if(id.size()){
		os << ">\n";
		os << "\t<title>" << id << "</title>\n";
		os << "</path>\n";
	}else{
		
		os << "/>\n";
		
	}
	
}

//
// drawAdoptedIn: - Draws brackets around an individual like this: [ ]
//                - Conforms to PSTF (Bennet et al.)
//
void SVG::drawAdoptedIn(std::ostringstream& os,double x,double y,bool widerMargin){
	
	double d = DrawingMetrics::getIconDiameter();
	double r = 0.5 * d;
	double t = DrawingMetrics::getVerticalTick();
	double v = d+t+t;
	
	x -= r;
	if(widerMargin) x -= 2*t;
	y -= r;
	y -= t;
	
	os << "<path d=\"M " << x << " " << y;
	os << " l " << -t << " " << 0;
	os << " "   <<  0 << " " << v;
	os << " "   <<  t << " " << 0;
	os << "\" class=\"solid\" />\n";
	
	x += d;
	if(widerMargin) x += 4*t;
	
	os << "<path d=\"M " << x << " " << y;
	os << " l " <<  t << " " << 0;
	os << " "   <<  0 << " " << v;
	os << " "   << -t << " " << 0;
	os << "\" class=\"solid\" />\n";
	
}

//
// drawAdoptedOut: - Draws brackets around an individual like this: ] [
//                 - This is intentionally different from the PSTF
//                   (Bennet et al.) recommendations.
//
void SVG::drawAdoptedOut(std::ostringstream& os,double x,double y,bool widerMargin){
	
	double d = DrawingMetrics::getIconDiameter();
	double r = 0.5 * d;
	double t = DrawingMetrics::getVerticalTick();
	double v = d+2*t;
	
	x -= r;
	x -= 2*t;
	if(widerMargin) x -= 2*t;
	
	y -= r;
	y -= t;
	
	os << "<path d=\"M " << x << " " << y;
	os << " l " <<  t << " " << 0;
	os << " "   <<  0 << " " << v;
	os << " "   <<  -t << " " << 0;
	os << "\" class=\"solid\" />\n";
	
	x += d;
	x += 4*t;
	if(widerMargin) x += 4*t;
	
	os << "<path d=\"M " << x << " " << y;
	os << " l " << -t << " " << 0;
	os << " "   <<  0 << " " << v;
	os << " "   <<  t << " " << 0;
	os << "\"  class=\"solid\" />\n";
	
}

//
// drawMiniMaleIcon: - Draws a miniature-sized male icon
//                     to indicate gender of terminated or aborted pregnancies
//                     - Not part of PSTF recommendation
//
void SVG::drawMiniMaleIcon(std::ostringstream& os,double x,double y){
	
	double r  = DrawingMetrics::getIconRadius();
	double hr = 0.5*r;
	
	x -= hr;
	y += hr;
	y += DrawingMetrics::getVerticalTick();
	
	os << "<path d=\"M " << x << " " << y;
	os << " l " <<  0 << " " << r;
	os << " "   <<  r << " " << 0;
	os << " "   <<  0 << " " << -r;
	os << " z ";
	os << "\" class=\"solid\" />\n";
	
}

//
// drawMiniFemaleIcon: - Draws a miniature-sized female icon
//                       to indicate gender of terminated or aborted pregnancies
//                     - Not part of PSTF recommendation
//
void SVG::drawMiniFemaleIcon(std::ostringstream& os,double x,double y){
	
	double r  = DrawingMetrics::getIconRadius();
	double hr = 0.5*r;
	
	y += r;
	y += DrawingMetrics::getVerticalTick();
	
	os << "<circle cx=\"" << x << "\" cy=\"" << y << "\" r=\"" << hr << "\"";
	os << " class=\"solid\" />\n";
	
}

//
// drawMiniCircle: - Draws a small filled circle indicating carrier status
//
void SVG::drawMiniCircle(std::ostringstream& os,double x,double y){
	
	double r  = DrawingMetrics::getIconRadius();
	double hr = 0.25*r;
	
	os << "<circle cx=\"" << x << "\" cy=\"" << y << "\" r=\"" << hr << "\"";
	os << " class=\"filled\" />\n";
	
}

//
// drawHeart
//
void SVG::drawHeart(std::ostringstream& os,double x,double y){
	
	double  sc=4;
	std::string sp=" ";
	
	y+=7;
	
	os << "<path class=\"specialIcons\"";
	os << " d=\"M" << x << sp << y << sp;
	os << "c" << -0.7*sc << sp << -0.3*sc << sp <<   -2*sc << sp << -1.5*sc << sp << -2*sc << sp << -2.5*sc << sp;
	os << "c" <<    0*sc << sp << -0.5*sc << sp <<  0.5*sc << sp <<   -1*sc << sp <<  1*sc << sp <<   -1*sc << sp;
	os << "c" <<  0.5*sc << sp <<    0*sc << sp <<    1*sc << sp <<  0.4*sc << sp <<  1*sc << sp <<    1*sc << sp;
	os << "c" <<    0*sc << sp << -0.6*sc << sp <<  0.5*sc << sp <<   -1*sc << sp <<  1*sc << sp <<   -1*sc << sp;
	os << "c" <<  0.5*sc << sp <<    0*sc << sp <<    1*sc << sp <<  0.5*sc << sp <<  1*sc << sp <<    1*sc << sp;
	os << "c" <<    0*sc << sp <<    1*sc << sp << -1.3*sc << sp <<  2.2*sc << sp << -2*sc << sp <<  2.5*sc << sp;
	os << "z\" />\n";

}

//
// drawSquiggle
//
void SVG::drawSquiggle(std::ostringstream& os,double x,double y){
	
	double sc=1.25;
	std::string sp=" ";
	
	x-=10;
	y+=14;
	
	os << "<path class=\"specialLines\"";
	os << " d=\"M" << x << sp << y << sp;
	for(int i=0;i<4;i++){
		os << "c" <<  1.25*sc << sp <<   -1.25*sc << sp << -1.25*sc << sp << -3.75*sc << sp << 0*sc << sp << -5*sc << sp;
		os << "c" <<  1.25*sc << sp <<   -1.25*sc << sp <<  3.75*sc << sp <<  1.25*sc << sp << 5*sc << sp <<  0*sc << sp;
	}
	os << "\" />\n";
	
}


//
// drawCurve -- For showing curved connectors in the background -- NEEDS WORK !
// 2009.05.21.ET ADDENDUM
//
// dxc, dyc specify a *relative* offset from the first end point.  The
// second control point will then be a symmetric reflected control point from the
// second end point.
//
void SVG::drawCurve(std::ostringstream& os, double x1, double y1, double x2, double y2){
	
	//if(x2<x1){
	//	double xx = x1;
	//	x1=x2;
	//	x2=xx;
	//}
	
	//if(y2<y1){
	//	double yy = y1;
	//	y1=y2;
	//	y2=yy;
	//}
	
	std::string sp=" ";
	
	double dx = x2-x1;
	double dy = y2-y1;
	
	double h = sqrt(dx*dx + dy*dy);
	
	//
	// Calculate relative displacement for
	// control points dxc, dyc:
	//
	double dxc = 0.25*h;   // a quarter of distance h seems about right ...
	double dyc = 5*DrawingMetrics::getIconDiameter(); // This just seems about right ...
	
	
	// Control point #1 (when p1 is translated to the origin):
	double c1x = (dxc*dx-dyc*dy)/h;
	double c1y = (dxc*dy+dyc*dx)/h;
	
	// Translate:
	c1x += x1; 
	c1y += y1;
	
	// Control point #2 (when p1 is translated to the origin):
	double c2x = ((h-dxc)*dx-dyc*dy)/h;
	double c2y = ((h-dxc)*dy+dyc*dx)/h;
	
	// Translate:
	c2x += x1; 
	c2y += y1;
	
	os << "<path class=\"curvedConnector\"";
	os << " d=\"M" << x1 << sp << y1 << sp;
	os << "C" <<  c1x << sp << c1y << sp << c2x << sp << c2y << sp << x2 << sp << y2 << sp;
	os << "\" />\n";
	
} 
//
// drawPregnancyLetterP
//
// Double-struck P to indicate a pregnancy
//
void SVG::drawPregnancyLetterP(std::ostringstream& os,double x,double y){  
	
	// move to x-y position:
	os << "<path d=\"M " << x << " " << y;
	// double-struck letter P instructions:
	os << " m 6.7367209,1049.0524 c -2e-5,1.3071 -0.59766,2.3137 -1.79294,3.0197 -0.99258,0.5801 -2.19136,0.8702 -3.59634,0.8702 -0.0979,0 -0.24815,0 -0.45086001,-0.01 -0.20271,-0.01 -0.353,-0.011 -0.45085,-0.011 -0.0979,0 -0.29708003,0 -0.59764003,0.011 l 0,4.3827 c -1e-5,0.5452 0.11533,0.8808 0.346,1.0066 0.13979,0.077 0.51376003,0.1153 1.12189004,0.1153 0.23765,0 0.35648,0.1258 0.35649,0.3775 -1e-5,0.2726 -0.11884,0.4089 -0.35649,0.4089 l -6.19662,0 c -0.23766,0 -0.35649,-0.1363 -0.35649,-0.4089 0,-0.2517 0.1398,-0.3775 0.4194,-0.3775 0.58716,0 0.94365,-0.056 1.06947,-0.1677 0.13281,-0.1189 0.19921,-0.4509 0.19921,-0.9961 l 0,-10.2229 c 0,-0.4962 -0.0629,-0.8073 -0.18873,-0.9331 -0.12582,-0.1258 -0.45435,-0.1887 -0.98558,-0.1888 -0.34251,10e-5 -0.51377,-0.1328 -0.51377,-0.3984 0,-0.2586 0.11883,-0.3879 0.35649,-0.3879 l 6.24905,0 c 1.49585,0 2.70861,0.2726 3.63828,0.8178 1.15334,0.6711 1.73001,1.7021 1.73003,3.0931 m -0.78638,0 c -10e-6,-1.5308 -0.87376,-2.5024 -2.62124,-2.9149 0.4124,0.5523 0.6186,1.5239 0.61861,2.9149 -1e-5,1.3001 -0.22019,2.2542 -0.66055,2.8624 1.77544,-0.4823 2.66317,-1.4365 2.66318,-2.8624 m -2.789,0 c -1e-5,-1.0555 -0.10486,-1.7895 -0.31455,-2.2019 -0.30757,-0.6151 -0.93667,-0.9226 -1.88730001,-0.9227 -0.27261,10e-5 -0.51377,0.063 -0.72346003,0.1888 -0.25863,0.1468 -0.38795,0.346 -0.38794,0.5976 l 0,5.4312 1.10092003,0 c 0.90869001,0 1.53080001,-0.318 1.86632001,-0.9541 0.23066,-0.4473 0.346,-1.1603 0.34601,-2.1389 m -3.85847001,-3.1246 -2.23329999,0 c 0.11183,0.2657 0.16775,0.6396 0.16776,1.1219 l 0,10.2229 c -1e-5,0.5033 -0.0524,0.8912 -0.15728,1.1638 l 2.18087999,0 c -0.13282,-0.2726 -0.19921999,-0.6466 -0.19921999,-1.1219 l 0,-10.6003 c 0,-0.2935 0.0804,-0.5557 0.24115999,-0.7864 \"/>";
	
}

//
// drawPregnancyFetalP
//
// Draw a "fetal" P to indicate pregnancy
//
void SVG::drawPregnancyFetalP(std::ostringstream& os,double x,double y){   
	
	// move to x-y position:
	os << "<path d=\"M " << x << " " << y;
	os << "m 0.7819386,1044.4769 c -7.30727,0.014 -5.94666,12.7312 -3.4687499,15.1562 0.55024,0.4125 0.37687,0.222 0.8125,0.5625 0.54065,0.6232 1.15171988,1.2073 1.4374999,0.9375 0.25964,-0.245 -0.20900003,-0.4194 -0.09375,-0.5937 0.13001003,-0.1967 0.49347003,0.2403 0.68750003,-0.1563 0.1667,-0.3407 -0.30237,-0.3891 -0.25,-0.5937 0.0483,-0.1888 0.4316,0.027 0.46875,-0.3438 0.0407,-0.4055 -1.11913005,-0.5175 -1.53124993,-0.4687 -0.24926,0.03 -0.45503,0.01 -0.625,-0.031 -0.28606,-0.1131 -0.29095,-0.2707 -0.1875,-0.6563 0.10641,-0.4007 0.19232,-0.8471 0.25,-1.3437 0.0132,-0.3365 0.16338,-0.4712 0.46875,-0.4063 0.14426,0.029 0.26021998,0.074 0.37499988,0.125 0.75286005,0.3375 1.57320005,0.5838 1.71875005,0.2188 0.13224,-0.3317 -0.37481,-0.2933 -0.34375,-0.5 0.0351,-0.2332 0.55359997,0 0.56249997,-0.4375 0.008,-0.3793 -0.46127997,-0.1986 -0.49999997,-0.4063 -0.0357,-0.1916 0.40357,-0.1797 0.28125,-0.5312 -0.13392,-0.3849 -1.2093,0 -1.56250005,0.2187 -0.13977,0.086 -0.31206988,0.1725 -0.46874988,0.2188 -0.30366,0.064 -0.45563,-0.071 -0.46875,-0.4063 -0.0101,-0.4101 -0.0196,-0.8146 -0.0625,-1.1875 -0.0153,-0.4756 0.23285,-0.5937 0.65625,-0.5 0.5638799,0.2438 1.19035993,0.375 1.84374993,0.375 2.56187997,0 4.62499997,-2.0631 4.62499997,-4.625 0,-2.5618 -2.06312,-4.625 -4.62499997,-4.625 z m 0.03125,2.1562 c 1.34243,0 2.4375,1.0951 2.4375,2.4375 0,1.3425 -1.09507,2.4063 -2.4375,2.4063 -1.34243003,0 -2.4374999,-1.0638 -2.4374999,-2.4063 0,-1.3424 1.09506987,-2.4375 2.4374999,-2.4375 z \"/>";
	
}

