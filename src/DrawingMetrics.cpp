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
// 2005.12.07.ET.
//

//
// DrawingMetrics.cpp
//
// -- Container for the default pedigree drawing metrics
// 

#include "DrawingMetrics.h"
#include <math.h>

//
// DEBUG ONLY:
//
#include <iostream>

///////////////////////////////////
//
// STATIC AND CONST STATIC MEMBERS:
//
///////////////////////////////////

std::string DrawingMetrics::_unit;
double DrawingMetrics::_horizontalInterval;
double DrawingMetrics::_iconInterval;
double DrawingMetrics::_iconDiameter;
double DrawingMetrics::_verticalDrop1;
double DrawingMetrics::_verticalDrop2;
double DrawingMetrics::_cellSpacing;
double DrawingMetrics::_lineWidth;
double DrawingMetrics::_lineHeight;
double DrawingMetrics::_labelMargin;
double DrawingMetrics::_scalingFactor;
double DrawingMetrics::_pointsToMmConversionFactor;
double DrawingMetrics::_yMaximum;
double DrawingMetrics::_yMinimum;     // Probably not needed.
double DrawingMetrics::_verticalTick; // Used for drawing twins.

bool DrawingMetrics::_isEmbedded; // State flag for embedded document creation
bool DrawingMetrics::_displayBirthOrder; // State flag that indicates DOB order has to be drawn with the labelset
std::string DrawingMetrics::_lineWidthUnit;
std::string DrawingMetrics::_fontFamily;
double DrawingMetrics::_fontSize;
double DrawingMetrics::_headerFontSize;
std::string DrawingMetrics::_fontSizeUnit;
std::set<DrawingMedia,DrawingMedia::compare> DrawingMetrics::media;
std::vector<DrawingColor> DrawingMetrics::monochromat;
std::vector<DrawingColor> DrawingMetrics::bichromat;
DrawingMetricsLoader DrawingMetricsLoader::drawingMetricsLoader;

std::string DrawingMetrics::_drawingFileNamePrefix;
bool DrawingMetrics::_hasOnlyOnePedigree;

bool DrawingMetrics::_blackAndWhite;
bool DrawingMetrics::_color;

///////////////////////////////////
//
// STATIC METHODS:
//
///////////////////////////////////

//
// getHorizontalInterval(): Returns scaled horizontal interval.
//                          
double DrawingMetrics::getHorizontalInterval(){
	
	return _horizontalInterval * _scalingFactor; 
	
}

//
// getIconInterval(): Returns scaled icon interval.
//
double DrawingMetrics::getIconInterval(){
	
	return _iconInterval * _scalingFactor; 
}

//
// getIconDiameter(): Returns scaled icon diameter.
//
double DrawingMetrics::getIconDiameter(){
	
	return DrawingMetrics::_iconDiameter * _scalingFactor;
	
}

//
// getIconRadius(): Returns scaled icon radius.
//
double DrawingMetrics::getIconRadius(){
	
	return 0.5*DrawingMetrics::_iconDiameter * _scalingFactor; 
	
}

//
// getVerticalDrop1(): Returns scaled vertical drop one.
//
double DrawingMetrics::getVerticalDrop1(){
	
	return DrawingMetrics::_verticalDrop1 * _scalingFactor; 
	
}

//
// getVerticalDrop2(): Returns scaled vertical drop two.
//
double DrawingMetrics::getVerticalDrop2(){
	
	return DrawingMetrics::_verticalDrop2 * _scalingFactor; 
	
}

//
// getLineWidth(): Returns line width -- not scaled.  Be sure
//                 to call getLineWidthUnit() to find out the units.
//                 This is designed intentionally to work directly with
//                 setting up CSS output.
//
double DrawingMetrics::getLineWidth(){
	
	return _lineWidth; 
	
}

//
// getLineWidthUnit()
//
std::string DrawingMetrics::getLineWidthUnit(){
	
	return _lineWidthUnit;
	
}

