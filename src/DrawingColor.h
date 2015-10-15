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
// DrawingColor.h
//
#ifndef DrawingColor_INCLUDED
#define DrawingColor_INCLUDED

#include <string>
#include <sstream>
#include <strings.h>
#include <map>
#include <iostream>

//////////////////////
//
// DrawingColor
//
//////////////////////
class DrawingColorLoader;

class DrawingColor {
public:
	
	// RGB constructor:
	DrawingColor(const std::string &name="default",unsigned char red=255,unsigned char green=255,unsigned char blue=255);
	// Postscript or HTML-style hex triplet constructor:
	DrawingColor(const std::string &name,const std::string &postScriptOrHexTriplet);
	// HSV Constructor: Note 0<=h<=360, 0<=s<=1, 0<=v<=1 :
	DrawingColor(const std::string &name,double h,double s, double v);
	
	// Comparison functor:
	struct compare
	{
		bool operator()(const DrawingColor & c1, const DrawingColor & c2) const
		{
			return strcasecmp(c1._name.c_str(), c2._name.c_str() ) < 0;
		}
	};
	
	void set(unsigned char red, unsigned char green, unsigned char blue);
	void set(const std::string &color);
	void setFromHSV(double h, double s, double v);
	std::string get(void) const;
	std::string getName(void) const;
	std::string getComplement(void) const;
	std::string getPostscript(void) const;
	bool useBlackInk(void) const;
	
	static void setCutoffAdjustment(double value);
	static double getCutoffAdjustment(void);
	
	double getHue(void)        const {return _h;};
	double getSaturation(void) const {return _s;};
	double getValue(void)      const {return _v;};
	
	bool   isMissing(void)     const {return _missing; };
	
	// DEBUG only:
	//void printColor(void){ std::cout << "Color: " << std::hex << (int)_red << ":" << std::hex << (int)_green << ":" << std::hex << (int)_blue << std::endl; };
	
private:
	
	
	std::string   _name;
	// RGB model:
	unsigned char _red;   // Range: [0,255]
	unsigned char _green; // Range: [0,255]
	unsigned char _blue;  // Range: [0,255]
	
	//HSV model:
	double _h; // Range [0,360] degrees
	double _s; // Range [0,1]
	double _v; // Range [0,1]
	
	// MISSINGNESS: false by default
	bool _missing;
	
	//
	// static lookup table for determining
	// whether a label printed on a colored
	// background should be printed using 
	// black or white ink.  The 36 entries
	// represent 10-degree increment hue
	// "buckets":
	// 
	static int _cutoff[36];
	
	//
	// Because different screens and printers
	// will render / print colors differently,
	// manual adjustment of the _cutoff entries
	// may be necessary, either up or down:
	// 
	static double _cutoffAdjustment;
	
	void _setColorFromString(const std::string &color);
	void _calculateHSV(void);
	
	unsigned char _hexCharacterToInt(const char digit);
	std::string   _intToHexString(unsigned char v) const;
	
	// 2015.10.14.ET ADDENDUM: named color table lookup
	friend class DrawingColorLoader;
	static std::map<std::string,std::string> _namedColors;
};

std::ostream& operator<<(std::ostream& s, const DrawingColor& color);

//////////////////////////////////////////////
//
// CLASS DrawingColorLoader
//
// Used to initialize named colors
//
//////////////////////////////////////////////
class DrawingColorLoader
{
public:
	
	static DrawingColorLoader drawingColorLoader;
	
