/* 
 * dataUpdate.c
 *
 * Module for updating data ( prepare for writing to output file )
 * 
 * Created by Siriwimon Suksukhon (Poo), ID 3436
 * 15 May 2016
 * - Team We Must Survive -
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "mainStructure.h"
#include "globalAccess.h"
#include "checkMath.h"

/* Function to check type of data and store its type into string
 * Argument:
 * input => string that we want to check
 * type => string that will contain input's type
 * Return :
 * 0 => if it's invalid type
 * 1 => if it's valid type and checking success
 */
int checkType(char input[], char type[])
	{
	int i = 0;
	VARIABLE_T* findVar = NULL;		/* store a structure of variable that we found in line */				

	findVar = searchWord(input);		/* find the variable that has name match with 'input' */

	/* if it's not a variable, we will find what type it is */
	if ( findVar == NULL )
		{
		/* if it doesn't have exactly two quotes (initial and terminal) then we know it's not a string or char */
		if ( input[0] != '\"' || input[strlen(input)-1] != '\"' )
			{
			/* if it has only one quote, return 0 */
			if ( input[0] != '\"' && input[strlen(input)-1] == '\"' )
				{
				return 0;
				}
			else if ( input[0] == '\"' && input[strlen(input)-1] != '\"' )					
				{
				return 0;
				}
			/* if it doesn't have any quote, we absolutely know that it must be an int or double or char */ 
			else
				{
				if ( input[0] == '\'' && input[strlen(input)-1] == '\'' )
					{
					strcpy(type,"char");
					}
				else if ( input[0] != '\'' && input[strlen(input)-1] == '\'' )
					{
					return 0;
					}
				else if ( input[0] == '\'' && input[strlen(input)-1] != '\'' )
					{
					return 0;
					}
				else
					{
					/* if it still has any alphabet or space then return 0 */
					for (i=0;i<strlen(input);i++)
						{
						if ( isalpha(input[i]) )
							{
							return 0;
							}
						else if ( isspace(input[i]) )
							{
							return 0;
							}
						/* if we found dot, we suddenly know it is a double */
						else if ( input[i] == '.' )
							{
							strcpy(type,"double");
							}
						}
					/* if it's nothing until now, it's an int */
					if ( strcasecmp(type,"double") != 0 )
						{
						strcpy(type,"int");
						}
					}
				}
			}
		/* if it has two quotes, we completely know that it's a string */
		else
			{
			strcpy(type,"string");
			}
		}
	/* if it's a variable, we can catch its type suddenly */
	else
		{
		strcpy(type,findVar->type);
		}
	return 1;
	}

/* function to check that condition is valid
 * Argument:
 * - input => string of condition
 * Return:
 * - 1 => if condition is ok
 * - 0 => if condition is not ok 
 */
