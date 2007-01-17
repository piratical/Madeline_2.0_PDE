#include "ColumnClassifier.h"
#include <string>

class ColumnOffset{
	
private:
	
	int _start;
	int _end;
	int _width;
	
	const char * _name;
	// Madeline column types, i.e. "C", "N", "G", "X", "A" ... :
	DATATYPE _discoveredType;
	char _declaredType;
	
public:
	
	// constructor:
	ColumnOffset(){ 
		
		_start=0;
		_end=0;
		_width=0;
		_name='\0';
		// The discovered type becomes the official type.
		// In some cases, the _discoveredType will be set
		// according to _declaredType (see setTypeFromDeclaration() ):
		_discoveredType=UNCLASSIFIED;
		_declaredType='\0'; // '\0' indicates the default 'not declared' state
		
	}
	
	// setters:
	void setStart(int start){ _start=start; }
	void setEnd(int end ){ _end=end; if(_end>_start) _width=_end-_start; }
	
	// setName():
	void setName( const char *stt){
		_name = stt;
	}
	
	void setDeclaredType( char type ){ _declaredType=type; }
	void setDiscoveredType(DATATYPE type ){ _discoveredType=type; }
	
	//
	// setTypeFromDeclaration:
	//
	void setTypeFromDeclaration( void ){
		
		if(_declaredType=='\0') _discoveredType=MISSING;
		if(_declaredType=='N' ) _discoveredType=NUMBER;
		if(_declaredType=='D' ) _discoveredType=DATE;
		if(_declaredType=='X' ) _discoveredType=GENDER;
		if(_declaredType=='G' ) _discoveredType=GENOTYPE;
		if(_declaredType=='A' ) _discoveredType=ALLELE;
		if(_declaredType=='C' ) _discoveredType=STRING;
		
	}
	
	//
	// setDeclaredTypeFromDiscovery
	//
	void setDeclaredTypeFromDiscovery( void ){
		
		if(_discoveredType==MISSING ) _declaredType='\0';
		if(_discoveredType==NUMBER  ) _declaredType='N';
		if(_discoveredType==DATE    ) _declaredType='D';
		if(_discoveredType==GENDER  ) _declaredType='X';
		if(_discoveredType==GENOTYPE) _declaredType='G';
		if(_discoveredType==ALLELE  ) _declaredType='A';
		if(_discoveredType==STRING  ) _declaredType='S';
	}
	
	// getters:
	int getStart( void ){ return _start; }
	int getEnd( void ){ return _end; }
	int getWidth( void ){ return _width; }
	const char *getName( void ){ return _name; }
	DATATYPE getDiscoveredType( void ){ return _discoveredType; }
	char getDeclaredType( void ){ return _declaredType; }
	
};

