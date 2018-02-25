#include "fileIO.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int readFromBinaryFile(char* fileName,int offset, int numberOfElements,float* buffer)
{
	FILE* fp;

	fp=fopen(fileName,"rb"); //Open binary file to read it
	if(fp==NULL)
	{
		return FALSE;
	}
	else
	{
		if(fseek(fp,offset*sizeof(float),SEEK_SET)==0)//Go to the proper position to read
		{
			fread(buffer,sizeof(float),numberOfElements,fp); //Read
			fclose(fp);
			return TRUE;
		}
		else
		{
			printf("Error with file %s\n",fileName );
			fclose(fp);
			return FALSE;
		}
	}
}


int writeToBinaryFile(char* fileName, float* buffer, int numberOfElements)
{/*Appends a binary file*/
	FILE *fp;
	fp=fopen(fileName,"ab");
	if(fp==NULL)
	{
		printf("Cannot write to file %s\n",fileName);
		return FALSE;
	}
	else
	{
		int offset;
		int size=getBinaryFileSize(fileName);
		offset=size/sizeof(float);
		if(fseek(fp,offset,SEEK_SET)==0) //We want to write at the end of the file
		{
			fwrite(buffer,sizeof(float),numberOfElements,fp);
			fclose(fp);
			return TRUE;
		}
	}
}


int getBinaryFileSize(char* fileName)
{
	/*
	Returns the size of a binary file in bytes
	Using stat
	Code snippet from stackoverflow
	*/
	struct stat st;
	stat(fileName,&st);
	return st.st_size;
}


int createGNUplotScript(char* gnuScriptName, char** fileNames, int arrayLength, char** colors, char* commandSerial)
{/*This function creates the script gnuplot takes as input*/
	FILE *fp;
	fp=fopen(gnuScriptName,"w+");
	if(fp==NULL)
	{
		printf("Unable to create gnuplot script %s\n",gnuScriptName );
		return FALSE;
	}
	else
	{
		/*The first two lines are the same for every gnu script file*/
		fputs("set terminal png\n",fp);
		fputs("set size ratio -1\n",fp);
		char* line=line=malloc(((strlen("set output ")+1+strlen(commandSerial)+1+strlen("\"_image.png\"\n")+1))*sizeof(char));
		strcpy(line,"set output \"");
		strcat(line,commandSerial);
		strcat(line,"_image.png\"\n");
		fputs(line,fp);
		free(line);
		fputs("plot ",fp);
		int i;
		for(i=0;i<arrayLength;++i)
		{
			fputs("\"",fp);
			fputs(fileNames[i],fp);
			fputs("\" notitle with points pointsize 0.5 linecolor rgb \"",fp);
			fputs(colors[i],fp);
			fputs("\"",fp);
			fputs(", ",fp);
			if(i!=(arrayLength-1))
			{
				fputs("\\",fp);
			}
			fputs("\n",fp);
		}
		fclose(fp);
		return TRUE;
	}
}


int createHandlerOutFile(char* fileName, float* buffer, int bufferSize)
{/*This function is used by the handler to create the SHPID.out file*/
	FILE *fp;
	fp=fopen(fileName,"a");
	if(fp==NULL)
	{
		printf("Unable to create file %s\n",fileName );
		return FALSE;
	}
	else
	{
		int i;
		for(i=0;i<bufferSize;++i)
		{
			if((i%2)==0)
			{
				fprintf(fp, "%f\t",buffer[i] );
			}
			else
			{
				fprintf(fp, "%f\n",buffer[i] );
			}
			
		}
		fclose(fp);
		return TRUE;
	}
}