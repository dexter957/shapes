#include "utilsExtra.h"
#include READINGHEADER
#include "utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>


void makeArguments(int argc, int* offset, int* pointsToReadCount, char** inFile, char** outFile,char **howManyFile,float** utilsArgs, 
	char** argv, int* numOfArgs, int callerUtility , char** Point)
{
	switch(callerUtility)
	{
		case CIRCLE_CASE: (*numOfArgs)=3;
		break;
		case SEMICIRCLE_CASE: (*numOfArgs)=3;
		break;
		case RING_CASE: (*numOfArgs)=4;
		break;
		case SQUARE_CASE: (*numOfArgs)=3;
		break;
		case ELLIPSE_CASE: (*numOfArgs)=4;
		break;
		default: ;
		break;
	}
	char* inFileFlag="-i";
	char* outFileFlag="-o";
	char* utilArgsFlag="-a";
	char* offsetFlag="-f";
	char* pointsFlag="-n";
	char* howManyFlag="-h";
	int i;
	for(i=1;i<argc;++i)
	{
		if(strcmp(argv[i],inFileFlag)==0)
		{/*Get input file*/
			(*inFile)=malloc((strlen(argv[i+1])+1)*sizeof(char));
			strcpy((*inFile),argv[i+1]);
		}
		if(strcmp(argv[i],outFileFlag)==0)
		{/*Get output file*/
			(*outFile)=malloc((strlen(argv[i+1])+1)*sizeof(char));
			strcpy((*outFile),argv[i+1]);
		}
		if(strcmp(argv[i],howManyFlag)==0)
		{/*Get output file*/
			(*howManyFile)=malloc((strlen(argv[i+1])+1)*sizeof(char));
			strcpy((*howManyFile),argv[i+1]);
		}
		if(strcmp(argv[i],utilArgsFlag)==0)
		{/*Get float args*/
			(*utilsArgs)=malloc((*numOfArgs)*sizeof(float));
			int j;
			for(j=0;j<(*numOfArgs);++j)
			{
				(*utilsArgs)[j]=strtod(argv[i+j+1],NULL);
			}
			if(callerUtility==SEMICIRCLE_CASE)
			{
				(*Point)=malloc((strlen(argv[i+j+1])+1)*sizeof(char));
				strcpy((*Point),argv[i+j+1]);
			}
		}
		if(strcmp(argv[i],offsetFlag)==0)
		{/*Get offset to read*/
			(*offset)=atoi(argv[i+1]);
		}
		if(strcmp(argv[i],pointsFlag)==0)
		{/*Get points to read*/
			(*pointsToReadCount)=atoi(argv[i+1]);
		}
	}
}

