
#include "DrawingMetrics.h"

int main(int argc, char *argv){
	
	DrawingColor white("white","#fff");
	std::cout << "White:" << white.get() << " complement: " << white.getComplement() << std::endl;
	std::cout << "Print label in black?: " << white.useBlackInk() << std::endl;
	
	DrawingColor gray("gray","#82b");
	std::cout << "Gray:" << gray.get()  << " complement: " << gray.getComplement()<< std::endl;
	std::cout << "Print label in black?: " << gray.useBlackInk() << std::endl;
	
	DrawingColor newColor("new","#123456");
	std::cout << "newColor: " << newColor.get()  << " complement: " << newColor.getComplement() << std::endl;
	std::cout << "Print label in black?: " << newColor.useBlackInk() << std::endl;
	
	DrawingColor new2("new2","0.071 0.20 0.34");
	std::cout << new2.getPostscript() << std::endl;
	std::cout << new2.get() << std::endl;
	std::cout << "Print label in black?: " << new2.useBlackInk() << std::endl;
	
	std::cout << "white=" << white <<"; gray=" << gray << "; newC=" << newColor << "; new2=" << new2 << std::endl;
	
	DrawingColor blue("blueSeries","#00f");
	for(int b=0; b<=256;b+=16){
		blue.set(0,0,b);
		std::cout << blue.getName() << "=" << blue.get() << " black ink?=" << blue.useBlackInk() << std::endl;
	}
	return 0;
	
}

