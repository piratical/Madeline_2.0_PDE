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
// 2005.11.18.ET.
//

//
// DrawingMetrics.h
//
// -- Container for the default pedigree drawing metrics
// 
#ifndef DrawingMetrics_INCLUDED
#define DrawingMetrics_INCLUDED

#include <string>
#include <set>
// For operator << :
#include <ostream>
// For debugging, at least:
#include <iostream>

#include <iomanip>
#include <sstream>

#include <string.h>
#include <stdlib.h>

#include "DrawingColor.h"
#include <vector>
#include <map>
#include "ColorSeries.h"

class DrawingMetricsLoader;
/////////////////////////
//
// DrawingMedia class
// 
/////////////////////////
class DrawingMedia{
public:
	//
	// Constructor.  Note that units default to millimeters.
	//               Recognized units are the same as for CSS: pt px mm cm in
	//               
	DrawingMedia(const char *name="a4",double width=210.0,double height=297.0,const char *unit="mm" );
	const std::string getName();
	const std::string getUnit();
	double            getWidth() const;
	double            getHeight() const;
	
	//
	// 
	//
	struct compare
	{
		bool operator()(const DrawingMedia & m1, const DrawingMedia & m2) const
		{
			return strcasecmp(m1._name.c_str(), m2._name.c_str() ) < 0;
		}
	};
	
private:
	
	friend class DrawingMetricsLoader;
	
	std::string _name;
	double _width;
	double _height;
	std::string _unit;
	
};


/////////////////////////
//
// DrawingMetrics class
// 
/////////////////////////
class DrawingMetrics
{
public:
	
private:
	
	friend class DrawingMetricsLoader;
	friend class DrawingLabel;
	
	//
	// Basic unit variable:
	//
	static std::string _unit;
	//
	// The following all use the basic unit variable, _unit:
	//
	static double _horizontalInterval;
	static double _iconInterval;
	static double _iconDiameter;
	static double _verticalDrop1;
	static double _verticalDrop2;
	static double _cellSpacing;   // Apparently not used => _labelMargin is used instead.
	static double _lineHeight;
	static double _yMaximum;
	static double _yMinimum;      // Is this needed / used ?
	static double _labelMargin;
	static double _verticalTick;
	
	//
	// Line width has its own unit variable:
	//
	static double _lineWidth;
	static std::string _lineWidthUnit;
	//
	// Font stuff:
	//
	static std::string _fontFamily;
	static double _fontSize;
	static double _headerFontSize;
	static double _counterFontSize;
	static std::string _fontSizeUnit;
	
	static double _scalingFactor;
	static double _pointsToMmConversionFactor;
	
	//
	// State flags which control output:
	//
	static bool _isEmbedded;
	static bool _displayBirthOrder;
	static bool _blackAndWhite;
	static bool _color;
	static bool _useQuadrantShadingMethod;
	static bool _noIconLabels;
	static bool _outputPedTable;
	static bool _outputDataTable;
	
	static bool _noLabelTruncation;
	//
	// Output File naming prefix
	// (If there is only one pedigree, this 
	// _is_ the file name.  Otherwise, it is
	// just the prefix):
	//
	static std::string _drawingFileNamePrefix;
	static std::string _drawingFileNameExtension;
	
	static bool _hasOnlyOnePedigree;
	static bool _useCustomIconColors;
	static bool _collapsible;
	
public:
	
	static std::set<DrawingMedia,DrawingMedia::compare> media;
	//
	// 2006.12.22.ET: Default colors used for icon shadings:
	//
	static std::vector<DrawingColor> monochromat;
	static std::vector<DrawingColor> bichromat;
	
	// 2015.09.18.ET ADDENDUM: custom color series:
	static std::vector<ColorSeries> customColorSeries;
	// 2015.10.12.ET ADDENDUM: Adding a map for more flexibility;
	static std::vector< std::map<std::string,DrawingColor> > customColorMap;
	
public:
	
	//
	// Constructors:
	//
	
	//
	// Static Methods:
	//
	// Getters:
	static double getHorizontalInterval();
	static double getIconInterval();
	static double getIconDiameter();
	static double getIconRadius();
	static double getVerticalDrop1();
	static double getVerticalDrop2();
	static double getVerticalTick();
	
