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

//////////////////////
//
// DrawingColor
//
//////////////////////
class DrawingColor {
public:
	
	// RGB constructor:
	DrawingColor(const std::string &name="white",unsigned char red=255,unsigned char green=255,unsigned char blue=255);
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
	// DEBUG only:
	//void printColor(void){ std::cout << "Color: " << std::hex << (int)_red << ":" << std::hex << (int)_green << ":" << std::hex << (int)_blue << std::endl; };
	
private:
	
	//friend class DrawingMetricsLoader;
	
	std::string   _name;
	// RGB model:
	unsigned char _red;   // Range: [0,255]
	unsigned char _green; // Range: [0,255]
	unsigned char _blue;  // Range: [0,255]
	
	//HSV model:
	double _h; // Range [0,360] degrees
	double _s; // Range [0,1]
	double _v; // Range [0,1]
	
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
};

std::ostream& operator<<(std::ostream& s, const DrawingColor& color);

#endif

