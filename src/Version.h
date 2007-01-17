#include <iostream>
class Version{
private:
	std::string _version;
	std::string _name;
	
	
public:
	Version(){ _name=""; _version="0.0"; }
	Version(std::string name,std::string version){
		_name = name;
		_version = version;
	}
	void printVersion(){
		std::cout << _name << " : " << _version << std::endl;
	}
	
};
