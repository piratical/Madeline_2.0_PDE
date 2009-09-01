/////////////////////////////////////////////////////////
//
// HexavigesimalConverter.cpp
// 
// Yes, I know! The name of this class 
// is totally ridiculous!
//
// (c) 2009 by Edward H. Trager
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
// 2009.08.31.ET
//
#include "HexavigesimalConverter.h"

//////////////////////////////////
//
// itoa
//
//////////////////////////////////
const char *HexavigesimalConverter::itoa(unsigned int n){
	
	// Make sure the buffer is null-terminated:
	_buffer[ _BUFSIZE-1 ]='\0';
	
	//
	// We can't display zero if this is a bijective enumeration:
	//
	if(_bijective && !n) return (const char *) _buffer+_BUFSIZE-1;
	
	//
	// Moving backward from one before the end of the
	// buffer, store the digit symbols.  In this way
	// we can get away with using a simple char[] buffer
	// without having to perform a 'reverse()' operation
	// afterwards. Also, we never have to clear the buffer
	// either ...
	//
	int i=_BUFSIZE-2;
	
	do{
		//
		// Use n-1 for bijective, else just n:
		//
		int r = (_bijective?n-1:n) % _base;
		
		_buffer[i] = r+'A';
		
		if(r) n-=r;
		
		n/=_base;
		
		--i;
		
	}while(n && i>=0);
	
	//
	// Increment to get to the "beginning" of the string:
	//
	i++;
	
	return (const char *) _buffer+i;
	
}

//
// setBijective()
//
void HexavigesimalConverter::setBijective(bool b){
	_bijective=b;
}

