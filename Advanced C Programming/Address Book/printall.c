/*

NAME : 
DATE :
DESCRIPTION :
SAMPLE INPUT :
SAMPLE OUTPUT :
 
 */

#include"common.h"

void print_all(BookInfo *filedata)
{
    printf("\n         ADDRESS BOOK : %d Contacts", filedata->size);

    printf("\n      ----------------------------------------------------"
	    "-------------------------------------------\n");
    rewind(filedata->fptr);
    char ch;
    while(fread(&ch,1,1,filedata->fptr) > 0)
    {
	//printf("%c",ch);
	if (ch == '\n')
	    break;
    }
    printf(
	    "%8s %-24s %16s %24s %24s\n",
	    " ",
	    "Name",
	    "Mobile No.",
	    "Email id",
	    "Location"
	    );

    printf(
	    "%8s %-24s %16s %24s %24s",
	    " ",
	    "=======",
	    "===========",
	    "==========",
	    "========="
	  );
    //printf(" %d ", filedata->size);
    for(int i = 0; i < (filedata->size); i++)
    {

        ContactInfo contact;
	int pos = 0;
	while(fread(&ch,1,1,filedata->fptr) > 0)
	{
	    if(ch == ',' || ch == '\n')
	    {
		contact.name[pos++] = '\0';
		break;
	    }
	    contact.name[pos++] = ch;
	}
	printf("\n%6d  ",i+1);

	printf(" %-24s", contact.name);

	if (ch == '\n')
	    continue;
	pos = 0;
	contact.mob[0] = '\0';
	while(fread(&ch,1,1,filedata->fptr) > 0)
	{
	    if(ch == ',' || ch == '\n')
	    {
		contact.mob[pos++] = '\0';
		break;
	    }
	    contact.mob[pos++] = ch;
	}
	printf(" %16s", contact.mob);
	if (ch == '\n')
	    continue;
	
	pos = 0;
	contact.email[0] = '\0';
	while(fread(&ch,1,1,filedata->fptr) > 0)
	{
	    if(ch == ',' || ch == '\n')
	    {
		contact.email[pos++] = '\0';
		break;
	    }
	    contact.email[pos++] = ch;
	}
	printf(" %24s", contact.email);
	if (ch == '\n')
	    continue;

	pos = 0;
	contact.location[0] = '\0';
	while(fread(&ch,1,1,filedata->fptr) > 0)
	{
	    if(ch == '\n'|| ch == ',')
	    {
		contact.location[pos++] = '\0';
		break;
	    }
	    contact.location[pos++] = ch;
	}
	printf(" %24s", contact.location);
	if (ch == '\n')
	    continue;
	else
	{
	    while(fread(&ch , 1, 1, filedata ->fptr) > 0)
	    {
		if(ch == '\n')
		{
		    break;
		}
	    }
	}
    }
    printf("\n      ----------------------------------------------------"
	    "-------------------------------------------\n");
}
