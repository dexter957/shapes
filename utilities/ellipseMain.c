#include "utilsExtra.h"
#include <stdio.h>
#include <stdlib.h>


/*If pointsToReadCount is not specified, then one is supposed to read the entire file*/

int main(int argc,char* argv[])
{
	/*Now check if you have enough command line arguments*/
	if((argc-1)<9)
	{
		printf("Error; not enough arguments\n");
		exit(1);
	}
	else
	{
		utilityUse(ELLIPSE_CASE, argc,  argv);
	}
}