	DrawingColorLoader(){
		
		DrawingColor::_namedColors["aliceblue"]="#f0f8ff";
		DrawingColor::_namedColors["antiquewhite"]="#faebd7";
		DrawingColor::_namedColors["aqua"]="#00ffff";
		DrawingColor::_namedColors["aquamarine"]="#7fffd4";
		DrawingColor::_namedColors["azure"]="#f0ffff";
		DrawingColor::_namedColors["beige"]="#f5f5dc";
		DrawingColor::_namedColors["bisque"]="#ffe4c4";
		DrawingColor::_namedColors["black"]="#000000";
		DrawingColor::_namedColors["blanchedalmond"]="#ffebcd";
		DrawingColor::_namedColors["blue"]="#0000ff";
		DrawingColor::_namedColors["blueviolet"]="#8a2be2";
		DrawingColor::_namedColors["brown"]="#a52a2a";
		DrawingColor::_namedColors["burlywood"]="#deb887";
		DrawingColor::_namedColors["cadetblue"]="#5f9ea0";
		DrawingColor::_namedColors["chartreuse"]="#7fff00";
		DrawingColor::_namedColors["chocolate"]="#d2691e";
		DrawingColor::_namedColors["coral"]="#ff7f50";
		DrawingColor::_namedColors["cornflowerblue"]="#6495ed";
		DrawingColor::_namedColors["cornsilk"]="#fff8dc";
		DrawingColor::_namedColors["crimson"]="#dc143c";
		DrawingColor::_namedColors["cyan"]="#00ffff";
		DrawingColor::_namedColors["darkblue"]="#00008b";
		DrawingColor::_namedColors["darkcyan"]="#008b8b";
		DrawingColor::_namedColors["darkgoldenrod"]="#b8860b";
		DrawingColor::_namedColors["darkgray"]="#a9a9a9";
		DrawingColor::_namedColors["darkgreen"]="#006400";
		DrawingColor::_namedColors["darkkhaki"]="#bdb76b";
		DrawingColor::_namedColors["darkmagenta"]="#8b008b";
		DrawingColor::_namedColors["darkolivegreen"]="#556b2f";
		DrawingColor::_namedColors["darkorange"]="#ff8c00";
		DrawingColor::_namedColors["darkorchid"]="#9932cc";
		DrawingColor::_namedColors["darkred"]="#8b0000";
		DrawingColor::_namedColors["darksalmon"]="#e9967a";
		DrawingColor::_namedColors["darkseagreen"]="#8fbc8f";
		DrawingColor::_namedColors["darkslateblue"]="#483d8b";
		DrawingColor::_namedColors["darkslategray"]="#2f4f4f";
		DrawingColor::_namedColors["darkturquoise"]="#00ced1";
		DrawingColor::_namedColors["darkviolet"]="#9400d3";
		DrawingColor::_namedColors["deeppink"]="#ff1493";
		DrawingColor::_namedColors["deepskyblue"]="#00bfff";
		DrawingColor::_namedColors["dimgray"]="#696969";
		DrawingColor::_namedColors["dodgerblue"]="#1e90ff";
		DrawingColor::_namedColors["firebrick"]="#b22222";
		DrawingColor::_namedColors["floralwhite"]="#fffaf0";
		DrawingColor::_namedColors["forestgreen"]="#228b22";
		DrawingColor::_namedColors["fuchsia"]="#ff00ff";
		DrawingColor::_namedColors["gainsboro"]="#dcdcdc";
		DrawingColor::_namedColors["ghostwhite"]="#f8f8ff";
		DrawingColor::_namedColors["gold"]="#ffd700";
		DrawingColor::_namedColors["goldenrod"]="#daa520";
		DrawingColor::_namedColors["gray"]="#808080";
		DrawingColor::_namedColors["green"]="#008000";
		DrawingColor::_namedColors["greenyellow"]="#adff2f";
		DrawingColor::_namedColors["honeydew"]="#f0fff0";
		DrawingColor::_namedColors["hotpink"]="#ff69b4";
		DrawingColor::_namedColors["indianred"]="#cd5c5c";
		DrawingColor::_namedColors["indigo"]="#4b0082";
		DrawingColor::_namedColors["ivory"]="#fffff0";
		DrawingColor::_namedColors["khaki"]="#f0e68c";
		DrawingColor::_namedColors["lavender"]="#e6e6fa";
		DrawingColor::_namedColors["lavenderblush"]="#fff0f5";
		DrawingColor::_namedColors["lawngreen"]="#7cfc00";
		DrawingColor::_namedColors["lemonchiffon"]="#fffacd";
		DrawingColor::_namedColors["lightblue"]="#add8e6";
		DrawingColor::_namedColors["lightcoral"]="#f08080";
		DrawingColor::_namedColors["lightcyan"]="#e0ffff";
		DrawingColor::_namedColors["lightgoldenrodyellow"]="#fafad2";
		DrawingColor::_namedColors["lightgray"]="#d3d3d3";
		DrawingColor::_namedColors["lightgreen"]="#90ee90";
		DrawingColor::_namedColors["lightpink"]="#ffb6c1";
		DrawingColor::_namedColors["lightsalmon"]="#ffa07a";
		DrawingColor::_namedColors["lightseagreen"]="#20b2aa";
		DrawingColor::_namedColors["lightskyblue"]="#87cefa";
		DrawingColor::_namedColors["lightslategray"]="#778899";
		DrawingColor::_namedColors["lightsteelblue"]="#b0c4de";
		DrawingColor::_namedColors["lightyellow"]="#ffffe0";
		DrawingColor::_namedColors["lime"]="#00ff00";
		DrawingColor::_namedColors["limegreen"]="#32cd32";
		DrawingColor::_namedColors["linen"]="#faf0e6";
		DrawingColor::_namedColors["magenta"]="#ff00ff";
		DrawingColor::_namedColors["maroon"]="#800000";
		DrawingColor::_namedColors["mediumaquamarine"]="#66cdaa";
		DrawingColor::_namedColors["mediumblue"]="#0000cd";
		DrawingColor::_namedColors["mediumorchid"]="#ba55d3";
		DrawingColor::_namedColors["mediumpurple"]="#9370db";
		DrawingColor::_namedColors["mediumseagreen"]="#3cb371";
		DrawingColor::_namedColors["mediumslateblue"]="#7b68ee";
		DrawingColor::_namedColors["mediumspringgreen"]="#00fa9a";
		DrawingColor::_namedColors["mediumturquoise"]="#48d1cc";
		DrawingColor::_namedColors["mediumvioletred"]="#c71585";
		DrawingColor::_namedColors["midnightblue"]="#191970";
		DrawingColor::_namedColors["mintcream"]="#f5fffa";
		DrawingColor::_namedColors["mistyrose"]="#ffe4e1";
		DrawingColor::_namedColors["moccasin"]="#ffe4b5";
		DrawingColor::_namedColors["navajowhite"]="#ffdead";
		DrawingColor::_namedColors["navy"]="#000080";
		DrawingColor::_namedColors["oldlace"]="#fdf5e6";
		DrawingColor::_namedColors["olive"]="#808000";
		DrawingColor::_namedColors["olivedrab"]="#6b8e23";
		DrawingColor::_namedColors["orange"]="#ffa500";
		DrawingColor::_namedColors["orangered"]="#ff4500";
		DrawingColor::_namedColors["orchid"]="#da70d6";
		DrawingColor::_namedColors["palegoldenrod"]="#eee8aa";
		DrawingColor::_namedColors["palegreen"]="#98fb98";
		DrawingColor::_namedColors["paleturquoise"]="#afeeee";
		DrawingColor::_namedColors["palevioletred"]="#db7093";
		DrawingColor::_namedColors["papayawhip"]="#ffefd5";
		DrawingColor::_namedColors["peachpuff"]="#ffdab9";
		DrawingColor::_namedColors["peru"]="#cd853f";
		DrawingColor::_namedColors["pink"]="#ffc0cb";
		DrawingColor::_namedColors["plum"]="#dda0dd";
		DrawingColor::_namedColors["powderblue"]="#b0e0e6";
		DrawingColor::_namedColors["purple"]="#800080";
		DrawingColor::_namedColors["rebeccapurple"]="#663399";
		DrawingColor::_namedColors["red"]="#ff0000";
		DrawingColor::_namedColors["rosybrown"]="#bc8f8f";
		DrawingColor::_namedColors["royalblue"]="#4169e1";
		DrawingColor::_namedColors["saddlebrown"]="#8b4513";
		DrawingColor::_namedColors["salmon"]="#fa8072";
		DrawingColor::_namedColors["sandybrown"]="#f4a460";
		DrawingColor::_namedColors["seagreen"]="#2e8b57";
		DrawingColor::_namedColors["seashell"]="#fff5ee";
		DrawingColor::_namedColors["sienna"]="#a0522d";
		DrawingColor::_namedColors["silver"]="#c0c0c0";
		DrawingColor::_namedColors["skyblue"]="#87ceeb";
		DrawingColor::_namedColors["slateblue"]="#6a5acd";
		DrawingColor::_namedColors["slategray"]="#708090";
		DrawingColor::_namedColors["snow"]="#fffafa";
		DrawingColor::_namedColors["springgreen"]="#00ff7f";
		DrawingColor::_namedColors["steelblue"]="#4682b4";
		DrawingColor::_namedColors["tan"]="#d2b48c";
		DrawingColor::_namedColors["teal"]="#008080";
		DrawingColor::_namedColors["thistle"]="#d8bfd8";
		DrawingColor::_namedColors["tomato"]="#ff6347";
		DrawingColor::_namedColors["turquoise"]="#40e0d0";
		DrawingColor::_namedColors["violet"]="#ee82ee";
		DrawingColor::_namedColors["wheat"]="#f5deb3";
		DrawingColor::_namedColors["white"]="#ffffff";
		DrawingColor::_namedColors["whitesmoke"]="#f5f5f5";
		DrawingColor::_namedColors["yellow"]="#ffff00";
		DrawingColor::_namedColors["yellowgreen"]="#9acd32";
	}
	
};


#endif

