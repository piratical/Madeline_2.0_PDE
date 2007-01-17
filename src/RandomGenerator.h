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
// Class RandomGenerator
// 
// The random number generator presented here is based on
// the "ran2" routine presented on page 282 of Press, Flannery, Teukolsky
// and Vetterling's "Numerical Recipes in C" (c) 1992 by Cambridge
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
#include <fstream>
#include <iostream>

#define defined_EPS 3.0e-16
#define defined_RNMX (1.0 - defined_EPS )

#ifndef RANDOM_GENERATOR_INCLUDED

class RandomGenerator {
	
public:
	
	//
	// Used for entropy seed: 
	// TRUE_RANDOM grabs from /dev/random
	// PSEUDO_RANDOM grabs from /dev/urandom
	//
	enum ENTROPY_TYPE { TRUE_RANDOM, PSEUDO_RANDOM, UNSPECIFIED_RANDOM };
	
private:
	
	ENTROPY_TYPE _seedSource;
	
	const static int    IM1  = 2147483563;
	const static int    IM2  = 2147483399;
	const static int    IMM1 = IM1-1;
	const static int    IA1  = 40014;
	const static int    IA2  = 40692;
	const static int    IQ1  = 53668;
	const static int    IQ2  = 52774;
	const static int    IR1  = 12211;
	const static int    IR2  = 3791;
	const static int    NTAB = 32;
	const static int    NDIV = 1+IMM1/NTAB;
	
	//const static double EPS = defined_EPS;
	//const static double RNMX = defined_RNMX;
	//const static double AM   = 1.0/IM1;
	const static double EPS;
	const static double RNMX;
	const static double AM;
	
	int _idum;
	int _idum2;
	int iy;
	int iv[NTAB];
	int j;
	int k;
	double temp;
	
	//
	// Private method to gather entropy:
	//
	void _gatherEntropy();
	
	//
	// Private method to initialize the random number generator:
	//
	void _initialize(void);
	
public:
	
	//
	// Constructor:
	//
	// 2006.08.10.ET: Use "PSEUDO_RANDOM" by default:
	//               
	RandomGenerator( ENTROPY_TYPE seedSource = PSEUDO_RANDOM );
	
	//
	// setEntropySource() is used in the case that the constructor
	// for the object had seedSource=UNSPECIFIED_RANDOM:
	//
	void setEntropySource(ENTROPY_TYPE seedSource);
	
	//
	// reset(): Resets the random generator with a new seed 
	//
	void reset( void );
	
	//
	// getDeviate()
	//
	// Returns a number greater than or equal to 0.0
	// and less than 1.0 (*not* less than or equal):
	// 
	double getDeviate( void );
	
	//
	// getIntegerInRange
	//
	// Returns an integer in the range from low *inclusive*
	// to high *inclusive*:
	//
	int getIntegerInRange( int low, int high);
	
};

#define RANDOM_GENERATOR_INCLUDED
#endif
