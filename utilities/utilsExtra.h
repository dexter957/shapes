#include "../pathsHeader.h"

#define CIRCLE_CASE 1
#define SEMICIRCLE_CASE 2
#define RING_CASE  3
#define SQUARE_CASE 4
#define ELLIPSE_CASE 5


void makeArguments(int argc, int* offset, int* pointsToReadCount, char** inFile, char** outFile,char** howManyFile,float** utilsArgs, char** argv , 
	int* numOfArgs, int callerUtility, char** Point);

void utilityUse(int callerUtility,int argc, char** argv);
void cleanUp(char** inFile, char** outFile, char** howManyFile, float** utilsArgs, float** coordinates);