int checkCondition ( char input[] , char con[])
	{
	char* pToken;					/* hold token from each times of strtok */
	char item[512];					/* hold each item for checking type process */
	char tempDelim[512];				/* temporary of string that has a space (we need to strtok 2 times) */
	char tempData[512];				/* hold a copy of condition (input) */
	int i = 0;
	char tempCon[512];				/* use to combine all sub coondition into one condition */
	char leftToken[512];				/* store the token before the operator ( in sub condition ) */
	char sign[512];					/* store the operator ( in sub condition ) */
	char rightToken[512];				/* store the token after the operator ( in sub condition ) */
	char type1[16];					/* handle type of the left token */
	char type2[16];					/* handle type of the right token */
	char* hold_input;				/* pointer to condition */

	memset(tempCon,0,sizeof(tempCon));
	memset(tempDelim,0,sizeof(tempDelim));
	strcpy(tempData,input);
	hold_input = tempData;				/* pointer to temorary data of line */
	pToken = strtok_r(hold_input," ",&hold_input);	/* pop the first token */
	while ( pToken != NULL )
		{
		/* if it's a string that has a space, we need to strtok again */
		if ( pToken[0] == '\"' && pToken[strlen(pToken)-1] != '\"')
			{
			strcat(tempDelim,pToken);
			strcat(tempDelim," ");
			if ( strstr(hold_input,"\"") == NULL )
				{
				return 0;
				}
			pToken = strtok_r(NULL,"\"",&hold_input);
			strcat(tempDelim,pToken);
			strcat(tempDelim,"\"");
			strcpy(item,tempDelim);
			memset(tempDelim,0,sizeof(tempDelim));
			}
		/* if not, we can copy it suddenly */
		else
			{
			strcpy(item,pToken);
			}
		memset(type1,0,sizeof(type1));
		memset(type2,0,sizeof(type2));

		if ( checkType(item,type1) == 0 )
			{
			return 0;
			}
		strcpy(leftToken,item);

		/* checking the middle token */
		pToken = NULL;
		pToken = strtok_r(NULL," ",&hold_input);
		if ( pToken == NULL )
			{
			return 0;
			}	
		
		/* if it's not a valid sign, return 0 */
		if ( strcasecmp(pToken,">") != 0 && strcasecmp(pToken,"<") != 0 && strcasecmp(pToken,"==") != 0 && strcasecmp(pToken,"!=") != 0 && strcasecmp(pToken,">=") != 0 && strcasecmp(pToken,"<=") != 0 )
			{
			return 0;
			}
		strcpy(sign,pToken);

		/* checking third token, same as the first token */
		pToken = NULL;
		pToken = strtok_r(NULL," ",&hold_input);
		if ( pToken == NULL )
			{
			return 0;
			}	
		if ( pToken[0] == '\"' && pToken[strlen(pToken)-1] != '\"' )
			{
			strcat(tempDelim,pToken);
			strcat(tempDelim," ");
			if ( strstr(hold_input,"\"") == NULL )
				{
				return 0;
				}
			pToken = strtok_r(NULL,"\"",&hold_input);
			strcat(tempDelim,pToken);
			strcat(tempDelim,"\"");
			strcpy(item,tempDelim);
			memset(tempDelim,0,sizeof(tempDelim));
			}
		else
			{
			strcpy(item,pToken);
			}
		if ( checkType(item,type2) == 0 )
			{
			return 0;
			}
		/* if type of the two data in condition is not the same, return 0 */
		if ( strcasecmp(type1,"char") == 0 || strcasecmp(type1,"string") == 0 || strcasecmp(type2,"char") == 0 || strcasecmp(type2,"string") == 0 )
			{
			if ( strcasecmp(type1,type2) != 0 )
				{
				return 0;
				}
			}

		strcpy(rightToken,item);

		if ( strcasecmp(type1,"string") == 0 )
			{
			strcat(tempCon,"strcmp(");
			strcat(tempCon,leftToken);
			strcat(tempCon,",");
			strcat(tempCon,rightToken);
			strcat(tempCon,") ");
			strcat(tempCon,sign);
			strcat(tempCon," 0");
			}
		else
			{
			strcat(tempCon,leftToken);
			strcat(tempCon," ");
			strcat(tempCon,sign);
			strcat(tempCon," ");
			strcat(tempCon,rightToken);
			}

		/* pop the next token, it must be && or || */
		pToken = NULL;
		pToken = strtok_r(NULL," ",&hold_input);

		if ( pToken != NULL )
			{
			if ( strcasecmp(pToken,"||") != 0 && strcasecmp(pToken,"&&") != 0 )
				{
				return 0;
				}
			else if ( strlen(hold_input) == 0 )
				{
				return 0;
				}
			else
				{
				strcat(tempCon," ");
				strcat(tempCon,pToken);
				strcat(tempCon," ");
				}
			}
		pToken = NULL;	
		pToken = strtok_r(NULL," ",&hold_input);
		}
	strcpy(con,tempCon);
	return 1;
	}

/* function to check that name is available
 * Arguments:
 * - tempLine => string of that name
 * - command => its command
 * - varType => types that name can be
 * Return:
 * 1 -> if we find that it's an available 
 * 0 -> if not
 */