	static double      getLineWidth();
	static std::string getLineWidthUnit();
	
	static double getLineHeight();
	static double getLabelMargin();
	static double getScalingFactor();
	static double getPointsToMmConversionFactor();
	static void setIconInterval(double iconInterval);
	static DrawingMedia getDrawingMedia(std::string name);
	
	static std::string getFontFamily();
	static double getFontSize();
	static double getHeaderFontSize();
	static double getCounterFontSize();
	static std::string getFontSizeUnit();
	
	static bool getEmbeddedState();
	static std::string getDrawingFileNamePrefix();
	static bool getHasOnlyOnePedigreeState();
	static bool getDisplayBirthOrder();
	
	static bool getColor();
	static bool getBlackAndWhite();
	static bool getQuadrantShading();
	static bool getNoIconLabels();
	static bool getHasOutputPedTable();
	static bool getHasOutputDataTable();
	
	static bool getUseCustomIconColors(){ return _useCustomIconColors;};
	static std::string &getDrawingFileNameExtension(void){ return _drawingFileNameExtension; };
	static bool getCollapsible(void){ return _collapsible; };
	//
	// Setters:
	//
	static void setLineWidth(double lineWidth);
	static void setLineWidthUnit(const std::string lineWidthUnit);
	
	static void setFontFamily(const std::string &fontFamily);
	static void setFontSize(double fontSize);
	static void setHeaderFontSize(double headerFontSize);
	static void setFontSizeUnit(const std::string &fontSizeUnit);
	
	static void setLineMetrics(double lineHeight,double yMaximum,double yMinimum, unsigned labelCount );
	
	static double getYMaximum();
	static double getYMinimum();
	
	static void setEmbeddedState(bool isEmbedded);
	static void setDrawingFileNamePrefix(const std::string &prefix);
	static void setHasOnlyOnePedigreeState(bool hasOnlyOne);
	static void setDisplayBirthOrder(bool value);
	
	static void setColor(bool value);
	static void setBlackAndWhite(bool value);
	static void setQuadrantShading(bool value);
	static void setNoIconLabels(bool value);
	static void setOutputPedTable(bool value);
	static void setOutputDataTable(bool value);
	
	static void setUseCustomIconColors(bool value){ _useCustomIconColors=value; };
	static void setDrawingFileNameExtension(const std::string &ext);
	static void setCollapsible(bool value){ _collapsible=value; };
	//
	// 2011.11.18.ET Addenda:
	//
	static bool getNoLabelTruncation();
	static void setNoLabelTruncation(bool value);
	
};

