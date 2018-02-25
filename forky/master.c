#include "../pathsHeader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include READINGHEADER
#include "cli.h"
#include "utilitiesList.h"

#define INT_SIZE 50

void makeArraysForHadler(char*** pointsToReadCount,char*** offsets, int numOfWorkers, int inFileSize);
void finalCleanUp(char** fileName, char** tempDirName, char*** offsets, char*** pointsToReadCount, char** command, int arrayLength);

/*This is the code for shapes master*/
int main(int argc, char const *argv[])
{
	/*Going to check we have enough command line arguments to start*/
	if((argc-1)<3)
	{/*We need 3 command line arguments (besides the name of the executable)*/
		printf("Not enough input to work\nExiting . . .");
		exit(1);
	}
	else
	{
		/*First gather your input*/
		char* fileName;
		char* tempDirName;
		int numOfWorkers;
		int i;
		int workersPOs;
		char* inFileFlag="-i";
		char* workersFlag="-w";
		char* dirFlag="-d";
		for(i=1;i<argc;++i)
		{
			if(strcmp(argv[i],inFileFlag)==0)
			{
				fileName=malloc((strlen(argv[i+1])+1)*sizeof(char));
				strcpy(fileName,argv[i+1]);
			}
			if(strcmp(argv[i],workersFlag)==0)
			{
				workersPOs=i+1;
				numOfWorkers=atoi(argv[i+1]);
			}
			if(strcmp(argv[i],dirFlag)==0)
			{
				tempDirName=malloc((strlen(argv[i+1])+1)*sizeof(char));
				strcpy(tempDirName,argv[i+1]);
			}
		}
		/*Now get the length of the input file*/
		int inFileSize=getBinaryFileSize(fileName)/sizeof(float); /*File size in numbers we need to read*/
		if(inFileSize==0)
		{/*Sth went wrong or we were given an empty file*/
			printf("Cannot process empty file.\nExiting . . .\n");
			exit(1);
		}
		else
		{//Workers read input in pairs, so we want to check inFileSize/2
			char** offsets;
			char** pointsToReadCount;
			makeArraysForHadler(&pointsToReadCount,&offsets, numOfWorkers, inFileSize);
			/*Now it is time for the cli entered the pitch*/
			pointerToUtility listOfUtilities=NULL; /*This is the first node of the list of utilities to be created*/
			int toRunOrNotToRun;
			int commandSerial=0;
			do
			{/*Read user input as long as they want the program to run*/
				toRunOrNotToRun=cli(&listOfUtilities);
				if(toRunOrNotToRun==FINE)
				{//The user has given input, so we need to fork handlers and workers
					++commandSerial;
					int listLength=howManyUtilities(listOfUtilities); //We need to fork as many handlers as the number of utilities
					char*** handlerArgs;/*The array of arguments to be passed to the handler(s)*/
					handlerArgs=malloc(listLength*sizeof(char**));
					int i;
					for(i=0;i<listLength;++i)
					{
						char** args=getUtilityArgs(listOfUtilities,i);
						char* numOfArgs=getUtilityNumberOfArgs(listOfUtilities, i);
						int numberOfArgs=atoi(numOfArgs);
						int numOfHandlerArgs=4+7+numberOfArgs+2*numOfWorkers+1;
						/*
						7 for the flags, 4 for: 1:input, 2:output files, 3:number of workers, 4:utility name, and the 2*numberOfWorkers is for the 2 arrays,
						the offsets[numberOfWorkers] and the pointsToReadCount[numberOfWOrkers] + 1 for NULL at the end
						*/
						handlerArgs[i]=malloc(numOfHandlerArgs*sizeof(char*));
						handlerArgs[i][0]=inFileFlag;
						handlerArgs[i][1]=fileName;
						handlerArgs[i][2]=dirFlag;
						handlerArgs[i][3]=tempDirName;
						handlerArgs[i][4]=workersFlag;
						handlerArgs[i][5]=argv[workersPOs];
						handlerArgs[i][6]="-u"; /*Flag for utility name*/
						handlerArgs[i][7]=getUtilityName(listOfUtilities, i);
						handlerArgs[i][8]="-f"; /*Flag for offset*/
						int j;
						for(j=0;j<numOfWorkers;++j)
						{
							handlerArgs[i][8+j+1]=offsets[j];
						}
						handlerArgs[i][8+numOfWorkers+1]="-p"; /*Flag for pointsToReadCount*/
						for(j=0;j<numOfWorkers;++j)
						{
							handlerArgs[i][8+numOfWorkers+j+2]=pointsToReadCount[j];
						}
						handlerArgs[i][8+2*numOfWorkers+2]="-a"; /*Flag for utility arguments*/
						for(j=0;j<numberOfArgs;++j)
						{
							handlerArgs[i][8+2*numOfWorkers+3+j]=args[j];
						}
						handlerArgs[i][numOfHandlerArgs-1]=NULL;
					}
					int status;
					char** pathToResultsFile=malloc(listLength*sizeof(char*));
					char** colors=malloc(listLength*sizeof(char*));
					for(i=0;i<listLength;++i)
					{
						int pid=fork();
						if(pid==0)
						{
							execv(HANDLERPATH,handlerArgs[i]);
						}
						else
						{
							char* pidToString=malloc(INT_SIZE*sizeof(char));
							sprintf(pidToString,"%d",pid);
							pathToResultsFile[i]=malloc((strlen(TEMPDIRPATH)+1+strlen(tempDirName)+1+strlen("/")+1+strlen(pidToString)+1+strlen(".out")+1)
								*sizeof(char));
							strcpy(pathToResultsFile[i],TEMPDIRPATH);
							strcat(pathToResultsFile[i],tempDirName);
							strcat(pathToResultsFile[i],"/");
							strcat(pathToResultsFile[i],pidToString);
							strcat(pathToResultsFile[i],".out");
							/*Now get the color*/
							colors[i]=getUtilityColor(listOfUtilities, i);
							free(pidToString);
						}
					}
					while(wait(&status)>0)
					{/*Wait for all children to finish*/
						;
					}
					/**Now create your output file**/
					char commandSerialToStr[INT_SIZE];
					sprintf(commandSerialToStr,"%d",commandSerial);
					char* finalResultsFile=malloc((strlen(TEMPDIRPATH)+1+strlen(tempDirName)+1+strlen("/")+1
						+strlen(commandSerialToStr)+1+strlen("_script.gnuplot")+1)*sizeof(char));
					strcpy(finalResultsFile,TEMPDIRPATH);
					strcat(finalResultsFile,tempDirName);
					strcat(finalResultsFile,"/");
					strcat(finalResultsFile,commandSerialToStr);
					strcat(finalResultsFile,"_script.gnuplot");
					createGNUplotScript(finalResultsFile, pathToResultsFile, listLength,colors,commandSerialToStr);
					/*Call gnuplot now*/
					int gnuPid=fork();
					if(gnuPid==0)
					{/*Forked child*/
						execlp(GNUPLOTPATH,GNUPLOTEXEC,finalResultsFile,(char *)NULL);
					}
					else
					{/*Wait for gnuplot to finish*/
						wait(&status);
					}
					/*Now free your mallocs*/
					for(i=0;i<listLength;++i)
					{
						free(handlerArgs[i]);
						free(pathToResultsFile[i]);
					}	
					free(handlerArgs);
					free(pathToResultsFile);
					free(colors);
					free(finalResultsFile);
				}
				/*Now time to do some cleanup*/
				deleteUtilities(&listOfUtilities);/*First delete the list of utilities already inserted*/
				/*Now you run again or you exit*/
			} while (toRunOrNotToRun!=EXIT);
			/*Perform final cleanup before you go*/
			char* command=malloc((strlen(TEMPDIRPATH)+1+strlen(tempDirName)+1+strlen("rm -r ")+1)*sizeof(char));
			strcpy(command,"rm -r ");
			strcat(command,TEMPDIRPATH);
			strcat(command,tempDirName);
			system(command);
			finalCleanUp(&fileName, &tempDirName, &offsets, &pointsToReadCount, &command,numOfWorkers);
		}
	}
	printf("Exiting . . .\n");
}



