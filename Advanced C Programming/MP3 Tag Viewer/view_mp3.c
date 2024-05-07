/*

NAME : 
DATE :
DESCRIPTION :
SAMPLE INPUT :
SAMPLE OUTPUT :

 */

/*   MP3 ID3 v2.3  Reading   */

#include "view_mp3.h"
#include "common.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/* Function definitions */

/* validate mp3 file passed */
Status validate_mp3_file(char *argv)
{
    char *extn;

    if(argv != NULL)	// check if CLA passed is empty
    {	
	/* check if fiile name passed (CLA) has extension or not */
	if((extn = strchr(argv, '.')) != NULL) 	
	{
	    /* Check whether the file name's extension is .mp3 */
	    if((strcmp(extn, ".mp3") == 0) || (strcmp(extn, ".MP3"))) 
	    {
		//printf("CLA passed format check\n");
		return e_success;

	    }
	    /* Error Handling */
	    else
	    {
		printf("ERROR : File missing .mp3 extension\n");
		return e_failure;
	    }
	}
	else
	{
	    printf("ERROR : File has no extension\n");
	    return e_failure;
	}
    }
    else
    {
	printf("ERROR : CLA MP3 fileName is missing\n");
	return e_failure;
    }


}

/* displays the details of mp3 file */

Status read_id3_details(char *argv, MP3_Info *mp3Info)
{
    /* Open the Original mp3 audio file to read data */
    mp3Info -> src_fptr = fopen(argv, "r");
    mp3Info -> src_filename = argv;

    if(mp3Info->src_fptr == NULL)
    {
	printf("ERROR : %s file failed to open\n", argv);
	return e_failure;

    }

    /* Header for ID3 V2.3*/
    char file_identifier[3] = {'I', 'D', '3'};
    char id3version[2] = {0x03, 0x00};

    /* Check if the header bytes are valid or not */
    char header[10];
    if(fread(header, 10, 1, mp3Info -> src_fptr) > 0)
    {
	for(int i = 0; i < 3; i++)
	{
	    /* first 3 bytes must be 'ID3' */
	    if(header[i] != file_identifier[i])
	    {
		printf("ERROR : %s file data is not mp3\n", argv);
		return e_failure;
	    }
	}
	/* next 2 bytes is for version and it must be 03 00 for v2.3 */
	if((header[3] != id3version[0] ) || (header[4] != id3version[1]))
	{
	    printf("ERROR : this version of ID3 is not supported\n");
	    return e_failure;
	}

	/* After 1 byte for flag, Next 4 bytes are for Extended header size 
	   the MSB of these bytes is 0 by defult.
	   So size is found by removing MSBs and converting the 28 bits to number */
	mp3Info -> extended_headersize = change_to_header_size(header[6],header[7],header[8],header[9]);
	//printf("header size = %d = %X\n", mp3Info -> extended_headersize, mp3Info -> extended_headersize);

	/* Extract next 6 header frames */
	char tag_id[5];
	for(int i = 0; i < 6; i++)
	{
	    if(fread(tag_id, 5, 1, mp3Info -> src_fptr) > 0)
	    {
		tag_id[4] = '\0';
		fseek(mp3Info -> src_fptr, -1, SEEK_CUR);
		// save current position with frame header's data
		save_starting_address_for_frames(tag_id, mp3Info);
		// extract header frame data and store in the structure
		extract_frame_info(tag_id, mp3Info);
	    }
	}

	return e_success;

    }
    else // Error when 10 bytes header is missing
    {
	printf("ERROR : %s file data missing header or currupted\n", argv);
	return e_failure;
    }

}

/* To convert the 4 bytes in header for size by removing MSBs (to get 28 bit number) */
int change_to_header_size(char m, char n,  char o, char p)
{
    int size = (int) m;
    size = size << 7;
    size = size | (int) n;
    size = size << 7;
    size = size | (int) o;
    size = size << 7;
    size = size | (int) p;
    return size;
}

