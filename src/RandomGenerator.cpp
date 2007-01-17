/////////////////////////////////////////////////////////
//
// This file is part of the SHUFFLE program 
// written by Edward H. Trager and
// Copyright (c) 2005 by the
// Regents of the University of Michigan.
// All Rights Reserved.
// 
// The latest version of this program is available from:
// 
//   http://eyegene.ophthy.med.umich.edu/shuffle/
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
// RandomGenerator.cpp
// 
// The random number generator presented here is based on
// the "ran2" routine presented on page 282 of Press, Flannery, Teukolsky
// and Verrerling's "Numerical Recipes in C" (c) 1992 by Cambridge
// University Press (ISBN 0-521-43108-5).
//
// The routine implements the long-period (>2x10^18) random number generator
// of L'Ecuyer with Bays-Durham shuffle and added safeguards.  Returns a 
// random uniform deviate between 0.0 and 1.0 (exclusive of the endpoint values).
// RNMX should approximate the largest floating value that is less than 1.
//
// I have added the feature of gathering entropy from either
// /dev/random or /dev/urandom in order to get a starting seed.
//

#include "RandomGenerator.h"

const double RandomGenerator::EPS = defined_EPS;
const double RandomGenerator::RNMX = defined_RNMX;
const double RandomGenerator::AM   = 1.0/RandomGenerator::IM1;

//
// Private method to gather entropy:
//
void RandomGenerator::_gatherEntropy( void ){
	
	std::ifstream entropySource;
	char *device;
	int temp;
	
	if     ( _seedSource == TRUE_RANDOM   ) device = "/dev/random";
	else if( _seedSource == PSEUDO_RANDOM ) device = "/dev/urandom";
	else {
		
		std::cerr << "RandomGenerator::_gatherEntropy(): The entropy source has not been specified\n";
		return;
	}
	
	// Prompt the user for keystroke interval-based entropy:
	if( _seedSource == TRUE_RANDOM ) std::cerr << "Please enter some random keystrokes to gather entropy ...\n";
	
	entropySource.open( device , std::ifstream::in );
	entropySource.read((char *)&temp,sizeof(int));
	entropySource.close();
	if( _seedSource == TRUE_RANDOM ) std::cerr << "Thank you! That's enough.\n";
	
	// Seed needs to be negative (based on Numerical Recipes):
	if(temp>0) temp *= -1;
	
	// Set the seed value:
	_idum = (int) temp;
	
	// DEBUG:
	//std::cerr << "The SEED is: " << _idum << std::endl;
	
}

//
// Private method to initialize the random number generator:
//
void RandomGenerator::_initialize( void ){
	
	//
	// This part is directly from Numerical Recipes:
	//
	_idum  = ( _idum==0 ? 1 : - _idum );    // Be sure to prevent idum==0;
	_idum2 = _idum;                         // The initial assn. of _idum2 in NR makes no sense ...
	for (j=NTAB+7;j>=0;j--) {               // Load the shuffle table
		k = _idum/IQ1;                  // (after 8 warm-ups)
		_idum=IA1*(_idum-k*IQ1)-k*IR1;
		if ( _idum < 0 ) _idum += IM1;
		if ( j < NTAB  ) iv[j] = _idum;
	}
	iy = iv[0];
	
}

//
// Public Constructor:
//
RandomGenerator::RandomGenerator( ENTROPY_TYPE seedSource ){
	
	// Set the seed source:
	_seedSource = seedSource;
	
	//
	// Only gather entropy now if the SeedSource has been specified:
	//
	if( _seedSource != UNSPECIFIED_RANDOM ){
		
		// Gather entropy:
		_gatherEntropy();
		
		// initialize the random number generators:
		_initialize();
		
		// Initial values from Numerical Recipes:
		// I don't understand why the assignment of idum2 ?
		_idum2=123456789;
		iy=0;
		for(int i=0;i<NTAB;i++) iv[i]=0;
		
	}
	
}

//
// setEntropySource() is used in the case that the constructor
// for the object had seedSource=UNSPECIFIED_RANDOM:
//
void RandomGenerator::setEntropySource(ENTROPY_TYPE seedSource){
	
	if( seedSource == UNSPECIFIED_RANDOM ){
		
		std::cerr << "RandomGenerator::setEntropySource(): Specify seedSource as TRUE_RANDOM or PSEUDO_RANDOM\n";
		return; 
		
	}
	
	// set the seed source:
	_seedSource = seedSource;
	
	// Gather entropy:
	_gatherEntropy();
	
	// initialize the random number generators:
	_initialize();
	
}

//
// reset(): Resets the random generator with a new seed 
//
void RandomGenerator::reset( void ){
	
	_gatherEntropy();
	_initialize();
	
}

//
// getDeviate()
//
// Returns a number greater than or equal to 0.0
// and less than 1.0 (*not* less than or equal):
// 
double RandomGenerator::getDeviate( void ){
	
	k = _idum/IQ1;                              // Start here when not initializing
	_idum  = IA1*( _idum  - k*IQ1 ) - k*IR1;    // Compute idum=(IA1*idum)%M1 without
	if ( _idum < 0 ) _idum += IM1;              // overflow by Schrage's method.
	k = _idum2/IQ2;
	_idum2 = IA2*( _idum2 - k*IQ2 ) - k*IR2;    // Compute idum2 likewise.
	if ( _idum2 < 0 ) _idum2 += IM2;    
	j     = iy/NDIV;                            // Will be in the range 0...NTAB-1
	iy    = iv[j] - _idum2;                     // Here idum is shuffled and idum and idum2 are
	iv[j] = _idum;                              // combined to generate output.
	if (iy < 1) iy += IMM1;
	if ((temp=AM*iy) > RNMX ) return RNMX;      // Because users don't expect endpoint values.
	else return temp;
	
}

//
// getIntegerInRange
//
// Returns an integer in the range from low *inclusive*
// to high *inclusive*.  
int RandomGenerator::getIntegerInRange( int low, int high){
	
	//
	// Because the cast to "int" always drops the decimal
	// portion, we need to add 1 to "high-low" in order to
	// obtain the inlusion of the "high" value.  (Remember
	// that getDeviate() returns a number greater than or
	// equal to zero and less than 1.0).
	//
	return low + (int) ((high-low+1)*getDeviate());
	
}

