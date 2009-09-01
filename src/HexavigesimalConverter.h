/////////////////////////////////////////////////////////
//
// HexavigesimalConverter.h
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
#ifndef HEXAVIGESIMAL_CONVERTER
#define HEXAVIGESIMAL_CONVERTER

//
// HexavigesimalConverter
//
class HexavigesimalConverter{
	
private:
	
	//
	// 26 letters in the alphabet
	// gives the base for the numeral system:
	//
	static const int _base=26;
	
	//
	// With the buffer size set to 8,
	// we can display up to 26^7 =
	// 8,031,810,176 -- that should be
	// big enough:
	// 
	static const int _BUFSIZE=8;
	char _buffer[ _BUFSIZE ];
	
	//
	// Bijective switch:
	//
	bool _bijective;
	
public:
	//
	// Constructor:
	// 
	// We set bijective to TRUE because this is the
	// expected output for enumerations of this sort:
	// 
	// =========== BASE 26 ENUMERATIONS ==========
	// ========= USING THE LATIN ALPHABET ========
	// 
	// INTEGER      NON      TYPICAL
	//  VALUE    BIJECTIVE  BIJECTIVE
	// ========  =========  =========
	//    0          A          .
	//    1          B          A
	//    2          C          B
	//   ...         ...        ...
	//   24          Y          X
	//   25          Z          Y
	//   26          BA         Z
	//   27          BB         AA
	//   28          BC         AB
	//   ...         ...        ...
	// 
	HexavigesimalConverter(){ _bijective=true; };
	
	//
	// This converts an integer to the Hexavigesimal
	// display form:
	// 
	const char *itoa(unsigned int n);
	
	//
	// Pass FALSE to get true hexavigesimal output.
	// (The default is TRUE which gives bijective
	// output):
	// 
	void setBijective(bool b);
	
};

#endif
