/*

NAME : 
DATE :
DESCRIPTION :
SAMPLE INPUT :
SAMPLE OUTPUT :

 */
#include"common.h"

Status add_contact(BookInfo *filedata)
{
    ContactInfo newcontact;
    printf("Enter the new contact name : ");
    scanf(" %[^\n]", newcontact.name);

    // strip extra spaces at both ends
    int len = strlen(newcontact.name);
    int i,j;
    for(i = 0; i < len; i++)
    {
	if(newcontact.name[0] == ' ')
	{
	    for(j = 0; j<len-1;j++)
	    {
		newcontact.name[j] = newcontact.name[j+1];
	    }
	    newcontact.name[j] = '\0';
	    len--;
	    i=-1;
	    continue;
	}
	if(newcontact.name[len-1] == ' ')
	{
	    newcontact.name[len -1] = '\0';
	    len--;
	    i--;
	}
    }

    int flag = 0;
    rewind(filedata->fptr);
    char ch;
    while(fread(&ch,1,1,filedata->fptr) > 0)
    {
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

	if(strcmp(contact.name, newcontact.name) == 0)
	    flag++;
	while(fread(&ch,1,1,filedata->fptr) > 0)
	{
	    if(ch == '\n')
		break;
	}
    }
    if(flag != 0)
    {
	printf("Warning: %s is already exist in the Addressbook\n", newcontact.name);
	return failure;
    }
    printf( "name : %20s\n", newcontact.name);

    /*mob no*/

    printf("Enter %s 's 10 digit Mobile No. : ", newcontact.name);
    scanf(" %s", newcontact.mob);

    if((len = strlen(newcontact.mob)) == 10)
    {
	for(i = 0; i < 10; i++)
	{
	    if(newcontact.mob[i] < '0' || newcontact.mob[i] > '9')
	    {
		printf("ERROR: Mob number should not have non digit characters\n");
		return failure;
	    }
	}
    }
    else
    {
	printf("ERROR: Mob number must be 10 digits\n");
	return failure;
    }

    printf( "name : %20s\n"
	    "Mob no. : %20s\n",
	    newcontact.name, newcontact.mob);


    /*email*/
    printf("Enter %s 's Email id : ", newcontact.name);
    scanf(" %s", newcontact.email);

    char *str;
    len = strlen(newcontact.email);
    if((str = strstr(newcontact.email, "@")) != NULL)
    {
	if(strstr(str, ".com") != NULL)
	{
	    printf( "name : %20s\n"
		    "Mob no. : %20s\n"
		    "email id : %20s\n",
		    newcontact.name, newcontact.mob, newcontact.email);
	}
	else
	{
	    printf("ERROR: Email missing '.com' \nEmail id eg: sample@gmail.com\n");
	    return failure;
	}
    }
    else
    {
	printf("ERROR: emial missing '@' \nEmail id eg: sample@gmail.com\n");
	return failure;
    }

    /*loc*/
    printf("Enter %s 's location : ", newcontact.name);
    scanf(" %[^\n]", newcontact.location);

    printf( "name : %20s\n"
	    "Mob no. : %20s\n"
	    "email id : %20s\n"
	    "location : %20s\n",
	    newcontact.name, newcontact.mob,
	    newcontact.email, newcontact.location);

    while(ch != 'Y' && ch != 'y')
    {
	printf("Save the contact ? (Y/N)");
	scanf(" %c", &ch);

	if(ch == 'n' && ch == 'N' )
	{
	    printf("Contact not saved\n");
	    return failure;
	}
    }
    /*save at last line */
    fseek(filedata->fptr,0,SEEK_END);

//    ch = '\n';
    i = 0;
  //  fwrite(&ch, 1, 1, filedata->fptr);
    while((ch = newcontact.name[i]) != '\0')
    {
	fwrite(&ch, 1, 1, filedata->fptr);
	i++;
    }

    ch = ',';
    i = 0;
    fwrite(&ch, 1, 1, filedata->fptr);
    while((ch = newcontact.mob[i]) != '\0')
    {
	fwrite(&ch, 1, 1, filedata->fptr);
	i++;
    }

    ch = ',';
    i = 0;
    fwrite(&ch, 1, 1, filedata->fptr);
    while((ch = newcontact.email[i]) != '\0')
    {
	fwrite(&ch, 1, 1, filedata->fptr);
	i++;
    }

    ch = ',';
    i = 0;
    fwrite(&ch, 1, 1, filedata->fptr);
    while((ch = newcontact.location[i]) != '\0')
    {
	fwrite(&ch, 1, 1, filedata->fptr);
	i++;
    }

    ch = '\n';
    fwrite(&ch, 1, 1, filedata->fptr);
    //printf("111 \n");
    char *tempfname = filedata->ftemp;

    FILE *tempfptr = fopen(tempfname, "w");
    if(tempfptr == NULL)
    {
	return failure;
    }

    rewind(filedata->fptr);
    rewind(tempfptr);


    //////////////////////////////////
    
    
    (filedata->size)++;
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
    
    while(fread(&ch,1,1,filedata->fptr) > 0)
	fwrite(&ch,1,1,tempfptr);

    //printf("2233111 \n");
    fclose(tempfptr);
    tempfptr = fopen(tempfname, "r");

    fclose(filedata->fptr);
    filedata->fptr = fopen(filedata->fname, "w");

    rewind(filedata->fptr);
    rewind(tempfptr);


    while(fread(&ch,1,1,tempfptr) > 0)
	fwrite(&ch,1,1,filedata->fptr);



    fclose(filedata->fptr);
    filedata->fptr = fopen(filedata->fname, "a+");

    remove(tempfname);






}


void change_to_str(char str[], int num)
{
    int i, rem, len = 0, n;

    n = num;
    while (n != 0)
    {
	len++;
	n /= 10;
    }
    for (i = 0; i < len; i++)
    {
	rem = num % 10;
	num = num / 10;
	str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
}


