void utilityUse(int callerUtility, int argc, char** argv)
{
	int offset=0, pointsToReadCount=-1;
	char* inFile; 
	char* outFile;
	char* howManyFile;
	float* utilsArgs;
	int numOfArgs;
	char* Point;
	makeArguments(argc,&offset,&pointsToReadCount,&inFile,&outFile,&howManyFile,&utilsArgs,argv,&numOfArgs,callerUtility,&Point);
	//Proceed to reading your input
	float* coordinates=malloc(2*sizeof(float)); /*(x,y)*/
	int numOfElems=2;
	int i;
	if(pointsToReadCount<0)
	{
		pointsToReadCount=getBinaryFileSize(inFile)/sizeof(float);
	}
	int fd;
	int fn;
	int howMany=0;
	switch(callerUtility)
	{
		case CIRCLE_CASE:
			fd=open(outFile,O_WRONLY); /*Open the file to write the output*/
			fn=open(howManyFile,O_WRONLY); /*Open the fifo to write how many points to be read*/
			for(i=0;i<pointsToReadCount/2;++i)
			{/*Read points two by two*/
				readFromBinaryFile(inFile, offset, numOfElems,coordinates);
				if(circle((double)utilsArgs[0],(double) utilsArgs[1], (double) utilsArgs[2],(double) coordinates[0],(double) coordinates[1])==TRUE)
				{
					write(fd,&(coordinates[0]),sizeof(float));
					write(fd,&(coordinates[1]),sizeof(float));
					howMany+=2;
				}
				offset+=2;
			}
			write(fn,&howMany,sizeof(int)); /*Write how many points the handler should read from the other fifo*/
			/*Now close the pipes on your side*/
			close(fd);
			close(fn);
			/*And unlink ypurself from them*/
			unlink(outFile);
			unlink(howManyFile);
		break;
		
		case SEMICIRCLE_CASE:
			fd=open(outFile,O_WRONLY); /*Open the file to write the output*/
			fn=open(howManyFile,O_WRONLY); /*Open the fifo to write how many points to be read*/
			for(i=0;i<pointsToReadCount;++i)
				{
					readFromBinaryFile(inFile,offset, numOfElems,coordinates);
					/*call the utility*/
					if(semicircle((double) utilsArgs[0], (double) utilsArgs[1], (double) utilsArgs[2], (double) coordinates[0],
					 (double) coordinates[1], Point)==TRUE)
					{
						write(fd,&(coordinates[0]),sizeof(float));
						write(fd,&(coordinates[1]),sizeof(float));
						howMany+=2;
					}
					offset+=2;
				}
				write(fn,&howMany,sizeof(int)); /*Write how many points the handler should read from the other fifo*/
				/*Now close the pipes on your side*/
				close(fd);
				close(fn);
				/*And unlink ypurself from them*/
				unlink(outFile);
				unlink(howManyFile);
		break;
		
		case RING_CASE:
			fd=open(outFile,O_WRONLY); /*Open the file to write the output*/
			fn=open(howManyFile,O_WRONLY); /*Open the fifo to write how many points to be read*/
			for(i=0;i<pointsToReadCount;++i)
				{
					readFromBinaryFile(inFile, offset, numOfElems,coordinates);
					if(ring((double) utilsArgs[0], (double) utilsArgs[1], (double) utilsArgs[2], (double) utilsArgs[3], 
						(double) coordinates[0], (double) coordinates[1])==TRUE)
					{
						write(fd,&(coordinates[0]),sizeof(float));
						write(fd,&(coordinates[1]),sizeof(float));
						howMany+=2;
					}
					offset+=2;
				}	
				write(fn,&howMany,sizeof(int)); /*Write how many points the handler should read from the other fifo*/
				/*Now close the pipes on your side*/
				close(fd);
				close(fn);
				/*And unlink ypurself from them*/
				unlink(outFile);
				unlink(howManyFile);
		break;
		
		case SQUARE_CASE:
			fd=open(outFile,O_WRONLY); /*Open the file to write the output*/
			fn=open(howManyFile,O_WRONLY); /*Open the fifo to write how many points to be read*/
			for(i=0;i<pointsToReadCount;++i)
				{
					readFromBinaryFile(inFile, offset, numOfElems,coordinates);
					if(square((double) utilsArgs[0], (double) utilsArgs[1], (double) utilsArgs[2], (double) coordinates[0], (double) coordinates[1])==TRUE)
					{
						write(fd,&(coordinates[0]),sizeof(float));
						write(fd,&(coordinates[1]),sizeof(float));
						howMany+=2;
					}
					
					offset+=2;
				}
				write(fn,&howMany,sizeof(int)); /*Write how many points the handler should read from the other fifo*/
				/*Now close the pipes on your side*/
				close(fd);
				close(fn);
				/*And unlink ypurself from them*/
				unlink(outFile);
				unlink(howManyFile);
		break;
		
		case ELLIPSE_CASE:
			fd=open(outFile,O_WRONLY); /*Open the file to write the output*/
			fn=open(howManyFile,O_WRONLY); /*Open the fifo to write how many points to be read*/
			for(i=0;i<pointsToReadCount;++i)
				{
					readFromBinaryFile(inFile, offset, numOfElems,coordinates);
					if(ellipse((double) utilsArgs[0], (double) utilsArgs[1], (double) utilsArgs[2], (double) utilsArgs[3], 
						(double) coordinates[0], (double) coordinates[1])==TRUE)
					{
						write(fd,&(coordinates[0]),sizeof(float));
						write(fd,&(coordinates[1]),sizeof(float));
						howMany+=2;
					}
					offset+=2;
				}
				write(fn,&howMany,sizeof(int)); /*Write how many points the handler should read from the other fifo*/
				/*Now close the pipes on your side*/
				close(fd);
				close(fn);
				/*And unlink ypurself from them*/
				unlink(outFile);
				unlink(howManyFile);
		break;

		default: ;
		break;
	}
	/*CleanUp before return*/
	cleanUp(&inFile,&outFile,&howManyFile,&utilsArgs,&coordinates);
}


void cleanUp(char** inFile, char** outFile, char** howManyFile, float** utilsArgs,float** coordinates)
{
	free((*inFile));
	free((*outFile));
	free((*utilsArgs));
	free((*coordinates));
}