//
// setLineWidth(double lineWidth): It is suggested to use "mm" for
//                                 CSS and Postscript compatability.
//                                 "px" is also OK for CSS at least.
//
void DrawingMetrics::setLineWidth(double lineWidth){
	
	_lineWidth=lineWidth; 
	
}

//
// setLineWidthUnit(const std::string lineWidthUnit): It is suggested to use
//                                                    "mm" for CSS and Postscript
//                                                    compatability. "px" also OK for
//                                                    CSS at least.
//
void DrawingMetrics::setLineWidthUnit(const std::string lineWidthUnit){
	
	_lineWidthUnit=lineWidthUnit;
	
}

//
// getLineHeight(): Returns scaled line height.
//
double DrawingMetrics::getLineHeight(){
	
	return DrawingMetrics::_lineHeight * _scalingFactor; 
	
}

//
// getLabelMargin(): Returns scaled label margin.
//
double DrawingMetrics::getLabelMargin(){
	
	return DrawingMetrics::_labelMargin * _scalingFactor;
	
}

//
// getScalingFactor()
//
double DrawingMetrics::getScalingFactor(){
	return DrawingMetrics::_scalingFactor;
}

//
// getPointsToMmConversionFactor()
//
double DrawingMetrics::getPointsToMmConversionFactor(){
	return _pointsToMmConversionFactor;
}

//
// setIconInterval(double iconInterval): Uses "_unit" units.
//
void DrawingMetrics::setIconInterval(double iconInterval){ 
	_iconInterval = iconInterval; 
}

//
// getDrawingMedia(std::string name)
//
DrawingMedia DrawingMetrics::getDrawingMedia(std::string name){
	
	
	std::set<DrawingMedia,DrawingMedia::compare>::const_iterator dmit = media.find(DrawingMedia(name.c_str()));
	if(dmit != media.end()) return *dmit;
	
	// DrawingMedia with the given name was not found
	// The default a4 is returned
	
	std::cout << "Return Default DrawingMedia" << std::endl;
	dmit = media.find(DrawingMedia());
	return *dmit;
	
}


//
// getFontFamily()
//
std::string DrawingMetrics::getFontFamily(){
	
	return _fontFamily;
	
}

//
// getFontSize()
//
double DrawingMetrics::getFontSize(){
	
	return _fontSize;
	
}

//
// getHeaderFontSize()
//
double DrawingMetrics::getHeaderFontSize(){
	
	return _headerFontSize;
	
}

//
// getFontSizeUnit()
//
std::string DrawingMetrics::getFontSizeUnit(){
	
	return _fontSizeUnit;

}

//
// setFontFamily(const std::string &fontFamily)
//
void DrawingMetrics::setFontFamily(const std::string &fontFamily){
	
	_fontFamily = fontFamily;
	
}

//
// setFontSize(double fontSize)
//
void DrawingMetrics::setFontSize(double fontSize){
	
	_fontSize = fontSize;
	
}

//
// setHeaderFontSize(double headerFontSize)
//
void DrawingMetrics::setHeaderFontSize(double headerFontSize){
	
	_headerFontSize = headerFontSize;
	
}

//
// setFontSizeUnit: Only mm, px, and pts are understood:
//
// FIXME: ADD STRING VALIDATION CODE
//
void DrawingMetrics::setFontSizeUnit(const std::string &fontSizeUnit){
	
	_fontSizeUnit = fontSizeUnit;
	
}

