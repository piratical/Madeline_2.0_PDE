/////////////////////////////////////////////////////////
//
// This file is part of the MADELINE 2 program 
// written by Edward H. Trager, Ritu Khanna and Adrian Marrs
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
// 2006.11.16.ET.
//

//
// DrawingColor.cpp
//
#include "DrawingColor.h"
#include <math.h>
#include <iomanip>
#include <string.h>
#include <stdlib.h>
#include <iostream>

//
// Original table:
//
//int DrawingColor::_cutoff[36]={ 7, 7, 7, 6, 5, 5, 5, 5, 5, 6,
//                                6, 6, 6, 6, 6, 6, 6, 6, 7, 8, 
//                                9,10,10,10,10,10,10,10, 9, 8, 
//                                7, 7, 7, 7, 7, 7 };

//
// Revised table:
//
int DrawingColor::_cutoff[36]={ 7, 7, 7, 6, 5, 5, 5, 5, 6, 6,
                                6, 6, 6, 6, 6, 6, 6, 6, 7, 8, 
                                9,10,10,10,10,10,10,10, 9, 8, 
                                7, 7, 7, 7, 7, 7 };

double DrawingColor::_cutoffAdjustment=0.0;

//
// STATIC _namedColors lookup table:
// 
std::map<std::string,std::string> DrawingColor::_namedColors;
DrawingColorLoader DrawingColorLoader::drawingColorLoader;

/////////////////////////////////
//
// DrawingColor class
// DrawingColor class
// DrawingColor class
//
/////////////////////////////////

//
// Constructor taking RGB triplet:
//
DrawingColor::DrawingColor(const std::string &name,unsigned char red,unsigned char green,unsigned char blue){

	_name  = name;
	_red   = red;
	_green = green;
	_blue  = blue;
	_missing = false;
	
	_calculateHSV();
	
}

//
// set() using unsigned char integers:
//
void DrawingColor::set(unsigned char red,unsigned char green,unsigned char blue){
	
	_red   = red;
	_green = green;
	_blue  = blue;
	_missing = false;
	
	_calculateHSV();
}

//
// _hexCharacterToInt() Private method
//
unsigned char DrawingColor::_hexCharacterToInt(const char digit){
	
	switch(digit){
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		
		return digit-'0';
		break;
		
	case 'a':
	case 'A':
		
		return 10;
		break;
		
	case 'b':
	case 'B':
		
		return 11;
		break;
		
	case 'c':
	case 'C':
		
		return 12;
		break;
		
	case 'd':
	case 'D':
		
		return 13;
		break;
		
	case 'e':
	case 'E':
		
		return 14;
		break;
		
	case 'f':
	case 'F':
		
		return 15;
		break;
		
	}
	return 0;
	
}