///////////////////////////////////
//
// DrawingMetricsLoader 
// 
// is the Friend class that 
// initializes the DrawingMetrics 
// class:
//
///////////////////////////////////
class DrawingMetricsLoader
{
public:
	static DrawingMetricsLoader drawingMetricsLoader;
	DrawingMetricsLoader(){
		
		//
		// Basic unit variable -- DON'T CHANGE : CURRENTLY MUST BE MM
		//
		// FIX ME : IN FUTURE, ALLOW ANY UNITS ?  MAYBE NO ? ...
		//
		DrawingMetrics::_unit = "mm";
		//
		// The following variables all share the "_unit" units and must be
		// scaled according to _scalingFactor depending on media output:
		// 
		//DrawingMetrics::_horizontalInterval = 7.5;
		DrawingMetrics::_horizontalInterval = 12;
		DrawingMetrics::_iconInterval = 2*DrawingMetrics::_horizontalInterval;
		DrawingMetrics::_iconDiameter = 6;
		DrawingMetrics::_verticalDrop1=30;
		DrawingMetrics::_verticalDrop2 = 7.5;
		DrawingMetrics::_cellSpacing = 1; // Hmm ... Not used? FIX ME: NOT USED? ONLY _labelMargin USED INSTEAD ?
		DrawingMetrics::_lineHeight = 3; // in mm
		DrawingMetrics::_labelMargin = 1; // one mm margination is good
		DrawingMetrics::_verticalTick=1; // one mm vertical tick drop (used for drawing twins) is good
		
		//
		// LineWidth has its own unit variable:
		//
		DrawingMetrics::_lineWidth = 0.5;
		DrawingMetrics::_lineWidthUnit = "mm";
		//
		// FontSize has its own unit variable ... :
		// 
		// NOTA BENE:  ...but only "mm","px", and "pt" are recognized:
		//
		// 2009.09.01.ET: Changed default font to DejaVu Sans
		// 
		DrawingMetrics::_fontFamily = "DejaVu Sans";
		DrawingMetrics::_fontSize        = 12;
		DrawingMetrics::_headerFontSize  = 18;
		DrawingMetrics::_counterFontSize = 14;
		DrawingMetrics::_fontSizeUnit = "px";
		
		//
		// State flags:
		//
		DrawingMetrics::_isEmbedded=false;
		DrawingMetrics::_displayBirthOrder=false;
		//
		// These two override the automatic settings:
		//
		DrawingMetrics::_blackAndWhite=false;
		DrawingMetrics::_color=false;
		//
		// 2010.09.28.ET addendum:
		// The quadrant shading method is off by default:
		//
		DrawingMetrics::_useQuadrantShadingMethod=false;
		//
		// Icon labels are on by default
		// because this seems the better choice
		// for lab work (but is not the choice
		// for publication):
		// 
		DrawingMetrics::_noIconLabels=false;
		
		//
		// By default labels are truncated to fit
		// within column widths:
		//
		DrawingMetrics::_noLabelTruncation=false;
		
		//
		// This is the default scaling factor used for SVG drawing output to a screen.
		// Assuming a default screen has 96 dots (pixels) per inch, we then have:
		// 
		//  96 dots    1 inch     1 cm
		//  ------- x -------- x ------- = 3.7795 dots per millimeter
		//  1 inch     2.54 cm    10 mm
		//  
		// Although DrawingMetrics makes it look like you could use
		// any units you want, in reality the measurement system is based on SI
		// millimeters
		// 
		DrawingMetrics::_scalingFactor = 3.78;
		//
		// Since LASi was designed for Postscript, LASi returns font metrics
		// in points, defined as 1/72nd of an inch. As Madeline uses SI units,
		// specifically millimeters, we have this conversion factor:
		//
		//    1                     2.54 cm      10 mm
		//  ---- inch / 1 point x ----------- x ------- = 0.35278 mm per point
		//   72                     1 inch        1 cm
		DrawingMetrics::_pointsToMmConversionFactor = 0.3527777778;
		
		//
		// ISO standard paper media sizes
		// ==============================
		// 
		// Markus Kuhn provides a nice overview of the ISO
		// paper sizes at
		// http://www.cl.cam.ac.uk/~mgk25/iso-paper.html 
		// 
		// Note that because the ratio of the sides is 1:sqrt(2),
		// cutting one size in half parallel to the shorter side yields
		// two pages of the next smaller ISO size.  This is not the
		// case with the American and Canadian size standards:
		// 
		DrawingMetrics::media.insert(DrawingMedia("4A0",1682,2378));
		DrawingMetrics::media.insert(DrawingMedia("2A0",1189,1682));
		DrawingMetrics::media.insert(DrawingMedia("A0",841,1189));
		DrawingMetrics::media.insert(DrawingMedia("A2",420,594));
		DrawingMetrics::media.insert(DrawingMedia("A3",297,420));
		DrawingMetrics::media.insert(DrawingMedia("A4",210,297));
		DrawingMetrics::media.insert(DrawingMedia("A5",148,210));
		DrawingMetrics::media.insert(DrawingMedia("A6",105,148));
		DrawingMetrics::media.insert(DrawingMedia("A7",74,105));
		DrawingMetrics::media.insert(DrawingMedia("A8",52,74));
		DrawingMetrics::media.insert(DrawingMedia("A9",37,52));
		DrawingMetrics::media.insert(DrawingMedia("A10",26,37));
		DrawingMetrics::media.insert(DrawingMedia("B0",1000,1414));
		DrawingMetrics::media.insert(DrawingMedia("B1",707,1000));
		DrawingMetrics::media.insert(DrawingMedia("B2",500,707));
		DrawingMetrics::media.insert(DrawingMedia("B3",353,500));
		DrawingMetrics::media.insert(DrawingMedia("B4",250,353));
		DrawingMetrics::media.insert(DrawingMedia("B5",176,250));
		DrawingMetrics::media.insert(DrawingMedia("B6",125,176));
		DrawingMetrics::media.insert(DrawingMedia("B7",88,125));
		DrawingMetrics::media.insert(DrawingMedia("B8",62,88));
		DrawingMetrics::media.insert(DrawingMedia("B9",44,62));
		DrawingMetrics::media.insert(DrawingMedia("B10",31,44));
		DrawingMetrics::media.insert(DrawingMedia("C0",917,1297));
		DrawingMetrics::media.insert(DrawingMedia("C1",648,917));
		DrawingMetrics::media.insert(DrawingMedia("C2",458,648));
		DrawingMetrics::media.insert(DrawingMedia("C3",324,458));
		DrawingMetrics::media.insert(DrawingMedia("C4",229,324));
		DrawingMetrics::media.insert(DrawingMedia("C5",162,229));
		DrawingMetrics::media.insert(DrawingMedia("C6",114,162));
		DrawingMetrics::media.insert(DrawingMedia("C7",81,114));
		DrawingMetrics::media.insert(DrawingMedia("C8",57,81));
		DrawingMetrics::media.insert(DrawingMedia("C9",40,57));
		DrawingMetrics::media.insert(DrawingMedia("C10",28,40));
		//
		// North American standard paper media sizes
		// =========================================
		// 
		DrawingMetrics::media.insert(DrawingMedia("letter",216,279));
		DrawingMetrics::media.insert(DrawingMedia("legal",216,356));
		DrawingMetrics::media.insert(DrawingMedia("executive",190,254));
		DrawingMetrics::media.insert(DrawingMedia("ledger",279,432));
		//
		// Computer screen resolutions for screen-based SVG output
		// =======================================================
		// 
		DrawingMetrics::media.insert(DrawingMedia("VGA",640,480,"px"));
		DrawingMetrics::media.insert(DrawingMedia("SVGA",800,600,"px"));
		DrawingMetrics::media.insert(DrawingMedia("XGA",1024,768,"px"));
		DrawingMetrics::media.insert(DrawingMedia("SXGA",1280,1024,"px"));
		DrawingMetrics::media.insert(DrawingMedia("UXGA",1600,1200,"px"));
		DrawingMetrics::media.insert(DrawingMedia("WXGA",1366,768,"px"));
		DrawingMetrics::media.insert(DrawingMedia("WSXGA",1680,1050,"px"));
		DrawingMetrics::media.insert(DrawingMedia("WUXGA",1920,1200,"px"));
		//DrawingMetrics::media.insert(DrawingMedia("",,));
		
		//
		// Monochromat and bichromatic color series default colors
		// =======================================================
		//
		// => For monochromatic series, just the "monochromat" colors are
		//    used to create ColorSeries.
		//
		// => For bichromatic series, both the "monochromat" and the 
		//    "bichromat" at a given matching index are used.
		//    
		// => The very first color series is used as the default when there is
		//    only one pie section.  Therefore, we force it to be plain black
		//    & white.
		//    
		// => The 2nd, 3rd, 4th, etc. color series are to be used when
		//    there is more than one pie section:
		//    
		//DrawingMetrics::monochromat.push_back(DrawingColor("black","#000")); // DEFAULT
		//DrawingMetrics::bichromat.push_back(  DrawingColor("white","#fff")); // DEFAULT
		
		DrawingMetrics::monochromat.push_back(DrawingColor("navy","#0000a0"));
		DrawingMetrics::bichromat.push_back(  DrawingColor("maize","#ffd803"));
		DrawingMetrics::monochromat.push_back(DrawingColor("red","#800"));
		DrawingMetrics::bichromat.push_back(  DrawingColor("banana","#ffffa2"));
		DrawingMetrics::monochromat.push_back(DrawingColor("forest","#003f00"));
		DrawingMetrics::bichromat.push_back(  DrawingColor("peach","#ffcca2"));
		DrawingMetrics::monochromat.push_back(DrawingColor("ceramicBlue","#768dff"));
		DrawingMetrics::bichromat.push_back(  DrawingColor("lavender","#fcf"));
		
		DrawingMetrics::_drawingFileNameExtension = ".xml";
		DrawingMetrics::_collapsible = false;
		
	}
};

#endif