/* Extract frame details to the structure */
void extract_frame_info(char *tag_id, MP3_Info *mp3Info)
{

    char size[4];
    int fsize = 0;
    // read the frame size bytes to get the size of tag's content
    fread(size, 4, 1, mp3Info -> src_fptr);

    /*size bytes are in bigindian form */
    void *ptr = malloc(4);
    *(char *) ptr = size[3];
    *(char *) (ptr + 1) = size[2];
    *(char *) (ptr + 2) = size[1];
    *(char *) (ptr + 3) = size[0];

    fsize = *(int *)ptr;
    free(ptr);
    //printf("\n%s %d\n",tag_id, fsize);


    fseek(mp3Info -> src_fptr, 3, SEEK_CUR); // skip to content

    char content_str[fsize], content_arr[fsize - 1];

    /* Read the content (and change 00 bytes to '.') */
    fread(content_arr, fsize - 1,1, mp3Info->src_fptr);
    for(int i = 0; i < fsize - 1; i++)
	content_str[i] = content_arr[i] != 0? content_arr[i] : '.';
    content_str[fsize - 1] = '\0';

    //printf("content : %s\n", content_str);

    /* printf("content : ");
       fwrite(content, fsize, 1, stdout);
       printf("\n");*/


    /* Based on tag id, save the id, size and content to the corresponding tag in the structure */
    if(strcmp(tag_id, "TPE1") == 0)
    {
	strcpy(mp3Info -> TPE1.frame_id , tag_id);
	mp3Info -> TPE1.frame_size = fsize;
	strcpy(mp3Info -> TPE1.frame_str, content_str);

    }
    else if(strcmp(tag_id, "TIT2") == 0)
    {
	strcpy(mp3Info -> TIT2.frame_id , tag_id);
	mp3Info -> TIT2.frame_size = fsize;
	strcpy(mp3Info -> TIT2.frame_str, content_str);
    }
    else if(strcmp(tag_id, "TALB") == 0)
    {
	strcpy(mp3Info -> TALB.frame_id , tag_id);
	mp3Info -> TALB.frame_size = fsize;
	strcpy(mp3Info -> TALB.frame_str, content_str);
    }
    else if(strcmp(tag_id, "TYER") == 0)
    {
	strcpy(mp3Info -> TYER.frame_id , tag_id);
	mp3Info -> TYER.frame_size = fsize;
	strcpy(mp3Info -> TYER.frame_str, content_str);
    }
    else if(strcmp(tag_id, "TCON") == 0)
    {
	strcpy(mp3Info -> TCON.frame_id , tag_id);
	mp3Info -> TCON.frame_size = fsize;
	strcpy(mp3Info -> TCON.frame_str, content_str);
    }
    else if(strcmp(tag_id, "COMM") == 0)
    {
	strcpy(mp3Info -> COMM.frame_id , tag_id);
	mp3Info -> COMM.frame_size = fsize;
	strcpy(mp3Info -> COMM.frame_str, content_str);
    }

}

/* To keep the record for each frame header / tag position */

void save_starting_address_for_frames(char *tag_id, MP3_Info *mp3Info)
{

    int pos;
    if(strcmp(tag_id, "TPE1") == 0)
    {
	pos = ftell(mp3Info -> src_fptr);
	mp3Info -> TPE1.src_ftell = pos;
    }
    else if(strcmp(tag_id, "TIT2") == 0)
    {
	pos = ftell(mp3Info -> src_fptr);
	mp3Info -> TIT2.src_ftell = pos;
    }
    else if(strcmp(tag_id, "TALB") == 0)
    {
	pos = ftell(mp3Info -> src_fptr);
	mp3Info -> TALB.src_ftell = pos;
    }
    else if(strcmp(tag_id, "TYER") == 0)
    {
	pos = ftell(mp3Info -> src_fptr);
	mp3Info -> TYER.src_ftell = pos;
    }
    else if(strcmp(tag_id, "TCON") == 0)
    {
	pos = ftell(mp3Info -> src_fptr);
	mp3Info -> TCON.src_ftell = pos;
    }
    else if(strcmp(tag_id, "COMM") == 0)
    {
	pos = ftell(mp3Info -> src_fptr);
	mp3Info -> COMM.src_ftell = pos;
    }
}

/* Displays the details of mp3 file from the structure */
void view_id3_details(MP3_Info *mp3Info)
{
    printf("-------------------------------------------------------------------\n");
    printf("  MP3 Tag Reader and Editor for ID3v2\n");
    printf("-------------------------------------------------------------------\n");
    printf( " %-20s :   %s\n"
	    " %-20s :   %s\n"
	    " %-20s :   %s\n"
	    " %-20s :   %s\n"
	    " %-20s :   %s\n"
	    " %-20s :   %s\n",
	    "Title", mp3Info -> TIT2.frame_str,
	    "Artist", mp3Info -> TPE1.frame_str,
	    "Album", mp3Info -> TALB.frame_str,
	    "Year", mp3Info -> TYER.frame_str,
	    "Music/Content type", mp3Info -> TCON.frame_str,
	    "Comment", mp3Info -> COMM.frame_str);
    printf("-------------------------------------------------------------------\n");
}


