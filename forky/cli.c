#include "cli.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int cli(pointerToUtility *start)
{
	char buffer[BUFFER_MAX];
	printf("Write your command>");
	fgets(buffer,BUFFER_MAX,stdin);      /*fgets bc it's safer than gets*/
	char instruction[20];
	//const char separator=',';
	char* position; 
	position=memchr(buffer,',',strlen(buffer));
	if(position==NULL)
	{/*There are no commas; therefore we got only one in*/
		return readCommand(start,buffer);
	}
	else
	{/*We have multiple instructions, therefore we need to tokenize the input in order to process it. The rest is the same though*/
		char* token=strtok(buffer,",");
		int ret=1;
		while(token!=NULL)
		{
			printf("%s\n",token );
			ret*=readCommand(start,token);
			token=strtok(NULL,",");
			
		}
		if(ret<0)
		{
			return ERROR;
		}
		else if(ret==0)
		{
			return EXIT;
		}
		else
		{
			return FINE;
		}
	}
}



int readCommand(pointerToUtility *start,char* buffer)
{
	char instruction[20];
	char flag[4];
	char x[FLOAT_LENGTH];
	char y[FLOAT_LENGTH];
	char r[FLOAT_LENGTH];
	char r1[FLOAT_LENGTH];
	char r2[FLOAT_LENGTH];
	char h[FLOAT_LENGTH];
	char k[FLOAT_LENGTH];
	char a[FLOAT_LENGTH];
	char b[FLOAT_LENGTH];
	char Point[4];
	char color[10];/*Color is red, blue or green*/
	char end[4]; /*end should be a ";" or a "," */
	if(sscanf(buffer,"%s",instruction)) /*Successfully read your instruction*/
	{
		if(strcmp(instruction,"exit;")==0)
		{
			return EXIT;
		}
		else if(strcmp(instruction,"circle")==0)
		{//Further reading
			if((((sscanf(buffer,"%s %s %s %s %s %s %s",instruction,flag,x,y,r,color,end)==7)&&(strcmp(end,";")==0)))||
			((sscanf(buffer,"%s %s %s %s %s %s",instruction,flag,x,y,r,color)==6)&&(!(strcmp(color,";")==0))))
			{
				char* numberOfArgs="3";
				/*First turn your floats into strings again*/
				char* array[]={x,y,r};
				insertUtility(start,instruction, array, numberOfArgs,color);
				return FINE;
			}
			else
			{
				printf("No such command\n");
				return ERROR;
			}
		}
		else if(strcmp(instruction,"ring")==0)
		{
			if((((sscanf(buffer,"%s %s %s %s %s %s %s %s",instruction,flag,x,y,r1,r2,color,end)==8)&&(strcmp(end,";")==0)))||
				((sscanf(buffer,"%s %s %s %s %s %s %s",instruction,flag,x,y,r1,r2,color)==7)&&(!(strcmp(color,";")==0))))
			{
				char* numberOfArgs="4";
				char* array[]={x,y,r1,r2};
				insertUtility(start,instruction, array, numberOfArgs,color);
				return FINE;
			}
			else
			{
				printf("No such command\n");
				return ERROR;
			}
		}
		else if(strcmp(instruction,"semicircle")==0)
		{
			if((((sscanf(buffer,"%s %s %s %s %s %s %s %s",instruction,flag,x,y,r,Point,color,end)==8)&&(strcmp(end,";")==0)))||
				((sscanf(buffer,"%s %s %s %s %s %s %s",instruction,flag,x,y,r,Point,color)==7)&&(!(strcmp(color,";")==0))))
			{
				char* numberOfArgs="4";
				char* array[]={x,y,r,Point};
				insertUtility(start,instruction, array, numberOfArgs,color);
				return FINE;
			}
			else
			{
				printf("No such command\n");
				return ERROR;
			}
		}
		else if(strcmp(instruction,"square")==0)
		{
			if((((sscanf(buffer,"%s %s %s %s %s %s %s",instruction,flag,x,y,r,color,end)==7)&&(strcmp(end,";")==0)))||
				((sscanf(buffer,"%s %s %s %s %s %s",instruction,flag,x,y,r,color)==6)&&(!(strcmp(color,";")==0))))
			{
				char* numberOfArgs="3";
				char* array[]={x,y,r};
				insertUtility(start,instruction, array, numberOfArgs,color);
				return FINE;
			}
			else
			{
				printf("No such command\n");
				return ERROR;
			}
		}
		else if(strcmp(instruction,"ellipse")==0)
		{
			if((((sscanf(buffer,"%s %s %s %s %s %s %s %s",instruction,flag,h,k,a,b,color,end)==8)&&(strcmp(end,";")==0)))||
				((sscanf(buffer,"%s %s %s %s %s %s %s",instruction,flag,h,k,a,b,color)==7)&&(!(strcmp(color,";")==0))))
			{
				char* numberOfArgs="4";
				char* array[]={h,k,a,b};
				insertUtility(start,instruction, array, numberOfArgs,color);
				return FINE;
			}
			else
			{
				printf("No such command\n");
				return ERROR;
			}
		}
		else
		{
			printf("No such command\n");
			return ERROR;
		}
	}
	else
	{
		return ERROR;
	}
}

