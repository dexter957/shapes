
#include "utilitiesList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
We want the user to be able to insert an arbitrary number of utilities to be executed at the same time. Therefore we need a data
structure that allows us to store everything we possibly need to know about a utility to be executed. Since we use this list to keep
the user's input, we include in the struct only the args given to us with the flag -a, as the rest of them are produced by the master 
and/or the handler.
*/



struct utility
{
	char* utilityName;
	char** utilityArgs;
	char* numOfArgs;
	char* color;
	pointerToUtility nextUtility;
};



int emptyList(pointerToUtility start)
{
	if(start==NULL)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


int insertUtility(pointerToUtility* start,char* name, char** args, char* numOfArgs, char* color)
{
	pointerToUtility assistant;
	/*Find the proper position*/
	if(emptyList((*start)))
	{
		(*start)=malloc(sizeof(struct utility));
		assistant=(*start);
	}
	else
	{
		assistant=(*start);
		while(assistant->nextUtility!=NULL)
		{
			assistant=assistant->nextUtility;
		}
		assistant->nextUtility=malloc(sizeof(struct utility));
		assistant=assistant->nextUtility;
	}
	/*Actual node initialisation*/
	if(assistant==NULL)
	{
		printf("Memory allocation failed\n");
		return FALSE;
	}
	else
	{
		int numberOfArgs=atoi(numOfArgs);
		assistant->utilityName=malloc((strlen(name)+1)*sizeof(char));
		assistant->utilityArgs=malloc(numberOfArgs*sizeof(char*));
		assistant->numOfArgs=malloc((strlen(numOfArgs)+1)*sizeof(char));
		assistant->color=malloc((strlen(color)+1)*sizeof(char));
		if((assistant->utilityName==NULL)||(assistant->utilityArgs==NULL)||(assistant->numOfArgs==NULL)||(assistant->color==NULL))
		{
			printf("Memory allocation failed\n");
			free(assistant->utilityArgs);
			free(assistant->utilityName);
			free(assistant->numOfArgs);
			free(assistant->color);
			free(assistant);
			return FALSE;
		}
		else
		{
			strcpy(assistant->utilityName,name);
			int i;
			for(i=0;i<numberOfArgs;++i)
			{
				assistant->utilityArgs[i]=malloc((strlen(args[i])+1)*sizeof(char));
				strcpy(assistant->utilityArgs[i],args[i]);
			}
			strcpy(assistant->numOfArgs,numOfArgs);
			strcpy(assistant->color,color);
			assistant->nextUtility=NULL;
			return TRUE;
		}
	}
}

void deleteUtilities(pointerToUtility* start)
{
	pointerToUtility iterator=(*start);
	pointerToUtility toDelete;
	while(iterator!=NULL)
	{
		toDelete=iterator;
		iterator=iterator->nextUtility;
		free(toDelete->utilityName);
		int i;
		int numberOfArgs=atoi(toDelete->numOfArgs);
		for(i=0;i<numberOfArgs;++i)
		{
			free(toDelete->utilityArgs[i]);
		}
		free(toDelete->utilityArgs);
		free(toDelete->numOfArgs);
		free(toDelete->color);
		free(toDelete);
	}
	(*start)=NULL;
}

char* getUtilityName(pointerToUtility start, int nodeOrder)
{
	pointerToUtility assistant=start;
	int i;
	for(i=0;i<nodeOrder;++i)
	{
		assistant=assistant->nextUtility;
	}
	return assistant->utilityName;
}

char** getUtilityArgs(pointerToUtility start,int nodeOrder)
{
	pointerToUtility assistant=start;
	int i;
	for(i=0;i<nodeOrder;++i)
	{
		assistant=assistant->nextUtility;
	}
	return assistant->utilityArgs;
}

char* getUtilityNumberOfArgs(pointerToUtility start,int nodeOrder)
{
	pointerToUtility assistant=start;
	int i;
	for(i=0;i<nodeOrder;++i)
	{
		assistant=assistant->nextUtility;
	}
	return assistant->numOfArgs;
}

char* getUtilityColor(pointerToUtility start,int nodeOrder)
{
	pointerToUtility assistant=start;
	int i;
	for(i=0;i<nodeOrder;++i)
	{
		assistant=assistant->nextUtility;
	}
	return assistant->color;
}

int howManyUtilities(pointerToUtility start)
{
	int length=0;
	pointerToUtility assistant=start;
	while(assistant!=NULL)
	{
		++length;
		assistant=assistant->nextUtility;
	}
	return length;
}

//debbugin function

void printUtilitiesList(pointerToUtility start)
{
	pointerToUtility iterator=start;
	while(iterator!=NULL)
	{
		printf("Name:%s\n",iterator->utilityName );
		int i;
		int numberOfArgs=atoi(iterator->numOfArgs);
		for(i=0;i<numberOfArgs;++i)
		{
			printf("%s\n",iterator->utilityArgs[i] );
		}
		iterator=iterator->nextUtility;
	}
}

