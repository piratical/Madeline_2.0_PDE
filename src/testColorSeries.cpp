//
// testColorSeries.cpp
//
// 2006.12.01.ET
//
// This program creates an SVG document used to visually test
// the ColorSeries class.  Both monochromatic and bichromatic
// color series are drawn as arc segments of circles.
//
// View the resulting SVG in Firefox, Opera, Inkscape, or another SVG
// viewing application.
//

#include <iostream>
#include <sstream>
#include <string>
#include <math.h>
#include "DrawingColor.h"
#include "ColorSeries.h"
#
using namespace std;

int main(int argc, char *argv[]);
std::string arc(double x, double y, double r, double startAngle, double endAngle,std::string color);
std::string curvedSegment( double x, double y, double r1,double r2, double startAngle, double endAngle,std::string color,std::string labelColor);

std::string pie( double x, double y, double r, int sections,const ColorSeries &colorSeries,bool reversed=false );

double PI=M_PI;

int main(int argc, char *argv[]){
	
	std::string svgStart="<?xml version=\"1.0\" standalone=\"no\"?>\n<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n<svg width=\"1500\" height=\"2000\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\">\n";
	std::string svgEnd  ="</svg>\n";
	
	cout << svgStart;
	
	DrawingColor black("black","#000");
	DrawingColor cyan("cyan","#0dd");
	DrawingColor orange("orange","#ffa800");
	DrawingColor red("red","#800");
	DrawingColor forest("forest","#003f00");
	DrawingColor blue("blue","#008");
	DrawingColor yellow("yellow","#eaed00");
	DrawingColor magenta("magenta","#fcf");
	DrawingColor deepOrange("deepOrange","#c48100");
	DrawingColor brown("brown","#524328");
	
	DrawingColor maize("maize","#ffd803");
	DrawingColor navy("navy","#060083");
	
	DrawingColor yellowOrange("yellowOrange","#ffc600");
	DrawingColor banana("banana","#ffffa2");
	DrawingColor redOrange("redOrange","#ff6900");
	DrawingColor peach("peach","#ffcca2");
	DrawingColor skyBlue("skyBlue","#768dff");
	DrawingColor white("white","#fff");
	
	int xc = 70;
	int yc = 70;
	int rc = 50;
	
	// Black monochromatic:
	for(int i=2;i<10;i++){
		
		cout << pie(xc,yc,rc,i,ColorSeries(i,black),true);
		xc+=140;
		if(xc>560){
			xc=70;
			yc+=140;
		}
	}
	
	// Red reversed:
	yc+=70;
	for(int i=2;i<10;i++){
		
		cout << pie(xc,yc,rc,i,ColorSeries(i,red),true);
		xc+=140;
		if(xc>560){
			xc=70;
			yc+=140;
		}
	}
	
	// Green monochromatic:
	yc+=70;
	for(int i=2;i<10;i++){
		
		cout << pie(xc,yc,rc,i,ColorSeries(i,forest),true);
		xc+=140;
		if(xc>560){
			xc=70;
			yc+=140;
		}
	}
	
	// Blue monochromatic:
	yc+=70;
	for(int i=2;i<10;i++){
		
		cout << pie(xc,yc,rc,i,ColorSeries(i,navy),true);
		xc+=140;
		if(xc>560){
			xc=70;
			yc+=140;
		}
	}
	
	// Sky Blue monochromatic:
	yc+=70;
	for(int i=2;i<10;i++){
		
		cout << pie(xc,yc,rc,i,ColorSeries(i,skyBlue),true);
		xc+=140;
		if(xc>560){
			xc=70;
			yc+=140;
		}
	}
	
	xc=650;
	yc=70;
	
	//////////////////////
	//
	// Bichromatic tests:
	//
	//////////////////////
	
	// Orange to black:
	for(int i=2;i<10;i++){
		
		cout << pie(xc,yc,rc,i,ColorSeries(i,black,yellowOrange),true);
		xc+=140;
		if(xc>1120){
			xc=650;
			yc+=140;
		}
	}
	
	// Red to banana:
	yc+=70;
	for(int i=2;i<10;i++){
		
		cout << pie(xc,yc,rc,i,ColorSeries(i,red,banana),true);
		xc+=140;
		if(xc>1120){
			xc=650;
			yc+=140;
		}
	}
	
	// blue to magenta:
	yc+=70;
	for(int i=2;i<10;i++){
		
		cout << pie(xc,yc,rc,i,ColorSeries(i,skyBlue,magenta),true);
		xc+=140;
		if(xc>1120){
			xc=650;
			yc+=140;
		}
	}
	
	// Yellow to black:
	yc+=70;
	for(int i=2;i<10;i++){
		
		cout << pie(xc,yc,rc,i,ColorSeries(i,navy,maize),true);
		xc+=140;
		if(xc>1120){
			xc=650;
			yc+=140;
		}
	}
	
	// Forest to peach:
	yc+=70;
	for(int i=2;i<10;i++){
		
		cout << pie(xc,yc,rc,i,ColorSeries(i,forest,peach),true);
		xc+=140;
		if(xc>1120){
			xc=650;
			yc+=140;
		}
	}
	
	// Custom color series test:
	xc=1300;
	yc=70;
	
	ColorSeries custom_001("#f00","#0f0","#00f","#000");
	cout << pie(xc,yc,rc,custom_001.levels(),custom_001,true);
	
	yc+=70;
	ColorSeries custom_002("#f00","#0f0","#00f","#000",
	                   "#f00","#0f0","#00f","#000",
	                   "#f00","#0f0","#00f","#000",
	                   "#f00","#0f0","#00f","#000");
	cout << pie(xc,yc,rc,custom_002.levels(),custom_002,true);
	cout << svgEnd;
	
	return 0;
}

