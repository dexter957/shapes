#include "utilitiesList.h"

#define ERROR		-1
#define EXIT		0
#define FINE		6
#define UTILITY1	1
#define UTILITY2	2
#define UTILITY3	3
#define UTILITY4	4
#define UTILITY5	5
#define BUFFER_MAX	200
#define FLOAT_LENGTH 50


int cli(pointerToUtility *start);
int readCommand(pointerToUtility *start,char* buffer);

