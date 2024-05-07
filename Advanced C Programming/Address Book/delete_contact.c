/*

NAME : 
DATE :
DESCRIPTION :
SAMPLE INPUT :
SAMPLE OUTPUT :

 */

#include"common.h"

Status delete_contact(BookInfo *filedata)
{
    printf( 
	    "\n==========================\n"
	    "Delete Contact\n"
	    "--------------------------\n"
	    "1. Search by Name\n"
	    "2. Search by Mobile No.\n"
	    "3. Search by Email\n"
	    "4. Search by Location\n"
	    "5. Exit\n"
	    "==========================\n"
	  );

    char ch;
    int n;
    printf("Choose the searching method\n");
    scanf(" %c", &ch);
    switch(ch)
    {
	case '1':
	    {
		char str[24];
		printf("Enter the name to search : ");
		scanf(" %[^\n]", str);
		if((search_by_name(filedata, str) > 0))
		{
		    printf("Enter the Sl.no number to delete :");
		    scanf(" %d", &n);
		    if(delete_SL(filedata, n) == success)
		    {
			return success;
		    }
		    else
			return failure;
		}
		else
		{
		    return failure;
		}

	    }
	    break;
	case '2':
	    {
		if(search_by_mob(filedata) > 0)
		{
		    printf("Enter the Sl.no number to delete :");
		    scanf(" %d", &n);
		    if(delete_SL(filedata, n) == success)
		    {
			return success;
		    }
		    else
			return failure;
		}
		else
		{
		    return failure;
		}
		break;
	    }
	case '3':
	    {
		if(search_by_email(filedata) > 0)
		{
		    printf("Enter the Sl.no number to delete :");
		    scanf(" %d", &n);
		    if(delete_SL(filedata, n) == success)
		    {
			return success;
		    }
		    else
			return failure;
		}
		else
		{
		    return failure;
		}
		break;
	    }
	case '4':
	    {
		if(search_by_loc(filedata) > 0)
		{
		    printf("Enter the Sl.no number to delete :");
		    scanf(" %d", &n);
		    if(delete_SL(filedata, n) == success)
		    {
			return success;
		    }
		    else
			return failure;
		}
		else
		{
		    return failure;
		}
		break;
	    }
	case '5':
	    return failure;
	    break;
	default:
	    printf("Invalid option\n");
    }
}

Status delete_SL(BookInfo *filedata, int n)
{
    char ch, str[24];
    int posL,posR, pos = 0;
    ContactInfo contact;
    rewind(filedata->fptr);
    for(int i = 0; i < n; i++)
    {
	while(fread(&ch, 1, 1, filedata->fptr) > 0)
	{
	    if(ch == '\n')
		break;
	}
    }
    posL = ftell(filedata->fptr);



    ////

    while(fread(&ch,1,1,filedata->fptr) > 0)
    {
	if(ch == ',')
	{
	    contact.name[pos++] = '\0';
	    break;
	}
	contact.name[pos++] = ch;
    }

    pos = 0;
    while(fread(&ch,1,1,filedata->fptr) > 0)
    {
	if(ch == ',')
	{
	    contact.mob[pos++] = '\0';
	    break;
	}
	contact.mob[pos++] = ch;
    }

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

    pos = 0;
    contact.location[0] = '\0';
    while(fread(&ch,1,1,filedata->fptr) > 0)
    {
	if(ch == '\n'|| ch == ',')
	{
	    contact.location[pos++] = '\0';
	    posR = ftell(filedata->fptr);
	    break;
	}
	contact.location[pos++] = ch;
    }
    ch = 'd';
    int i = 0,j = 0;
    int len = 0;
    printf("Delete the contact at ");
    printf("%6d  ?\n", n);
    printf("Name     : %24s\n", contact.name);
    printf("Mob No.  : %24s\n", contact.mob);
    printf("Email id : %24s\n", contact.email);
    printf("Location : %24s\n", contact.location);

    while(1)
    {
	printf("Confirm delete? (Y/N) : ");
	scanf(" %c", &ch);

	if(ch == 'Y' || ch == 'y')
	{

	    char *tempfname = filedata->ftemp;

	    FILE *tempfptr = fopen(tempfname, "w");
	    if(tempfptr == NULL)
	    {
		return failure;
	    }

	    rewind(filedata->fptr);
	    rewind(tempfptr);

	    //////////////////////////////////


	    (filedata->size)--;
	    char sizestr[8];
	    change_to_str(sizestr,filedata->size);
	    ch = '#';
	    //printf("2211 \n");
	    fwrite(&ch, 1, 1, tempfptr);
	    i=0;
	    while(sizestr[i] != '\0')
	    {
		ch = sizestr[i];
		fwrite(&ch, 1,1, tempfptr);
		i++;
	    }
	    ch = '#';
	    fwrite(&ch, 1, 1, tempfptr);

	    //printf("222111 \n");
	    while(fread(&ch,1,1,filedata->fptr) > 0)
	    {
		if(ch == '\n')
		    break;
	    }
	    fwrite(&ch, 1, 1, tempfptr);

	    ////////////////////////////////////////////

	    char ch1;
	    while(ftell(filedata->fptr) < posL)
	    {
		fread(&ch1,1,1,filedata->fptr);
		fwrite(&ch1,1,1,tempfptr);
	    }
	    /*
	       i=0;
	       while(contact.name[i] != '\0')
	       {
	       ch1 = contact.name[i];
	       fwrite(&ch1,1,1,tempfptr);
	       i++;
	       }

	       ch1 = ',';
	       fwrite(&ch1,1,1,tempfptr);

	       i=0;
	       while(contact.mob[i] != '\0')
	       {
	       ch1 = contact.mob[i];
	       fwrite(&ch1,1,1,tempfptr);
	       i++;
	       }

	       ch1 = ',';
	       fwrite(&ch1,1,1,tempfptr);

	       i=0;
	       while(contact.email[i] != '\0')
	       {
	       ch1 = contact.email[i];
	       fwrite(&ch1,1,1,tempfptr);
	       i++;
	       }

	       ch1 = ',';
	       fwrite(&ch1,1,1,tempfptr);

	       i=0;
	       while(contact.location[i] != '\0')
	       {
	       ch1 = contact.location[i];
	       fwrite(&ch1,1,1,tempfptr);
	       i++;
	       }
	     */

	    fseek(filedata->fptr, posR, SEEK_SET);

	    while(fread(&ch1,1,1,filedata->fptr) > 0)
		fwrite(&ch1,1,1,tempfptr);

	    fclose(tempfptr);
	    tempfptr = fopen(tempfname, "r");

	    fclose(filedata->fptr);
	    filedata->fptr = fopen(filedata->fname, "w");

	    rewind(filedata->fptr);
	    rewind(tempfptr);


	    while(fread(&ch1,1,1,tempfptr) > 0)
		fwrite(&ch1,1,1,filedata->fptr);



	    fclose(filedata->fptr);
	    filedata->fptr = fopen(filedata->fname, "a+");

	    remove(tempfname);
	    return success;


	}
	else if(ch == 'N' || ch == 'n')
	{
	    printf("Warning : exiting without deleting..\n");
	    return failure;
	}
    }

}


