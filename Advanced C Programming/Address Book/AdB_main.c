/*

NAME : SOUHAN SARBAS C H
DATE : 20/4/2024
DESCRIPTION : ADDRESS BOOK using CSV file
SAMPLE INPUT :
SAMPLE OUTPUT :

 */


#include "common.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
	printf( "ERROR: pass CSV filename as Command Line Argument\n"
		"Usage: %s addressbook.csv\n", argv[0]);
	return 1;
    }
    BookInfo filedata;
    char ch;
    filedata.fname = argv[1];
    filedata.ftemp = "temp.csv";
    if (open_and_validate_csv(&filedata) == success)
    {
	fprintf(stdout,"Address Book opened\n");
	while(1)
	{
	    print_menu();
	    printf("Choose an option : ");
	    scanf(" %c", &ch);
	    switch(ch)
	    {
		case '1' : // Add contact
		    {
			if(add_contact(&filedata) == success)
			{
			    printf("Contact added\n");
			}
			else
			{
			    printf("Adding contact failed\n");
			}
		    }
		    break;
		case '2' :
		    search_contact(&filedata);
		    break;
		case '3' : // Edit contact
		    {
			if(edit_contact(&filedata) == success)
			{
			    printf("Contact edited successfully\n");
			}
			else
			{
			    printf("Contact editing failed\n");
			}
			break;
		    }
		case '4' : // delete contact
		    {
			if(delete_contact(&filedata) == success)
			{
			    printf("Contact deleted successfully\n");
			}
			else
			{
			    printf("Contact deleting failed\n");
			}
			break;
		    }
		case '5' : // print all
		    print_all(&filedata);
		    break;
		case '6' : // Exit
		    printf("Exiting...\n");
		    return success;
		    break;
		default:
		    printf("Warning : Invalid option\n");
	    }

	}
    }
    else
    {
	fprintf(stderr,"ERROR: Address book cannot be opened\n");
    }

    return 0;
}

Status open_and_validate_csv(BookInfo *filedata)
{
    char *extn;
    if((extn = strstr(filedata->fname, ".")) == NULL)
    {
	printf("ERROR: use CSV extention for the file name\n");
	return failure;
    }

    //printf("extn   %s\n", extn);
    if((strcmp(extn, ".csv") != 0) && (strcmp(extn, ".CSV")) != 0)
    {
	printf("ERROR: file name must be with .csv extension\n");
	return failure;
    }

    filedata->fptr = fopen(filedata->fname, "a+");
    if(filedata->fptr == NULL)
    {
	return failure;
    }
    if((filedata -> size = read_address_book_size(filedata)) == failure)
    {
	return failure;
    }
    //printf("size = %d\n", filedata -> size);
    return success;
}


int read_address_book_size(BookInfo *filedata)
{
    rewind(filedata->fptr);

    char ch;
    int pos = 0;
    char *cptr;

    if(fread(&ch, 1, 1, filedata->fptr) > 0)
    {
	if(ch == '#')
	{
	    while(fread(&ch, 1, 1, filedata->fptr) > 0)
	    {
		if(ch == '#')
		{
		    pos = ftell(filedata->fptr);
		    break;
		}
		else if (ch < '0' || ch > '9')
		{
		    fprintf(stderr,"ERROR: Address book size cannot be found\n");
		    return failure;
		}
	    }
	    //   printf("%d\n",pos);
	    if(pos > 2)
	    {
		cptr = malloc(pos-2);
		fseek(filedata->fptr, 1, SEEK_SET);
		if(fread(cptr, pos-2, 1, filedata->fptr) > 0)
		{
		    int size = convert_to_int(cptr, pos - 2);
		    rewind(filedata->fptr);
		    free(cptr);
		    return size;
		}
		else
		{
		    free(cptr);
		    fprintf(stderr,"ERROR: Address book size reading failed\n");
		}
	    }
	    else
	    {
		fprintf(stderr,"ERROR: Address book size missing\n");
	    }

	}
	else
	{
	    fprintf(stderr,"ERROR: Address book missing '#' in size format\n");
	}
    }
    else
    {
	fprintf(stderr,"ERROR: Empty file\n");
    }
    return failure;
}

int convert_to_int(char cptr[], int size)
{
    int sum = 0, temp = 0;
    for(int i = 0; i < size; i++)
    {
	sum = sum * 10;
	temp = cptr[i] - '0';
	sum = sum + temp;	
    }
    //printf("sum = %d\n", sum);
    return sum;
}

void print_menu(void)
{

    printf( "\n===========================\n"
	    "  Main menu\n"
	    "---------------------------\n"
	    "  1. Add Contact\n"
	    "  2. Search Contact\n"
	    "  3. Edit Contact\n"
	    "  4. Delete Contact\n"
	    "  5. Print all Contacts\n"
	    "  6. Exit\n"
	    "===========================\n");
}









