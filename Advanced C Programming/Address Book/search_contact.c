/*

NAME : 
DATE :
DESCRIPTION :
SAMPLE INPUT :
SAMPLE OUTPUT :

 */
#include "common.h"
#include <ctype.h>

char *strcasestr(const char *haystack, const char *needle) {
    int c = tolower((unsigned char)*needle);
    if (c == '\0')
	return (char *)haystack;
    for (; *haystack; haystack++) {
	if (tolower((unsigned char)*haystack) == c) {
	    for (size_t i = 0;;) {
		if (needle[++i] == '\0')
		    return (char *)haystack;
		if (tolower((unsigned char)haystack[i]) != tolower((unsigned char)needle[i]))
		    break;
	    }
	}
    }
    return NULL;
}
void search_contact(BookInfo *filedata)
{
    printf( 
	    "\n============================\n"
	    "  Search Contact\n"
	    "----------------------------\n"
	    " 1. Search by Name\n"
	    " 2. Search by Mobile No.\n"
	    " 3. Search by Email\n"
	    " 4. Search by Location\n"
	    " 5. Exit\n"
	    "============================\n"
	  );

    char ch;
    printf("Choose the searching method\n");
    scanf(" %c", &ch);
    switch(ch)
    {
	case '1':
	    {
		char str[24];
		printf("Enter the name to search : ");
		scanf(" %[^\n]", str);
		search_by_name(filedata, str);
	    }
	    break;
	case '2':
	    search_by_mob(filedata);
	    break;
	case '3':
	    search_by_email(filedata);
	    break;
	case '4':
	    search_by_loc(filedata);
	    break;
	case '5':
	    return;
	    break;
	default:
	    printf("Invalid option\n");
    }
}