void makeArraysForHadler(char*** pointsToReadCount,char*** offsets, int numOfWorkers, int inFileSize)
{
	int collectiveOffsets=0;
	char* assistant=malloc(INT_SIZE*sizeof(char));
	(*offsets)=malloc(numOfWorkers*sizeof(char*));
	(*pointsToReadCount)=malloc(numOfWorkers*sizeof(char*));
	int i;
	int pointsToRead;
	int pairs=inFileSize/2;
	if((pairs%numOfWorkers)==0)
	{/*The pairs number is divisible by the number of workers*/
		for(i=0;i<numOfWorkers;++i)
		{
			pointsToRead=inFileSize/numOfWorkers;
			sprintf(assistant,"%d",pointsToRead);
			(*pointsToReadCount)[i]=malloc((strlen(assistant)+1)*sizeof(char));
			strcpy((*pointsToReadCount)[i],assistant);
			if(i==0)
			{/*The first worker reads from the start*/
				(*offsets)[i]=malloc((strlen("0")+1)*sizeof(char));
				strcpy((*offsets)[i],"0");
			}
			else
			{
				sprintf(assistant,"%d",collectiveOffsets);
				(*offsets)[i]=malloc((strlen(assistant)+1)*sizeof(char));
				strcpy((*offsets)[i],assistant);
			}
			collectiveOffsets+=pointsToRead;
		}
	}
	else
	{
		int extra=pairs%numOfWorkers;
		for(i=0;i<numOfWorkers;++i)
		{
			pointsToRead=2*(pairs/numOfWorkers);
			if(extra>0)
			{/*Read one more pair, 2 more points*/
				pointsToRead+=2;
			}
			--extra;
			sprintf(assistant,"%d",pointsToRead);
			(*pointsToReadCount)[i]=malloc((strlen(assistant)+1)*sizeof(char));
			strcpy((*pointsToReadCount)[i],assistant);
			if(i==0)
			{/*The first worker reads from the start*/
				//(*offsets)[i]="0";
				(*offsets)[i]=malloc((strlen("0")+1)*sizeof(char));
				strcpy((*offsets)[i],"0");
			}
			else
			{
				sprintf(assistant,"%d",collectiveOffsets);
				(*offsets)[i]=malloc((strlen(assistant)+1)*sizeof(char));
				strcpy((*offsets)[i],assistant);
			}
			collectiveOffsets+=pointsToRead;
		}
	}
	//free(assistant);
}	

void finalCleanUp(char** fileName, char** tempDirName, char*** offsets, char*** pointsToReadCount, char** command, int arrayLength)
{
	int i;
	free((*tempDirName));
	free((*fileName));
	free((*command));
	for(i=0;i<arrayLength;++i)
	{
		free((*offsets)[i]);
		free((*pointsToReadCount)[i]);
	}
	free((*offsets));
	free((*pointsToReadCount));
}
