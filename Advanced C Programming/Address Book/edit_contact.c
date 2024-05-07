/*

NAME : 
DATE :
DESCRIPTION :
SAMPLE INPUT :
SAMPLE OUTPUT :

 */
#include "common.h"
Status edit_contact(BookInfo *filedata)
{
    printf( 
	    "\n===========================\n"
	    "Edit Contact\n"
	    "---------------------------\n"
	    "1. Search by Name\n"
	    "2. Search by Mobile No.\n"
	    "3. Search by Email\n"
	    "4. Search by Location\n"
	    "5. Exit\n"
	    "===========================\n"
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
		    printf("Enter the Sl.no number to edit :");
		    scanf(" %d", &n);
		    if(edit_SL(filedata, n) == success)
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
		    printf("Enter the Sl.no number to edit :");
		    scanf(" %d", &n);
		    if(edit_SL(filedata, n) == success)
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
		    printf("Enter the Sl.no number to edit :");
		    scanf(" %d", &n);
		    if(edit_SL(filedata, n) == success)
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
		    printf("Enter the Sl.no number to edit :");
		    scanf(" %d", &n);
		    if(edit_SL(filedata, n) == success)
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

Status edit_SL(BookInfo *filedata, int n)
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
    while(1)
    {
	printf("Edit the contact at ");
	printf("%6d  :\n", n);
	printf("1. Name     : %24s\n", contact.name);
	printf("2. Mob No.  : %24s\n", contact.mob);
	printf("3. Email id : %24s\n", contact.email);
	printf("4. Location : %24s\n", contact.location);
	printf("5. Save edits\n");
	printf("6. Discard changes and exit\n");

	printf("Choose to edit : ");
	scanf(" %c", &ch);

	switch(ch)
	{
	    case '1':
		{
		    strcpy(str, contact.name);

		    len = strlen(contact.name);
		    printf("Update the name %s : ", contact.name);
		    scanf(" %[^\n]", contact.name);
		    // strip extra spaces at both ends
		    for(i = 0; i < len; i++)
		    {
			if(contact.name[0] == ' ')
			{
			    for(j = 0; j<len-1;j++)
			    {
				contact.name[j] = contact.name[j+1];
			    }
			    contact.name[j] = '\0';
			    len--;
			    i=-1;
			    continue;
			}
			if(contact.name[len-1] == ' ')
			{
			    contact.name[len -1] = '\0';
			    len--;
			    i--;
			}
		    }


		    //
		    int flag = 0;
		    rewind(filedata->fptr);
		    while(fread(&ch,1,1,filedata->fptr) > 0)
		    {
			if (ch == '\n')
			    break;
		    }
		    for(int i = 0; i < (filedata->size); i++)
		    {

			ContactInfo contact_t;
			int pos = 0;
			while(fread(&ch,1,1,filedata->fptr) > 0)
			{
			    if(ch == ',')
			    {
				contact_t.name[pos++] = '\0';
				break;
			    }
			    contact_t.name[pos++] = ch;
			}

			if(strcmp(contact.name, contact_t.name) == 0)
			    flag++;
			while(fread(&ch,1,1,filedata->fptr) > 0)
			{
			    if(ch == '\n')
				break;
			}
		    }
		    if(flag != 0)
		    {
			printf("Warning: %s is already exist in the Addressbook\n", contact.name);
		    strcpy(contact.name, str);
		    }

		    break;
		}
	    case '2':
		/*mob no*/
		{
		    printf("Enter %s 's 10 digit Mobile No. : ", contact.name);
		    scanf(" %s", str);

		    if((len = strlen(str)) == 10)
		    {
			for(i = 0; i < 10; i++)
			{
			    if(str[i] < '0' || str[i] > '9')
			    {
				printf("ERROR: Mob number should not have non digit characters\n");
				break;
			    }
			}
		    }
		    else
		    {
			printf("ERROR: Mob number must be 10 digits\n");
			break;
		    }
		    strcpy(contact.mob, str);

		    //
		    break;
		}
	    case '3':
		{
		    /*email*/

		    printf("Enter %s 's Email id : ", contact.name);
		    scanf(" %s", str);

		    char *str1;
		    if((str1 = strstr(str, "@")) != NULL)
		    {
			if(strstr(str1, ".com") != NULL)
			{
			    strcpy(contact.email, str);
			}
			else
			{
			    printf("ERROR: Email missing '.com' \nEmail id eg: sample@gmail.com\n");
			    break;
			}
		    }
		    else
		    {
			printf("ERROR: emial missing '@' \nEmail id eg: sample@gmail.com\n");
			break;
		    }
		    break;
		}
	    case '4':
		{
		    /*loc*/
		    printf("Enter %s 's location : ", contact.name);
		    scanf(" %[^\n]", contact.location);
		    break;
		}
	    case '5':
		{

		    char *tempfname = filedata->ftemp;
		    
		    FILE *tempfptr = fopen(tempfname, "w");
		    if(tempfptr == NULL)
		    {
			return failure;
		    }

		    rewind(filedata->fptr);
		    rewind(tempfptr);

		    char ch1;
		    while(ftell(filedata->fptr) < posL)
		    {
			fread(&ch1,1,1,filedata->fptr);
			fwrite(&ch1,1,1,tempfptr);
		    }
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

		    fseek(filedata->fptr, posR-1, SEEK_SET);

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


		    break;
		}
	    case '6':
		{
		    printf("Warning : exiting without saving..\n");
		    return failure;
		    break;
		}
	    default:
		printf("ERROR: Invalid option\n");
	}
    }


}














