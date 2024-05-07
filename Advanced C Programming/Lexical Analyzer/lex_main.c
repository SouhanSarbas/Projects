/*

NAME : 
DATE :
DESCRIPTION :
SAMPLE INPUT :
SAMPLE OUTPUT :

 */

#include "common.h"


int main(int argc, char *argv[])
{

    /*verify CLA is C file*/

    if(argc < 2)
    {
	printf( "ERROR: pass C-file name as Command Line Argument\n"
		"Usage: %s sample.c\n", argv[0]);
	return 1;
    }

    char *extn;
    char *fname = argv[1];
    if((extn = strstr(fname, ".")) == NULL)
    {
	printf("ERROR: use .C extention for the file name\n");
	return 1;
    }

    //printf("extn   %s\n", extn);
    if((strcmp(extn, ".c") != 0) && (strcmp(extn, ".C")) != 0)
    {
	printf("ERROR: file name must be with .c extension\n");
	return 1;
    }

    State state = idle;

    /*open file*/
    FILE *fptr;

    fptr = fopen(fname,"r");
    fseek(fptr,0,SEEK_END);
    int last = ftell(fptr);
    rewind(fptr);

    /*read c file*/
    printf( "----------------------------------------\n"
	    "     TOKEN TYPE      : LEXEME\n"
	    "----------------------------------------\n");
    while(ftell(fptr) < last)
    {	
	int i = -1, ret;

	char *data = calloc(100,1);// calloc to have zeroed array
	if(data == NULL)
	{
	    printf("Memory allocation failed\n");
	    return 0;
	}
	ret = fscanf(fptr, " %[^\n]", data);
	printf("\n");

	/*	printf("\n=====================================================================\n"
		"line:\n %s\n---------------------------------------------------------------\n", data);
	 */
	while(ret > 0 && data[++i] != '\0')
	{
	    //check comment start
	    if(state == idle && data[i] == '/')
	    {
		if(data[i+1] == '/')
		{
		    state = comment_s;
		    printf("\nSingle Line Comment : %s ", &data[i]);
		    state = next_line;
		}
		else if(data[i+1] == '*')
		{
		    i++;
		    i++;
		    state = comment_m;
		    printf("\nMulti-Line Comment   : \n/*");

		}

	    }

	    //check comment ending
	    if(state == comment_m)
	    {
		if(data[i] == '*')
		{
		    if(data[i+1] == '/')
		    {
			state = idle;
			printf("%c%c\n",data[i],data[i+1]);
			i++;
			i++;
		    }
		    else{
			printf("%c",data[i]);
		    }
		}
		else{
		    printf("%c",data[i]);
		}
	    }

	    //check macrons and headers
	    if(state == idle && data[i] == '#')
	    {
		state = preproc;
		printf("\nPreprocessor directive : %s ", &data[i]);
		state = next_line;
	    }

	    //to goto next line
	    if(state == next_line)
	    {
		state = idle;
		break;
	    }

	    //check for string start
	    if(state == idle && data[i] == '"')
	    {
		state = string;
		printf("\nString                 : \"");
		i++;
	    }

	    //check for string end
	    if(state == string)
	    {
		if( data[i] == '"')
		{
		    if(data[i - 1] == '\\')
		    {
			printf("\"");

		    }
		    else
		    {
			printf("\"");
			state = idle;

		    }
		}
		else
		{
		    printf("%c", data[i]);
		}

	    }

	    //check for numbers
	    if(state == idle && (data[i] >= '0' && data[i] <= '9'))
	    {
		state = numeric;
		if (data[i] == '0')
		{
		    char ch1, ch = data[i+1];

		    // hexadecimals
		    if (data[i+1] == 'X' || data[i+1] == 'x')
		    {
			i++;
			i++;
			state = hexa; 
			printf("\nHexadecimal number  : 0%c",data[i-1]);
			while((data[i] >= '0' && data[i] <='9' ) || (data[i] >= 'a' && data[i] <= 'f') ||(data[i] >= 'A' && data[i] <= 'F'))
			{
			    printf("%c", data[i]);
			    i++;
			}


			if(data[i] == 'u' || data[i] == 'U')
			{
			    printf("[%c -> unsigned] ",data[i]);
			    i++;
			}
			if(data[i] == 'L' || data[i] == 'l')
			{
			    //20L

			    if(data[i+1] == 'L' || data[i+1] == 'l')
			    {
				//20LL
				i++;
				i++;
				printf("[%c%c -> Long long int literal] ",data[i-2],data[i-1]);
				state = numend;

			    }
			    else
			    {
				i++;
				printf("[%c -> Long int literal] ",data[i-1]);
				state = numend;
			    }
			}
			else 
			{
			    state = numend;
			}


		    }
		    // binary number
		    else if (data[i+1] == 'b' || data[i+1] == 'B')
		    {
			i++;
			state = bina;
			i++;
			printf("\nBinary number       : 0%c",data[i-1]);
			while(data[i] == '0' || data[i] == '1')
			{
			    printf("%c", data[i]);
			    i++;
			}
			if(data[i] == 'u' || data[i] == 'U')
			{
			    printf("[%c -> unsigned] ",data[i]);
			    i++;
			}

			if(data[i] == 'L' || data[i] == 'l')
			{
			    //20L

			    if(data[i+1] == 'L' || data[i+1] == 'l')
			    {
				//20LL
				i++;
				i++;
				printf("[%c%c -> Long long int literal] " ,data[i-2],data[i-1]);
				state = numend;

			    }
			    else
			    {
				i++;
				printf("[%c -> Long int literal] ",data[i-1]);
				state = numend;
			    }
			}
			else 
			{
			    state = numend;
			} 


		    }
		    // octal number
		    else if (ch >='0' && ch <='9')
		    {
			state = octal;
			i++;
			printf("\nOctal number        : 0");
			while(data[i] >= '0' && data[i] <= '7')
			{
			    printf("%c", data[i]);
			    i++;
			}
			if(data[i] == 'u' || data[i] == 'U')
			{
			    printf("[%c -> unsigned] ",data[i]);
			    i++;
			}

			if(data[i] == 'L' || data[i] == 'l')
			{
			    //20L

			    if(data[i+1] == 'L' || data[i+1] == 'l')
			    {
				//20LL
				i++;
				i++;
				printf("[%c%c -> Long long int literal] ", data[i-2],data[i-1]);
				state = numend;

			    }
			    else
			    {
				i++;
				printf("[%c -> Long int literal] ", data[i-1]);
				state = numend;
			    }
			}

			if(state == octal && (data[i] == '8' || data[i] == '9'))
			{
			    state = error;
			}

			else 
			{
			    state = numend;
			} 

		    }			

		}
	    }


	    //determine int or float numbers
	    if(state == numeric)
	    {
		int n,d,s;
		char dot;
		s=sscanf(&data[i],"%d%c%d",&n,&dot,&d);
		if(s == 3 && dot == '.')
		{
		    printf("\nFloat Const         : ");
		}
		else
		{
		    printf("\nInteger const       : ");
		}

		while(data[i] >= '0' && data[i] <= '9')
		{
		    printf("%c",data[i]);
		    i++;
		}

		if(data[i] == '.' && data[i+1]>='0' && data[i+1] <= '9')
		{
		    printf(".");
		    i++;
		    while(data[i] >= '0' && data[i] <= '9')
		    {
			printf("%c",data[i]);
			i++;
		    }
		    if(data[i] == 'f' || data[i] == 'F')
		    {
			printf("[%c -> float literal]", data[i]);
			i++;
		    }  

		    state = numend;
		}

		if(state == numeric && ( data[i] == 'u' || data[i] == 'U'))
		{
		    printf("[%c -> unsigned]",data[i]);
		    i++;
		}

		// 20L 20LL
		if(state == numeric && (data[i] == 'L' || data[i] == 'l'))
		{
		    //20L

		    if(data[i+1] == 'L' || data[i+1] == 'l')
		    {
			//20LL
			i++;
			i++;
			printf("[%c%c -> Long long int literal] ",data[i-2],data[i-1]);
			state = numend;

		    }
		    else
		    {
			i++;
			printf("[%c -> Long int literal] ", data[i-1]);
			state = numend;
		    }
		}
		else 
		{
		    state = numend;
		}
	    }

	    //check valid number ending
	    if(state == numend)
	    {
		int k = 0;
		//printf("\nDEBUG361 (%c) \n",data[i]);
		while(k < 20)
		{
		    if(data[i] == opers_R[k])
		    {
			state = idle;
			break;
		    }
		    k++;
		    // printf("%c == %c ??\n",data[i+1],opers_R[k]);
		}
		if(k == 20)
		{
		    printf("%c", data[i]);
		    state = error;
		}


	    }

	    //ERROR handling
	    if(state == error)
	    {
		printf("\nERROR : invalid number\n");
		return 0;

	    }

	    //check for identifier start
	    if((state == idle) && ((data[i] >= 'a' && data[i] <= 'z') ||( data[i] >= 'A' && data[i] <= 'Z') || (data[i]) == '_' ))
	    {
		char word[30];
		int j = -1;
		state = identifier;
		int k = i;

		//check for keywords
		sscanf(&data[i], "%[a-z]", word);
		while(++j < 32)
		{
		    if(strcmp(word, keywords[j]) == 0)
		    {
			state = key;
			
			while(data[i] >= 'a' && data[i] <= 'z')
			{
			    i++;
			}
			if (strchr("(;{ \n", data[i]) != NULL)
			{
			printf("\nKeyword             : %s", word);
			state = idle;
			break;
			}
			else
			{
			    i = k;
			    break;
			}
		    }
		}

	    }

	    //check identifier ending
	    if(state == identifier)
	    {
		printf("\nIdentifier          : ");
		while((data[i] >= 'a' && data[i] <= 'z') || ( data[i] >= 'A' && data[i] <= 'Z') || (data[i] == '_' ) || (data[i] >= '0' && data[i] <= '9'))
		{
		    printf("%c", data[i]);
		    i++;
		}
		state = idle;
		i--;
	    }

	    if(state == idle)
	    {
		char ch = data[i];

		//check for operators
		if(ch == '+' || ch == '*' || ch == '/' || 
			ch == '-' || ch == '=' || ch == '<' ||
			ch == '>' || ch == '|' || ch == '&' || 
			ch == '!' || ch == '~' || ch == '^' || 
			ch == '%' || ch == '?' || ch == ':')
		{
		    printf("\nOperator            : %c ", ch);

		}

		//check for special characters
		if(ch == ';' || ch == '{' || ch == '('||
			ch == '[' || ch == ']' || ch == ')'||
			ch == '}' || ch == ',')
		{
		    printf("\nSpecial character   : %c ", ch);
		}

		//check for character constant
		if (ch == '\'')
		{
		    if (data[i+2] == '\'')
		    {
			printf("\nCharacter constant  : \'%c\'", data[i+1]);
			i++;
			i++;

		    }
		}


	    }



	}
	// free data array memory
	free(data);
    }
    printf("\n");
    return 0;
}



