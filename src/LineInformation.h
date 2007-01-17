
#include "string.h"
#include "ctype.h"

class LineInformation{
	
public:
	
	enum LINE_TYPE { EMPTY_LINE, DIRTY_LINE, HEADER_LINE, DATA_LINE, };
	
private:
	
	int _length;
	int _tabCount;
	// Note that this is not "const" so we can add null terminators
	// in places when we need to (using getMalleableLine() )
	char *_beginning;
	LINE_TYPE _type;
	
	void _determineLength( void ){ _length=strlen(_beginning); }
	void _determineTabCount( void ){ for(const char *b=_beginning;*b;b++) if(*b=='\t') _tabCount++; }
	void _determineIfDirty( void ) { for(const char *b=_beginning;*b;b++) if(!isspace(*b)) _type=DIRTY_LINE; }
	
public:
	
	// getters:
	int getLength( void ){ return _length; }
	int getTabCount( void ){ return _tabCount; }
	// This is the read-only version:
	const char *getLine( void ){ return (const char *) _beginning; }
	// Use this when mucking around with (changing) the line:
	char *getMalleableLine( void ){ return _beginning; }
	// This is actually one past the end, on the null terminator:
	const char *getLineEnd( void ){ return _beginning + _length; }
	LINE_TYPE getType( void ){ return _type; }
	// setters:
	void setBeginning( char *beginning ){ 
		_beginning=beginning; 
		_determineLength(); 
		_determineTabCount(); 
		_determineIfDirty();
	}
	void setType( LINE_TYPE type ){ _type=type; }
	
	// Constructor:
	LineInformation(){
		
		_length=0;
		_tabCount=0;
		_beginning=NULL;
		_type=EMPTY_LINE;
		
	}
	
};