int checkName ( char tempLine[] , char command[] , char varType[] )
	{
	VARIABLE_T* tempVar = NULL;			/* temporary for variable */
	int i = 0;
	char type[512];					/* store type of tempLine */

	tempVar = searchWord(tempLine);

	/* if it's not a variable, we need to find that is it match with command */ 
	if ( tempVar == NULL )
		{
		if ( strcasecmp(command,"print") != 0 && strcasecmp(command,"write") != 0 )
			{
			printf("Error - variable is undeclared (first use in this function)\n");
			return 0;
			}
		if ( tempLine[0] != '\"' || tempLine[strlen(tempLine)-1] != '\"' )
			{
			printf("Error - variable or string not found\n");
			return 0;
			}
		}
	/* if it's a variable, check its type */
	else
		{
		sprintf(type,"%c",tolower(tempVar->type[0]));
		if ( strstr(varType,type) == NULL )
			{
			printf("Error - this variable is not in supported type\n");
			return 0;
			}	
		}
	return 1;		
	}

/* function to separate line into each argument of command and store it into temporary data
 * Arguments:
 * - rule => rule for that command
 * - input => line
 * - data => an empty temporary data
 * Return:
 * - 1 if data update success and store line into data already
 * - 0 if something wrong with that line
 */
int dataUpdate ( RULE_T* rule, char input[], TEMP_T* data )
	{
	RULE_T* tempRule = NULL;			/* store temporary of rule */
	char command[256];				/* store the first string as a command */
	char line[512];					/* store temporary of line */
	char pre_post_in[256];				/* store temporary of prein/postin */
	char* tempFormat;				/* hold each token of prein/postin */
	char* tempLine;					/* hold each token of line */
	char lineCondition[512];			/* store condition for checking */
	char* hold_format = NULL;			/* pointer to prein/postin */
	char* hold_line = NULL;				/* pointer to line */			
	int bFormat = 0;				/* boolean to check format */
	int bName = 0;					/* boolean to check name */
	VARIABLE_T* tempVar = NULL;			/* store temporary of variable */
	FILE_T* tempFile = NULL;			/* store temporary of pointer to file */
	int bCondition = 0;				/* boolean to check condition */
	char* delim = NULL;				/* break between command and line */
	char tempString[512];				/* temporary of string that has a space ( need to strtok 2 times ) */
	int bFree = 0;					/* boolean to check that strdup is used */
	int i = 0;	

	memset(data->condition,0,sizeof(data->condition));
	memset(data->varName,0,sizeof(data->varName));
	memset(data->varType,0,sizeof(data->varType));
	memset(data->varSymbol,0,sizeof(data->varSymbol));
	memset(data->value,0,sizeof(data->value));
	memset(data->increm,0,sizeof(data->increm));
	memset(data->filePointer,0,sizeof(data->filePointer));
	memset(data->filePath,0,sizeof(data->filePath));
	memset(data->fileMode,0,sizeof(data->fileMode));
	memset(tempString,0,sizeof(tempString));
	memset(line,0,sizeof(line));
	memset(command,0,sizeof(command));
	memset(pre_post_in,0,sizeof(pre_post_in));
	memset(lineCondition,0,sizeof(lineCondition));

	tempRule = rule;				/* store rule into temporary */
	strcpy(command,input);				/* store line into temporary */
	
	delim = strpbrk(command," ");			/* separate between command and arguments */

	/* if it doesn't have any space, we know it's a postkey, then return 1 */
	if ( delim == NULL )
		{
		return 1;
		}
	*delim = '\0';
	strcpy(line,delim+1);

	/* storing format of command */
	if ( strlen(tempRule->preIn) != 0 )
		{
		strcpy(pre_post_in,tempRule->preIn);
		}
	else
		{
		strcpy(pre_post_in,tempRule->postIn);
		}

	hold_format = pre_post_in;			/* pointer to format */
	hold_line = line;				/* pointer to line */

	/* if user wants to declare a variable, we need to update data and also put it into linked list of variable */
	if ( strcasecmp("variable",command) == 0 )
		{
		/* strtok both format and line */
		tempFormat = strtok_r(hold_format," ",&hold_format);
		tempLine = strtok_r(hold_line," ",&hold_line);

		/* loop until format is empty */
		while ( tempFormat != NULL )
			{
			/* if format is not empty but line is empty , return 0 */
			if ( tempLine == NULL )
				{
				printf("Error - missing argument for this command\n");
				return 0;
				}
			bName = 0;
			/* if the first character of format is '$', then we know that need to store line as argument */
			if ( tempFormat[0] == '$' )
				{
				/* in variable name, it cannot be a duplicate name, cannot begin with a number and uppercase */
				if ( strcasecmp("$varName",tempFormat) == 0 )
					{
					tempVar = NULL;
					tempVar = searchWord(tempLine);
					if ( tempVar != NULL )
						{
						printf("Error - this variable is existing\n");
						return 0;
						}
					if ( isdigit(tempLine[0]) )
						{
						printf("Error - cannot begin a variable name with a digit.\n");
						return 0;
						}
					else if ( isupper(tempLine[0]) )
						{
						printf("Error - cannot begin a variable name with an uppercase\n");
						return 0;
						}
					strcpy(data->varName,tempLine);
					}
				/* in variable type, now we support noly int / char / double / string */
				else if ( strcasecmp("$varType",tempFormat) == 0 )
					{
					if ( strcasecmp(tempLine,"int") == 0 )
						{						
						strcpy(data->varType,tempLine);
						}
					else if ( strcasecmp(tempLine,"char") == 0 )
						{
						strcpy(data->varType,tempLine);
						}
					else if ( strcasecmp(tempLine,"double") == 0 )
						{
						strcpy(data->varType,tempLine);
						}
					else if ( strcasecmp(tempLine,"string") == 0 )
						{
						strcpy(data->varType,tempLine);
						}
					else
						{
						printf("Error - type of variable now support [ int / char / string / double ]\n");
						return 0;
						}
					}
				}
			/* if it doesn't begin with a dollar sign, we know it is some keyword and line must be matched */
			else
				{
				if ( strcasecmp(tempFormat,tempLine) != 0 )
					{
					printf("Error - incorrect format\n");
					return 0;
					}
				}
			/* pop the next token of format and line */
			tempFormat = strtok_r(NULL," ",&hold_format);
			tempLine = strtok_r(NULL," ",&hold_line);
			}
		
		addVariable(data->varName,data->varType);		/* put a variable into linked list */
		/* if it's a string, we need to assign size for declaring */
		if ( strcasecmp(data->varType,"string") == 0 )
			{
			strcpy(data->varType,"char");
			strcat(data->varName,"[256]");
			}
		}

	/* if command is open, we need to update data and put it into linked list of file */
	else if ( strcasecmp("open",command) == 0 )
		{
		/* strtok first token of format and line */
		tempFormat = strtok_r(hold_format," ",&hold_format);
		tempLine = strtok_r(hold_line," ",&hold_line);

		/* loop until format is empty */
		while ( tempFormat != NULL )
			{
			/* if format doesn't empty but line do, return 0 */
			if ( tempLine == NULL )
				{
				printf("Error - missing argument for this command\n");
				return 0;
				}
			tempFile = NULL;

			/* if format begins with a dollar sign , we need to store it as an argument */
			if ( tempFormat[0] == '$' )
				{
				/* store the name of file into filePath */
				if ( strcasecmp("$filePath",tempFormat) == 0 )
					{
					tempFile = searchFile(tempLine);

					/* if there is a file that has the same name opening already, return 0 */
					if ( tempFile != NULL )
						{
						printf("Error - this file is opening\n");
						return 0;
						}
					strcpy(data->filePath,tempLine);
					}
				/* store mode of file into data, now support read / wrtie */
				else if ( strcasecmp("$fileMode",tempFormat) == 0 )
					{
					if ( strcasecmp(tempLine,"read") == 0 )
						{
						strcpy(data->fileMode,"r");
						}
					else if ( strcasecmp(tempLine,"write") == 0 )
						{
						strcpy(data->fileMode,"w");
						}
					else
						{
						printf("Error - mode of file now support [ read / write ]\n");
						return 0;
						}
					}
				}
			/* if format doesn't begin with a dollar sign, we know it is some keyword that line must be matched */
			else
				{
				if ( strcasecmp(tempFormat,tempLine) != 0 )
					{
					printf("Error - incorrect format\n");
					return 0;
					}
				}
			/* pop the next token of format and line */
			tempFormat = strtok_r(NULL," ",&hold_format);
			tempLine = strtok_r(NULL," ",&hold_line);
			}
		/* add it into a linked list of file */
		addFile(data->filePath,data->fileMode,data->filePointer);
		}

	/* in the others command, we don't need to add anything into linked list */
	else
		{
		
		tempFormat = strtok_r(hold_format," ",&hold_format);		/* pop the first token of format */

		/* if format is not a condition, we will pop the first token of line */
		/* if it's a condition we need to strtok it in several times because we need to collect all components of condition */
		if ( strcasecmp(tempFormat,"$condition") != 0 )
			{
			tempLine = strtok_r(hold_line," ",&hold_line);

			/* if the first character of tempLine is a quote but in the end is not, 
			we need to check that Is it a string that has a space between itself? */
			if ( (tempLine[0] == '\"' && tempLine[strlen(tempLine)-1] != '\"') || (tempLine[0] == '\"' && strlen(tempLine) == 1) )
				{
				memset(tempString,0,sizeof(tempString));
				strcat(tempString,tempLine);
				if ( strstr(hold_line,"\"") == NULL )
					{
					printf("Error - condition is invalid\n");
					return 0;
					}
				tempLine = strtok_r(NULL,"\"",&hold_line);
				strcat(tempString," ");
				if ( tempLine != NULL )
					{
					strcat(tempString,tempLine);
					}
				strcat(tempString,"\"");
				tempLine = strdup(tempString);
				bFree = 1;
				memset(tempString,0,sizeof(tempString));
				}
			}
		
		/* loop until format is empty */
		while ( tempFormat != NULL )
			{
			/* if format is not condition but line is nothing, we return 0 */
			if ( strcasecmp(tempFormat,"$condition") != 0 && (tempLine == NULL ) )
				{
				printf("Error - missing argument for this command\n");
				return 0;
				}

			/* if format begins with a dollar sign, we need to store it as an argument */
			if ( tempFormat[0] == '$' )
				{
				/* if format is condition, we need to collect all of condition and send it to check that it is valid */
				if ( strcasecmp("$condition",tempFormat) == 0 )
					{
					memset(lineCondition,0,sizeof(lineCondition));
					tempLine = strtok_r(NULL," ",&hold_line);
					/* if the first token is nothing, return 0 */
					if ( tempLine == NULL )
						{
						printf("Error - missing argument for this command\n");
						return 0;
						}				
					/* strtok until line is empty */
					while ( tempLine != NULL )
						{
						/* if it's a string that has a space between itself */
						if ( tempLine[0] == '\"' && tempLine[strlen(tempLine)-1] != '\"')
							{
							strcat(tempString,tempLine);
							if ( strstr(hold_line,"\"") == NULL )
								{	
								printf("Error - condition is invalid\n");
								if ( bFree == 1 )
									{
									free(tempLine);	
									}
								return 0;
								}
							tempLine = strtok_r(NULL,"\"",&hold_line);
							if ( tempLine == NULL )
								{
								printf("Error - condition is invalid\n");
								if ( bFree == 1 )
									{
									free(tempLine);
									}
								return 0;
								}
							if ( tempLine != NULL )	
								{
								strcat(tempString," ");
								strcat(tempString,tempLine);
								strcat(tempString,"\"");
								}
							strcat(lineCondition,tempString);
							memset(tempString,0,sizeof(tempString));
							}
						/* if not, we put it into lineCondition suddenly */
						else
							{
							strcat(lineCondition,tempLine);
							}
						tempLine = strtok_r(NULL," ",&hold_line);
						if ( tempLine != NULL )
							{
							strcat(lineCondition," ");
							}
						}
					
					/* send lineCondition to function for condition checking */
					bCondition = 0;
					bCondition = checkCondition(lineCondition,data->condition);
					
					if ( bCondition != 1 )
						{
						printf("Error - condition is invalid\n");
						if ( bFree == 1 )
							{
							free(tempLine);
							}
						return 0;
						}
					}
				/* if format is a variable name, we put it for checking first */
				else if ( strcasecmp("$varName",tempFormat) == 0 )
					{			
					bName = 0;
					bName = checkName(tempLine,command,tempRule->varType);

					if ( bName != 1 )
						{
						if ( bFree == 1 )
							{
							free(tempLine);
							}
						return 0;
						}
					strcpy(data->varName,tempLine);

					tempVar = NULL;
					tempVar = searchWord(data->varName);
					/* if it's not a variable in linked list, we know it is a string that covered by two quotes */
					if ( tempVar == NULL )
						{
						strcpy(data->varSymbol,"%s");
						}
					/* if we found it in a linked list of variable, we catch its type for setting its symbol */
					else
						{
						strcpy(data->varType,tempVar->type);
						if ( strcasecmp(data->varType,"int") == 0 )
							{
							strcpy(data->varSymbol,"%d");
							}
						else if ( strcasecmp(data->varType,"char") == 0 )
							{
							strcpy(data->varSymbol,"%c");
							}
						else if ( strcasecmp(data->varType,"double") == 0 )
							{
							strcpy(data->varSymbol,"%lf");
							}
						else
							{
							strcpy(data->varSymbol,"%s");
							}
						}
					/* just add the last condition of for loop into data (i++) */
					strcat(data->increm,data->varName);
					strcat(data->increm,"++");			
					}
				/* if it's a value, we need to check that what is the command require */
				else if ( strcasecmp("$value",tempFormat) == 0 )
					{
					tempVar = searchWord(tempLine);

					/* if value is not a variable it must be matched with some case of command */
					if ( tempVar == NULL )
						{
						/* in SETSTRING, it must covered by two quotes */
						if ( strcasecmp(command,"SETSTRING") == 0 )
							{
							if ( tempLine[0] != '\"' || tempLine[strlen(tempLine)-1] != '\"' )
								{
								printf("Error - variable or string not found\n");
								if ( bFree == 1 )
									{
									free(tempLine);
									}
								return 0;
								}
							}
						/* if variable type require int or double, we need to check it is an operation in Maths */
						else if ( strcasecmp(data->varType,"int") == 0 || strcasecmp(data->varType,"double") == 0 )
							{
							if ( checkMath(tempLine,strlen(tempLine)) == 0 )
								{
								printf("Error - this value is invalid\n");
								if ( bFree == 1 )
									{
									free(tempLine);
									}
								return 0;
								}
							}
						/* if variable type require char,value must have length=3 and covered by single 2 quotes */
						else if ( strcasecmp(data->varType,"char") == 0 )
							{
							if ( strlen(tempLine) != 3 )
								{
								printf("Error - this value must be one character and covered by two single quotes\n");
								if ( bFree == 1 )
									{
									free(tempLine);
									}
								return 0;
								}
							if ( tempLine[0] != '\'' || tempLine[strlen(tempLine)-1] != '\'' )
								{
								printf("Error - this value must be one character and covered by two single quotes\n");
								if ( bFree == 1 )
									{
									free(tempLine);
									}
								return 0;
								} 
								
							}
						}
					/* if value is a variable, we need to check that its type match with varType */
					else
						{
						/* in double and int we can use it together but char and string cannot */
						if ( strcasecmp(data->varType,"char") == 0 || strcasecmp(data->varType,"string") == 0 || strcasecmp(tempVar->type,"char") == 0 || strcasecmp(tempVar->type,"string") == 0 )
							{
							if ( strcasecmp(tempVar->type,data->varType) != 0 )
								{
								printf("Error - value type is not supported\n");
								if ( bFree == 1 )
									{
									free(tempLine);
									}
								return 0;
								}
							}
						}
					strcpy(data->value,tempLine);						
					}
				/* if command is filePath, we need to store the name of file as an argument */
				else if ( strcasecmp("$filePath",tempFormat) == 0 )
					{
					tempFile = NULL;
					tempFile = searchFile(tempLine);

					/* in this case, we will not found 'open' command,
					so for using the others command, the file must be declared first */
					if ( tempFile == NULL )
						{
						printf("Error - file is undeclared or deleted\n");
						if ( bFree == 1 )
							{
							free(tempLine);
							}
						return 0;
						}
					/* if the file is opened, check that it's in the correct mode */
					else
						{
						if ( strcasecmp(command,"read") == 0 )
							{
							if ( strcasecmp(tempFile->mode,"r") != 0 )
								{
								printf("Error - this file is not in the read mode\n");
								if ( bFree == 1 )
									{
									free(tempLine);
									}
								return 0;
								}
							}
						else if ( strcasecmp(command,"write") == 0 )
							{
							if ( strcasecmp(tempFile->mode,"w") != 0 )
								{
								printf("Error - this file is not in the write mode\n");
								if ( bFree == 1 )
									{
									free(tempLine);
									}
								return 0;
								}
							}
						else if ( strcasecmp(command,"getline") == 0 )
							{
							if ( strcasecmp(tempFile->mode,"r") != 0 )
								{
								printf("Error - this file is not in the read mode\n");
								if ( bFree == 1 )
									{
									free(tempLine);
									}
								return 0;
								}
							}
						/* if command is close, we don't need to check anything, just delete it */
						else if ( strcasecmp(command,"close") == 0 )
							{
							strcpy(data->fileMode,tempFile->mode);
							strcpy(data->filePointer,tempFile->handle);
							deleteFile(tempLine);
							}
						}
					
					/* store filePath and filePointer into data */
					if ( strcasecmp(command,"close") != 0 )
						{
						strcpy(data->filePath,tempLine);
						strcpy(data->filePointer,tempFile->handle);
						}
					}
				}
			/* if format doesn't begin with a dollar sign, we know it is some keyword and line must be matched */
			else
				{
				if ( strcasecmp(tempFormat,tempLine) != 0 )
					{
					if ( bFree == 1 )
						{
						free(tempLine);
						}
					return 0;
					}
				}
			/* pop the next token of format */
			tempFormat = strtok_r(NULL," ",&hold_format);
			/* if it still has format and it's not condition, we strtok it as tempLine suddenly */
			if ( tempFormat != NULL )
				{
				if ( strcmp(tempFormat,"$condition") != 0 )
					{
					tempLine = strtok_r(NULL," ",&hold_line);
					if ( tempLine != NULL )
						{
						/* just for strtok string that has a space between itself */
						if ( tempLine[0] == '\"' )
							{
							if ( strlen(hold_line) != 0 )
								{
								strcat(tempString,tempLine);
								strcat(tempString," ");
								tempLine = strtok_r(NULL,"\"",&hold_line);
								if ( tempLine != NULL )
									{
									strcat(tempString,tempLine);
									}
								strcat(tempString,"\"");
								tempLine = strdup(tempString);
								bFree = 1;
								}
							}
						}
					}
				}
			}
		}
	/* if format is empty but line is not, return 0 */
	tempLine = strtok_r(NULL," ",&hold_line);
	if ( tempLine != NULL )
		{
		printf("Error - incorrect format\n");
		if ( bFree == 1 )
			{
			free(tempLine);
			}
		return 0;
		}
	/* make varType be a lowercase */
	for ( i=0;i<strlen(data->varType);i++ )
		{
		data->varType[i] = tolower(data->varType[i]);
		}
	if ( bFree == 1 )
		{
		free(tempLine);
		}
	return 1;
	}
