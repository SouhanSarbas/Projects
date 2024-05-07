/*

NAME : SOUHAN SARBAS C H
DATE : 28-3-2024
DESCRIPTION :	
SAMPLE INPUT :	
SAMPLE OUTPUT :	

 */

#include <stdio.h>
#include <string.h>
#include "common.h"
#include "edit_mp3.h"
#include "view_mp3.h"

/*  Main function  */
int main(int argc, char *argv[])
{

    /* Check operation type from Command Line Arguments (CLA) */
    Operation_type ret = check_operation(argv[1]);
    /* Create the structure to store the related datas */
    MP3_Info mp3Info; 

    /* Display Help Menu */
    if(ret == e_help)
	help_menu(argv[0]);
    /* View MP3 file details */
    else if (ret == e_view)
    {
	/* Check if the file passed is a valid mp3 file */
	if(validate_mp3_file(argv[2]) == e_success)
	{
	    /* Extract tag details to the structure */
	    if(read_id3_details(argv[2], &mp3Info) == e_success)
	    {
		printf("Tag reading Successfull\n");
		/* Display the tag details */
		view_id3_details(&mp3Info);
	    }
	    // Error Handling
	    else
	    {
		printf("ERROR : Failed to view mp3 file details\n");
	    }
	}
	// Error Handling
	else
	{
	    printf( "ERROR : Invalid arguments passed\n");
	    printf( "usage: %s -v fileName.mp3\n",argv[0]);
	    printf( "use --help for help menu\n");
	}
    }

    /* Edit MP3 file tag detail */
    else if (ret == e_edit)
    {
	/* Check if the file passed is a valid mp3 file */
	if(validate_mp3_file(argv[3]) == e_success)
	{
	    /* Extract tag details to the structure */
	    if(read_id3_details(argv[3], &mp3Info) == e_success)
	    {
		/* Edit the tag details */
		if(edit_mp3_data(argv[1], argv[2], &mp3Info) == e_success)
		    printf("Tag Editing Succussful\n");
		// Error Handling
		else
		    printf("ERROR : Editing failed\n");

	    }
	    // Error Handling
	    else
	    {
		printf("ERROR : Failed to view mp3 file details\n");
	    }
	}
	// Error Handling
	else
	{
	    printf("ERROR : Invalid arguments passed\n");
	    printf( "usage: %s -[taAmyc] \"value\" fileName.mp3\n\n",argv[0]);
	    printf( "use --help for help menu\n");
	}
    }
    // Error Handling
    if (ret == e_invalid)
    {
	printf( "ERROR : Invalid arguments passed\n");
	printf( "usage: %s -v fileName.mp3\n",argv[0]);
	printf( "       %s -[taAmyc] \"value\" fileName.mp3\n\n",argv[0]);
	printf( "use --help for help menu\n");
    }

    return 0;
}

/* Function to check operation type from CLA passed */
Operation_type check_operation(char *argv)
{
    /* if CLA is empty, its invalid operation */
    if(argv == NULL)
	return e_invalid;
    /* if CLA is "-v" operation is to 'view' */
    if(strcmp(argv, "-v") == 0)
	return e_view;
    /* if CLA is "-[taAmyc]" operation is to edit the tag */
    else if((strcmp(argv, "-t") == 0) ||
	    (strcmp(argv, "-a") == 0) ||
	    (strcmp(argv, "-A") == 0) ||
	    (strcmp(argv, "-m") == 0) ||
	    (strcmp(argv, "-y") == 0) ||
	    (strcmp(argv, "-c") == 0) )
    {
	return e_edit;
    }
    /* if CLA is "-h" or "--help" operation is to get help menu */
    else if((strcmp(argv, "--help") == 0) || (strcmp(argv, "-h") == 0))
	return e_help;
    /* otherwise it is invalid operation */
    else
	return e_invalid;
}