//
// arc
//
std::string arc( double x, double y, double r, double startAngle, double endAngle,std::string color){
	
	std::ostringstream arc;
	double x1,y1,x2,y2;
	
	x1=x2=x;
	y1=y2=y;
	
	x1 -= cos(startAngle)*r;
	y1 -= sin(startAngle)*r;
	x2 -= cos(endAngle  )*r;
	y2 -= sin(endAngle  )*r;
	
	// Move to center point:
	arc << "<path d=\"M " << x << " " << y << " ";
	
	// Line from center to edge of circle:
	arc << "L " << x1 << " " << y1 << " ";
	// Arc to second angle:
	arc << "A " << r << " " << r << " 0 0 1 " << x2 << " " << y2 << " ";
	// Line back to center:
	//arc << "Z\" style=\"fill:" << color << ";stroke:black;stroke-width:1.0mm;\"/>\n";
	arc << "Z\" style=\"fill:" << color << ";stroke:none;\"/>\n";
	
	return arc.str();
	
}


//
// pie
//
std::string pie( double x, double y, double r, int sections,const ColorSeries &colorSeries, bool reversed ){
	
	std::ostringstream pie;
	double arcAngle = 2.0*PI/sections;
	double startAngle = 0.5*PI - arcAngle;
	double endAngle   = startAngle+arcAngle;
	
	for(int i=0;i<sections;i++){
		
		pie << arc(x,y,r,startAngle,endAngle,(reversed?colorSeries.reversedSeriesGetColorAtLevel(i):colorSeries.getColorAtLevel(i)));
		startAngle+=arcAngle;
		endAngle+=arcAngle;
		
	}
	return pie.str();
}




//
// curvedSegment
//
std::string curvedSegment( double x, double y, double r1,double r2, double startAngle, double endAngle,std::string color,std::string labelColor){
	
	std::ostringstream arcc;
	double x1,x2,x3,x4,y1,y2,y3,y4;
	
	x1=x2=x3=x4=x;
	y1=y2=y3=y4=y;
	
	x1 -= cos(startAngle)*r2;
	y1 -= sin(startAngle)*r2;
	x2 -= cos(endAngle  )*r2;
	y2 -= sin(endAngle  )*r2;
	
	x4 -= cos(startAngle)*r1;
	y4 -= sin(startAngle)*r1;
	x3 -= cos(endAngle  )*r1;
	y3 -= sin(endAngle  )*r1;
	
	// COLOR TESTING:
	double xc,yc;
	xc = 0.5*(x2+x4);
	yc = 0.5*(y2+y4);
	std::string black="black";
	std::string white="white";
	
	
	// Move to starting point:
	arcc << "<path d=\"M " << x4 << " " << y4 << " ";
	
	// Line from starting point on edge of inner
	// circle out to edge of outer circle:
	arcc << "L " << x1 << " " << y1 << " ";
	
	// Arc to second angle:
	arcc << "A " << r2 << " " << r2 << " 0 0 1 " << x2 << " " << y2 << " ";
	//arc << "L " << x2 << " " << y2 << " ";
	
	// Line back to inner circle:
	arcc << "L " << x3 << " " << y3 << " ";
	
	// Arc back to starting point:
	arcc << "A " << r1 << " " << r1 << " 0 0 0 " << x4 << " " << y4 << " ";
	//arc << "L " << x4 << " " << y4 << " ";
	
	// Finish up:
	arcc << "Z\" style=\"fill:" << color << ";stroke:black;stroke-width:1px;\"/>\n";
	
	// COLOR TESTING:
	//arc << pie(xc,yc,5,2);
	
	//arcc << arc(xc,yc,5,-90/180.0*PI,90/180.0*PI, labelColor );
	arcc << "<text x=\"" << (xc-5) << "\" y=\"" << (yc+5) << "\" style=\"font-family:Verdana;font-size:14;font-weight:bold;\" fill=\"" << labelColor << "\" >A</text>";
	return arcc.str();
	
}