//
// _intToHexString() Private method:
//
std::string DrawingColor::_intToHexString(unsigned char v) const{
	
	char hexDigit[16]={'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
	
	std::string s;
	
	s += hexDigit[((v&0xf0)>>4)];
	v <<= 4;
	s+= hexDigit[((v&0xf0)>>4)];
	
	return s;
	
}

//
// Constructors taking a HTML-style or Postscript-style color string:
//
DrawingColor::DrawingColor(const std::string &name,const std::string &color){
	
	_name = name;
	
	_setColorFromString(color);
	_calculateHSV();
	
}

//
// set()
//
void DrawingColor::set(const std::string &color){
	_setColorFromString(color);
	_calculateHSV();
}

//
// _setColorFromString private method:
//
void DrawingColor::_setColorFromString(const std::string &color){
	
	if(color=="" || color=="."){
		_missing=true;
		return;
	}
	
	if(color[0]=='#'){
		//
		// Process hex triplet:
		//
		
		int j=1;
		switch(color.length()){
		case 4:
			//
			// Short CSS-style colors like "#ff8":
			//
			unsigned char tmp;
			_red   = tmp = _hexCharacterToInt(color[j++]);
			_red <<= 4; 
			_red  |= tmp;
			_green = tmp = _hexCharacterToInt(color[j++]);
			_green <<= 4; 
			_green |= tmp;
			_blue  = tmp = _hexCharacterToInt(color[j]);
			_blue  <<= 4;
			_blue  |= tmp;
			_missing=false;
			break;
			
		case 7:
			//
			// Traditional hex colors like "#f8d4e7":
			//
			_red = _hexCharacterToInt(color[j++]);
			_red <<= 4; 
			_red |= _hexCharacterToInt(color[j++]);
			_green = _hexCharacterToInt(color[j++]);
			_green <<= 4; 
			_green |= _hexCharacterToInt(color[j++]);
			_blue = _hexCharacterToInt(color[j++]);
			_blue <<= 4;
			_blue |= _hexCharacterToInt(color[j]);
			_missing=false;
			break;
			
		default:
			_red = _green = _blue = 0;
			_missing=true;
			break;
		}
	}else if(color[0]=='0' || color[0]=='1'){
		//
		// Process postScript-style triplet:
		// e.g., in a form like "1.0 0.56 0.89"
		//
		char *col=strdup(color.c_str());
		char *g=NULL;
		char *b=NULL;
		
		char *r=col;
		for(char *c=col;*c;c++){
			if(*c==' ' || *c==','){
				*c++='\0';
				if(!g) g=c;
				else   b=c;
			}
		}
		
		_red   = static_cast<unsigned char>( 255.0*atof(r) );
		_green = static_cast<unsigned char>( 255.0*atof(g) );
		_blue  = static_cast<unsigned char>( 255.0*atof(b) );
		_missing=false;
		free(col);
	}else{
		//
		// see if it matches a named color:
		//
		const std::map<std::string,std::string>::iterator it=_namedColors.find(color);
		if( it==_namedColors.end() ){
			_red=_blue=_green=0;
			_missing=true;
		}else{
			//
			// Use _setColorFromString() recursively here::
			//
			_setColorFromString(it->second);
		}
	}
}

//
// _calculateHSV() Private method
//
void DrawingColor::_calculateHSV(){
	
	double min, max, delta,r,g,b;
	
	// Convert to [0,1] scale:
	r= _red/255.0;
	g= _green/255.0;
	b= _blue/255.0;
	
	// Get min & max:
	min = max = r; 
	if( g < min ) min = g;
	if( b < min ) min = b;
	if( g > max ) max = g;
	if( b > max ) max = b;
	
	// Calculate value, v:
	_v = max;
	
	// Calculate delta:
	delta = _v - min;
	
	// Calculate saturation, s:
	if(_v == 0.0) _s = 0.0;
	else          _s = delta/_v;
	
	// Calculate hue, h:
	if(_s==0.0){
		 _h = -1;
	}else{
		
		// Get here if not black or white:
		if     ( r == _v ) _h =     ( g - b ) / delta; // between yellow & magenta
		else if( g == _v ) _h = 2 + ( b - r ) / delta; // between cyan & yellow
		else               _h = 4 + ( r - g ) / delta; // between magenta & cyan
		
		// Convert hue to degrees:
		_h *= 60.0;
		if( _h < 0 ) _h += 360.0;
		
	}
	
}

//
// setFromHSV() h: [0,360] (degrees)
//              s: [0,1]
//              v: [0,1]
void DrawingColor::setFromHSV(double h, double s, double v){
	
	int i;
	double f, p, q, t;
	double r,g,b;
	
	//
	// Achromatic case:
	//
	if( s == 0.0 ) {
		_red = _green = _blue = static_cast<unsigned char>( v*255.0 );
		_calculateHSV();
		_missing=false;
		return;
	}
	
	//
	// Chromatic cases:
	//
	
	// Sector 0-5:
	h /= 60;
	i = static_cast<int>( floor(h) );
	// Factorial part of h:
	f = h - i;
	p = v * ( 1 - s );
	q = v * ( 1 - s * f );
	t = v * ( 1 - s * ( 1 - f ) );
	
	switch( i ) {
	case 0:
		r = v;
		g = t;
		b = p;
		break;
	case 1:
		r = q;
		g = v;
		b = p;
		break;
	case 2:
		r = p;
		g = v;
		b = t;
		break;
	case 3:
		r = p;
		g = q;
		b = v;
		break;
	case 4:
		r = t;
		g = p;
		b = v;
		break;
	default: // case 5:
		r = v;
		g = p;
		b = q;
		break;
	}
	_red   = static_cast<unsigned char>( r*255.0 );
	_green = static_cast<unsigned char>( g*255.0 );
	_blue  = static_cast<unsigned char>( b*255.0 );
	_calculateHSV();
	_missing=false;
}

//
// get(): 
//
std::string DrawingColor::get(void) const{
	
	if(_missing){
		return std::string(".");
	}
	
	std::string s;
	
	s+='#';
	s+=_intToHexString(_red  );
	s+=_intToHexString(_green);
	s+=_intToHexString(_blue );
	return s;
	
}

//
// getName()
//
std::string DrawingColor::getName(void) const{
	return _name;
}

//
// getComplement(): 
//
std::string DrawingColor::getComplement(void) const{
	
	if(_missing){
		return std::string(".");
	}
	
	std::string s;
	
	s+='#';
	s+=_intToHexString( 255 - _red   );
	s+=_intToHexString( 255 - _green );
	s+=_intToHexString( 255 - _blue  );
	return s;
	
}



///
/// useBlackInk(): Should a label printed
/// with this color as a background be printed
/// using black ink or white ink?
///
bool DrawingColor::useBlackInk(void) const{
	
	//
	// When the saturation level is really low,
	// don't even bother with the color lookup table.
	// Just use a fixed cut-off value of 0.5:
	//
	// DEBUG:
	//std::cout << "SATURATION=" << _s << std::endl;
	
	if(_s <= 0.3 ) return ( _v > 0.5 + _cutoffAdjustment );
	
	// _h == -1 when the color is black,
	// in which case one clearly wants to use
	// white ink on a black background:
	if(_h == -1) return false;
	
	// Turn hue angle into an integer
	// for lookup in the cutoff table:
	//
	int hueIndex = static_cast<int>( floor( _h/10.0 ) );
	double tableCutoffValue = _cutoff[hueIndex]/10.0;
	
	// DEBUG
	//std::cerr << "_h=" << _h << " index=" << hueIndex << " cutVal=" << cutoffValue << std::endl;
	
	// Correct for saturation:
	double saturationCorrectionSlope =  0.2857;
	double saturationIntercept       = -0.17;
	
	//double cutoffValue = tableCutoffValue + saturationIntercept + saturationCorrectionSlope*_s; 
	double cutoffValue = tableCutoffValue + _cutoffAdjustment + 
	                     saturationIntercept + saturationCorrectionSlope*_s;
	
	return (_v > cutoffValue );
	
}

//
// setCutoffAdjustment
//
void DrawingColor::setCutoffAdjustment(double value){
	// Limited to half of the range [0,1.0]:
	if(value < 0.5 || value > 0.5) return;
	_cutoffAdjustment = value;
}

//
// getCutoffAdjustment
//
double DrawingColor::getCutoffAdjustment(void){
	return _cutoffAdjustment;
}


//
// getPostscript()
//
std::string DrawingColor::getPostscript(void) const{
	
	if(_missing){
		return std::string(".");
	}
	
	std::ostringstream os;
	
	os << std::setprecision(2) << (_red  /255.0) << " ";
	os << std::setprecision(2) << (_green/255.0) << " ";
	os << std::setprecision(2) << (_blue /255.0) << " ";
	return os.str();
	
}

//
// New HSV constructor:  Note 0<=h<=360, 0<=s<=1, 0<=v<=1 :
//
DrawingColor::DrawingColor(const std::string &name,double h,double s, double v){
	
	_name=name;
	setFromHSV(h,s,v);
	
}

//
//! Free function: ostream operator <<
//
std::ostream& operator<<(std::ostream& s, const DrawingColor& color){
	
	return s << color.get();
	
}