//
// setLineMetrics(): Accepts parameters in "points" (1/72nds of an inch
//                   from LASi) and sets DrawingMetric's private members
//                   in millimeters:
//
void DrawingMetrics::setLineMetrics(double lineHeight,double yMaximum,double yMinimum, unsigned labelCount ){
	
	_lineHeight = lineHeight * _pointsToMmConversionFactor;
	_yMaximum   = yMaximum   * _pointsToMmConversionFactor;
	_yMinimum   = yMinimum   * _pointsToMmConversionFactor;
	
	//
	// Default is to print only Indv. Id, so this counts as 1 "label":
	//
	if(!labelCount) labelCount=1;
	
	// If DOB is present and is the default sort order birthOrder has to be displayed.
	// Add an additional count for birthOrder
	if(_displayBirthOrder) labelCount++;
	//
	// Here we always add one to the label count so that we have extra room for printing
	// tags such as "A", "B", "C", etc. directly under individuals:
	//
	_verticalDrop1 = 0.5*_iconDiameter +
	                 2.0*_labelMargin  + 
	                 _lineHeight * (labelCount+2);
	
}

//
// getYMaximum(): Returns scaled y maximum ( == maximum label *ascent* from text setting baseline ).
//
double DrawingMetrics::getYMaximum(){
	return _yMaximum * _scalingFactor;
}

//
// getYMinimum(): Returns scaled y minimum ( == the maximum label *descent* from the text setting baseline ).
//
double DrawingMetrics::getYMinimum(){
	return _yMinimum * _scalingFactor;
}

//
// getVerticalTick(): Returns scaled vertical tick interval.
//                    The vertical tick is used when drawing twins.
//
double DrawingMetrics::getVerticalTick(){
	
	return _verticalTick * _scalingFactor;
	
}

//
// setEmbeddedState()
//
void DrawingMetrics::setEmbeddedState(bool isEmbedded){
	
	_isEmbedded=isEmbedded;
	
}

//
// getEmbeddedState
//
bool DrawingMetrics::getEmbeddedState(){
	
	return _isEmbedded;
	
}

//
// setDrawingFileNamePrefix(const std::string &prefix)
//
void DrawingMetrics::setDrawingFileNamePrefix(const std::string &prefix){
	
	_drawingFileNamePrefix = prefix;
	
}

//
// getDrawingFileNamePrefix()
//
std::string DrawingMetrics::getDrawingFileNamePrefix(){
	
	return _drawingFileNamePrefix;
	
}

//
// setHasOnlyOnePedigreeState()
//
void DrawingMetrics::setHasOnlyOnePedigreeState(bool hasOnlyOne){
	
	_hasOnlyOnePedigree = hasOnlyOne;
	
}

//
// getHasOnlyOnePedigreeState
//
bool DrawingMetrics::getHasOnlyOnePedigreeState(){
	
	return _hasOnlyOnePedigree;
	
}

//
// setDisplayBirthOrder:
//
void DrawingMetrics::setDisplayBirthOrder(bool value){
	
	_displayBirthOrder = value;
	
}

//
// getDisplayBirthOrder:
//
bool DrawingMetrics::getDisplayBirthOrder(){
	
	return _displayBirthOrder;
	
}

//
// getColor(): Get the color override flag
//
bool DrawingMetrics::getColor(){
	return _color;
}

//
// getBlackAndWhite(): Get the black and white override flag
//
bool DrawingMetrics::getBlackAndWhite(){
	return _blackAndWhite;
}

//
// setColor(): Set the color override flag
//
void DrawingMetrics::setColor(bool value){
	_color=value;
}

//
// setBlackAndWhite(): Set the black and white override flag
//
void DrawingMetrics::setBlackAndWhite(bool value){ 
	_blackAndWhite=value;
}

////////////////////////////////
//
// DrawingMedia class
// DrawingMedia class
// DrawingMedia class
//
/////////////////////////////////

//
// Constructor:
//
DrawingMedia::DrawingMedia(const char *name,double width,double height,const char *unit){
	
	_name   = name;
	_width  = width;
	_height = height;
	_unit   = unit;
	
}

const std::string DrawingMedia::getName()   { return _name;   };
const std::string DrawingMedia::getUnit()   { return _unit;   };
double           DrawingMedia::getWidth() const { return _width;  };
double           DrawingMedia::getHeight() const { return _height; };