int search_by_name(BookInfo *filedata, char *str)
{
    int flag = 0;


    rewind(filedata->fptr);
    char ch;
    while(fread(&ch,1,1,filedata->fptr) > 0)
    {
	//printf("%c",ch);
	if (ch == '\n')
	    break;
    }
    for(int i = 0; i < (filedata->size); i++)
    {

	ContactInfo contact;
	int pos = 0;
	while(fread(&ch,1,1,filedata->fptr) > 0)
	{
	    if(ch == ',')
	    {
		contact.name[pos++] = '\0';
		break;
	    }
	    contact.name[pos++] = ch;
	}


	if(strcasestr(contact.name, str) == NULL)
	{
	    while(fread(&ch,1,1,filedata->fptr) > 0)
	    {
		if(ch == '\n')
		    break;
	    }
	    continue;
	}
	else
	    flag++;


	printf("\n%6d  ",i+1);

	printf(" %-24s", contact.name);

	if (ch == '\n')
	    continue;
	pos = 0;
	contact.mob[0] = '\0';
	while(fread(&ch,1,1,filedata->fptr) > 0)
	{
	    if(ch == ',')
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
	    if(ch == ',')
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
    }
    if(flag == 0)
    {
	printf("Match not found for '%s'", str);
    }
    printf("\n");
    return flag;

}








int search_by_mob(BookInfo *filedata)
{
    char str[24];
    int flag = 0;
    printf("Enter the Mobile No. to search : ");
    scanf(" %s", str);


    rewind(filedata->fptr);
    char ch;
    while(fread(&ch,1,1,filedata->fptr) > 0)
    {
	//printf("%c",ch);
	if (ch == '\n')
	    break;
    }
    for(int i = 0; i < (filedata->size); i++)
    {

	ContactInfo contact;
	int pos = 0;
	while(fread(&ch,1,1,filedata->fptr) > 0)
	{
	    if(ch == ',')
	    {
		contact.name[pos++] = '\0';
		break;
	    }
	    contact.name[pos++] = ch;
	}



	if (ch == '\n')
	    continue;
	pos = 0;
	contact.mob[0] = '\0';
	while(fread(&ch,1,1,filedata->fptr) > 0)
	{
	    if(ch == ',')
	    {
		contact.mob[pos++] = '\0';
		break;
	    }
	    contact.mob[pos++] = ch;
	}
	if(strcasestr(contact.mob, str) == NULL)
	{
	    while(fread(&ch,1,1,filedata->fptr) > 0)
	    {
		if(ch == '\n')
		    break;
	    }
	    continue;
	}
	else
	    flag++;
	printf("\n%6d  ",i+1);
	printf(" %-24s", contact.name);
	printf(" %16s", contact.mob);
	if (ch == '\n')
	    continue;

	pos = 0;
	contact.email[0] = '\0';
	while(fread(&ch,1,1,filedata->fptr) > 0)
	{
	    if(ch == ',')
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
    }
    if(flag == 0)
	printf("Match not found for '%s'", str);
    printf("\n");
    return flag;

}

int search_by_email(BookInfo *filedata)
{    char str[24];
    int flag = 0;
    printf("Enter the Email id to search : ");
    scanf(" %[^\n]", str);


    rewind(filedata->fptr);
    char ch;
    while(fread(&ch,1,1,filedata->fptr) > 0)
    {
	//printf("%c",ch);
	if (ch == '\n')
	    break;
    }
    for(int i = 0; i < (filedata->size); i++)
    {

	ContactInfo contact;
	int pos = 0;
	while(fread(&ch,1,1,filedata->fptr) > 0)
	{
	    if(ch == ',')
	    {
		contact.name[pos++] = '\0';
		break;
	    }
	    contact.name[pos++] = ch;
	}



	if (ch == '\n')
	    continue;
	pos = 0;
	contact.mob[0] = '\0';
	while(fread(&ch,1,1,filedata->fptr) > 0)
	{
	    if(ch == ',')
	    {
		contact.mob[pos++] = '\0';
		break;
	    }
	    contact.mob[pos++] = ch;
	}
	if (ch == '\n')
	    continue;

	pos = 0;
	contact.email[0] = '\0';
	while(fread(&ch,1,1,filedata->fptr) > 0)
	{
	    if(ch == ',')
	    {
		contact.email[pos++] = '\0';
		break;
	    }
	    contact.email[pos++] = ch;
	}
	if(strcasestr(contact.email, str) == NULL)
	{
	    while(fread(&ch,1,1,filedata->fptr) > 0)
	    {
		if(ch == '\n')
		    break;
	    }
	    continue;
	}
	else
	    flag++;
	printf("\n%6d  ",i+1);
	printf(" %-24s", contact.name);
	printf(" %16s", contact.mob);
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
    }
    if(flag == 0)
	printf("Match not found for '%s'", str);
    printf("\n");
    return flag;

}

int search_by_loc(BookInfo *filedata)
{
    char str[24];
    int flag = 0;
    printf("Enter the Location to search : ");
    scanf(" %[^\n]", str);


    rewind(filedata->fptr);
    char ch;
    while(fread(&ch,1,1,filedata->fptr) > 0)
    {
	//printf("%c",ch);
	if (ch == '\n')
	    break;
    }
    for(int i = 0; i < (filedata->size); i++)
    {

	ContactInfo contact;
	int pos = 0;
	while(fread(&ch,1,1,filedata->fptr) > 0)
	{
	    if(ch == ',')
	    {
		contact.name[pos++] = '\0';
		break;
	    }
	    contact.name[pos++] = ch;
	}



	if (ch == '\n')
	    continue;
	pos = 0;
	contact.mob[0] = '\0';
	while(fread(&ch,1,1,filedata->fptr) > 0)
	{
	    if(ch == ',')
	    {
		contact.mob[pos++] = '\0';
		break;
	    }
	    contact.mob[pos++] = ch;
	}
	if (ch == '\n')
	    continue;

	pos = 0;
	contact.email[0] = '\0';
	while(fread(&ch,1,1,filedata->fptr) > 0)
	{
	    if(ch == ',')
	    {
		contact.email[pos++] = '\0';
		break;
	    }
	    contact.email[pos++] = ch;
	}
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
	if(strcasestr(contact.location, str) == NULL)
	{
	    while(fread(&ch,1,1,filedata->fptr) > 0)
	    {
		if(ch == '\n')
		    break;
	    }
	    continue;
	}
	else
	    flag++;
	printf("\n%6d  ",i+1);
	printf(" %-24s", contact.name);
	printf(" %16s", contact.mob);
	printf(" %24s", contact.email);
	printf(" %24s", contact.location);
    }
    if(flag == 0)
	printf("Match not found for '%s'", str);
    printf("\n");
    return flag;

}





