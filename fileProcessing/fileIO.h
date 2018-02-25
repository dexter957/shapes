#define TRUE 1
#define FALSE 0


int readFromBinaryFile(char* fileName, int offset, int numberOfElements,float* buffer);
int writeToBinaryFile(char* fileName,float* buffer, int numberOfElements);
int getBinaryFileSize(char* fileName);
int createGNUplotScript(char* gnuScriptName, char** fileNames, int arrayLength, char** colors, char* commandSerial);
int createHandlerOutFile(char* fileName, float* buffer, int bufferSize);
