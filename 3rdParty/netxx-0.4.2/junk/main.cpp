// Standard Includes
#include <iostream>
#include <exception>

#include "Network.h"

int main (int argc, char *argv[]) {
  if (argc != 2) {
      std::cout << "Usage: " << argv[0] << " URI [Host: header]\n";
       return 0;
  }


  std::string networkFile; 
  try{
	Network net;
	networkFile = net.getNetworkFile(argv[1]);
	std::cout << "The Server Response Header is : " << net.getServerResponseHeader() << std::endl;
	std::cout << "The FileSize is :" << net.getFileSize() << std::endl;
	// Display the entire file stream:
	//for(unsigned int i=0;i<net.getFileSize();i++) std::cout << networkFile[i];
	// Debugging: Display the first 20 bytes
	for(unsigned int i=0;i<20;i++) std::cout << networkFile[i];
    } catch (std::exception &e) {
	std::cerr << e.what() << std::endl;
	return 1;
    }

    return 0;
}


