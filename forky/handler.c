#include "../pathsHeader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include READINGHEADER
//#include "utilitiesList.h"

#define INT_SIZE 50
#define NAME_BUFFER 100
#define ACCESS_RIGHTS 0666


void finalCleanUp(char*** outPutFiles, char*** numOfPointsFifo, char** command, int arraySize, int** fn, int** fd, char** pidToStr, char** number);

/*This is the code for the handler process*/
int main(int argc, char const *argv[])
{
	int i;
	int numOfWorkers;
	char* workerFlag="-w";
	char* offsetFlag="-f";
	int offsetPos;
	char* utilityNameFlag="-u";
	int utilityNamePos;
	char* pointsToReadFlag="-p";
	int pointsToReadPos;
	char* argsFlag="-a";
	int argsPos;
	char* dirFlag="-d";
	int dirPos;
	char* inFileFlag="-i";
	int inFilePos;
	for(i=0;i<argc;++i)
	{//Here, the first argument is not the name of the executable
		if(strcmp(argv[i],workerFlag)==0)
		{
			numOfWorkers=atoi(argv[i+1]); /*The number of workers*/
		}
		if(strcmp(argv[i],utilityNameFlag)==0)
		{
			utilityNamePos=i+1; /*The name of the utility*/
		}
		if(strcmp(argv[i],offsetFlag)==0)
		{
			offsetPos=i+1; /*Where the offset array starts*/
		}
		if(strcmp(argv[i],pointsToReadFlag)==0)
		{
			pointsToReadPos=i+1; /*The the points to read array starts*/
		}
		if(strcmp(argv[i],argsFlag)==0)
		{
			argsPos=i+1; /*Where the array of the utility arguments starts*/
		}
		if(strcmp(argv[i],dirFlag)==0)
		{
			dirPos=i+1; /*The name of the temporary folder*/
		}
		if(strcmp(argv[i],inFileFlag)==0)
		{
			inFilePos=i+1;
		}
	}
	/*Create arrays for the files and the pids of the children*/
	int myPid=getpid(); /*Handler's pid*/
	char* number=malloc(INT_SIZE*sizeof(char)); /*Buffer to host a number's string version*/
	char* pidToStr=malloc(INT_SIZE*sizeof(char)); /*The string version of a pid*/
	sprintf(pidToStr,"%d",myPid);
	int kidsPids[numOfWorkers]; /* This array holds the pids of the children */
	int* fd=malloc(numOfWorkers*sizeof(int)); /*File descriptors for the pipes*/
	int* fn=malloc(numOfWorkers*sizeof(int)); /*File descriptors for the pipes that contain the number of results*/
	/**Now the utility's arguments **/
	char** outPutFiles=malloc(numOfWorkers*sizeof(char*)); //We need to create a FIFO for each worker, to write their results
	char** numOfPointsFifo=malloc(numOfWorkers*sizeof(char*));//And another fifo for each worker, to write how many points the handler should read
	for(i=0;i<numOfWorkers;++i)
	{
		char* fifoName=malloc(NAME_BUFFER*sizeof(char));
		char* resName=malloc(NAME_BUFFER*sizeof(char));
		/*The fifo for the output*/
		sprintf(number,"%d",i);
		strcpy(fifoName,pidToStr);
		strcat(fifoName,"_w");
		strcat(fifoName,number);
		strcat(fifoName,".fifo");
		int length=strlen(TEMPDIRPATH)+1+strlen(argv[dirPos])+1+strlen(fifoName)+1+strlen("/")+1;
		outPutFiles[i]=malloc(length*sizeof(char));
		strcpy(outPutFiles[i],TEMPDIRPATH);
		strcat(outPutFiles[i],argv[dirPos]);
		strcat(outPutFiles[i],"/");
		strcat(outPutFiles[i],fifoName);
		/*The fifo that tells the handler how many points to read form the other fifo*/
		strcpy(resName,pidToStr);
		strcat(resName,"numOfPoints_w");
		strcat(resName,number);
		strcat(resName,".fifo");
		length=strlen(TEMPDIRPATH)+1+strlen(argv[dirPos])+1+strlen(resName)+1+strlen("/")+1;
		numOfPointsFifo[i]=malloc(length*sizeof(char));
		strcpy(numOfPointsFifo[i],TEMPDIRPATH);
		strcat(numOfPointsFifo[i],argv[dirPos]);
		strcat(numOfPointsFifo[i],"/");
		strcat(numOfPointsFifo[i],resName);
		free(fifoName);
		free(resName);
	}
	//Now the handler will create the fifos
	int dirMakeLen=strlen(TEMPDIRPATH)+1+strlen(argv[dirPos])+1+strlen("mkdir ")+1;
	char* command=malloc(dirMakeLen*sizeof(char));
	strcpy(command,"mkdir ");
	strcat(command,TEMPDIRPATH);
	strcat(command,argv[dirPos]);
	system(command);
	//First create the fifos for the results
	for(i=0;i<numOfWorkers;++i)
	{
		int fo=mkfifo(outPutFiles[i],ACCESS_RIGHTS);
		if(fo==-1)
		{
			printf("Error creating fifo %s\nExiting  . . .\n", outPutFiles[i]);
			exit(1);
		}
	}
	//And now create the fifos for the number of results
	for(i=0;i<numOfWorkers;++i)
	{
		int fo=mkfifo(numOfPointsFifo[i],ACCESS_RIGHTS);
		if(fo==-1)
		{
			printf("Error creating fifo %s\nExiting  . . .\n", numOfPointsFifo[i]);
			exit(1);
		}
	}
	/**Find the utility and its path**/
	char* utilityPath;
	char* utilityName;
	int utilNumOfArgs;
	if(strcmp(argv[utilityNamePos],CIRCLE)==0)
	{
		utilityPath=CIRCLEPATH; /*Path to the executable*/
		utilityName=CIRCLE_UTIL;/*Executable*/
		utilNumOfArgs=3;
	}
	else if(strcmp(argv[utilityNamePos],SEMICIRCLE)==0)
	{
		utilityPath=SEMICIRCLEPATH; /*Path to the executable*/
		utilityName=SEMICIRCLE_UTIL;/*Executable*/
		utilNumOfArgs=4;
	}
	else if(strcmp(argv[utilityNamePos],RING)==0)
	{
		utilityPath=RINGPATH; /*Path to the executable*/
		utilityName=RING_UTIL;/*Executable*/
		utilNumOfArgs=4;
	}
	else if(strcmp(argv[utilityNamePos],SQUARE)==0)
	{
		utilityPath=SQUAREPATH; /*Path to the executable*/
		utilityName=SQUARE_UTIL;/*Executable*/
		utilNumOfArgs=3;
	}
	else if(strcmp(argv[utilityNamePos],ELLIPSE)==0)
	{
		utilityPath=ELLIPSEPATH; /*Path to the executable*/
		utilityName=ELLIPSE_UTIL;/*Executable*/
		utilNumOfArgs=4;
	}
	//Now fork the workers
	for(i=0;i<numOfWorkers;++i)
	{
		int pid=fork();
		if(pid==0)
		{//Child's code
			/*call the utility you need*/
			if(utilNumOfArgs==3)
			{
				execl(utilityPath,utilityName,inFileFlag,argv[inFilePos],"-o",outPutFiles[i],
				"-a",argv[argsPos],argv[argsPos+1],argv[argsPos+2],"-f",argv[offsetPos+i],"-n",argv[pointsToReadPos+i],
				"-h",numOfPointsFifo[i],(char *)NULL);

			}
			else if(utilNumOfArgs==4)
			{
				execl(utilityPath,utilityName,inFileFlag,argv[inFilePos],"-o",outPutFiles[i],
				"-a",argv[argsPos],argv[argsPos+1],argv[argsPos+2],argv[argsPos+3],"-f",argv[offsetPos+i],"-n",argv[pointsToReadPos+i],
				"-h",numOfPointsFifo[i],(char *)NULL);
			}
		}
		else
		{/*Parent's code*/
			kidsPids[i]=pid;
		}
	}
	for(i=0;i<numOfWorkers;++i)
	{/*Parent opens all fifos*/
		fd[i]=open(outPutFiles[i],O_RDONLY,O_NONBLOCK);
		fn[i]=open(numOfPointsFifo[i],O_RDONLY,O_NONBLOCK);
	}
	char* fullFinalOutputFile=malloc((strlen(TEMPDIRPATH)+1+strlen(argv[dirPos])+1+strlen("/")+1+strlen(pidToStr)+1+strlen(".out")+1)*sizeof(char));
	strcpy(fullFinalOutputFile,TEMPDIRPATH);
	strcat(fullFinalOutputFile,argv[dirPos]);
	strcat(fullFinalOutputFile,"/");
	strcat(fullFinalOutputFile,pidToStr);
	strcat(fullFinalOutputFile,".out");
	int status;
	int counter=0;
	int finished;
	while(counter<numOfWorkers)
	{/*As long as there are children working, execute this code*/
		finished=wait(&status);
		++counter; //One more child has finished
		/*Open the child's fifo to read*/
		/*First find the childs number*/
		for(i=0;i<numOfWorkers;++i)
		{
			if(kidsPids[i]==finished)
			{
				break;
			}
		}
		/*Read from the child's pipe*/
		/*We expect the number of points that belong to the shape to be equal to number of points the child read, at most*/
		/*Get how many points should be read*/
		int howMany;
		read(fn[i],&howMany,sizeof(int));
		float* points=malloc(howMany*sizeof(float));
		read(fd[i],points,howMany*sizeof(int));
		/*Close the pipes*/
		close(fn[i]);
		close(fd[i]);
		/*Write the results in the SHPID.out file*/
		createHandlerOutFile(fullFinalOutputFile, points, howMany);
		free(points);
	}
	/*Cleanup before you exit*/
	finalCleanUp(&outPutFiles,&numOfPointsFifo,&command,numOfWorkers,&fn,&fd,&pidToStr,&number);

	exit(0);
}


void finalCleanUp(char*** outPutFiles, char*** numOfPointsFifo, char** command, int arraySize,int** fn, int** fd, char** pidToStr, char** number)
{
	free((*command));
	int i;
	for(i=0;i<arraySize;++i)
	{
		free((*outPutFiles)[i]);
		free((*numOfPointsFifo)[i]);
	}
	free((*outPutFiles));
	free((*numOfPointsFifo));
	free((*fd));
	free((*fn));
	free((*pidToStr));
	free((*number));
}