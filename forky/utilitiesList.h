
#define TRUE 1
#define FALSE 0

typedef struct utility *pointerToUtility;



int emptyList(pointerToUtility start);
int insertUtility(pointerToUtility* start,char* name, char** args, char* numOfArgs, char* color);
void deleteUtilities(pointerToUtility* start);
void printUtilitiesList(pointerToUtility start);
char* getUtilityName(pointerToUtility start, int nodeOrder);
char** getUtilityArgs(pointerToUtility start,int nodeOrder);
char* getUtilityNumberOfArgs(pointerToUtility start,int nodeOrder);
char* getUtilityColor(pointerToUtility start,int nodeOrder);
int howManyUtilities(pointerToUtility start);