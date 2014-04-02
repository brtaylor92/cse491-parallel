#include <iostream>

using std::cerr;
using std::endl;

#include "track.h"


int main(int argc, char const *argv[])
{
	if(argc != 5) {
		cerr << "Format: " << argv[0] << " [gridSize] [numTrains] [trainSpeed] [numSteps]"  << endl;
	}
	(void) argc;
	(void) argv;
	/* code */
	return 0;
}