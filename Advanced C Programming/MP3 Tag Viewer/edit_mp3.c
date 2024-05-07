/*

NAME : 
DATE :
DESCRIPTION :
SAMPLE INPUT :
SAMPLE OUTPUT :
 
 */

/*   Edit MP3 ID3 v2.3 TAG   */

#include <stdio.h>
#include <string.h>
#include "common.h"
#include "edit_mp3.h"
#include <stdlib.h>

/* Function definitions */

/* Display help menu for Command Line Arguments to be passed */

void help_menu(char *argv)
{
    printf( "usage: %s -v fileName.mp3\n",argv);
    printf( "       %s -[taAmyc] \"value\" fileName.mp3\n\n",argv);
    printf( " -v   Displays All tag informations\n\n"
	    " -t   Modifies a Title tag\n"
	    " -a   Modifies an Artist tag\n"
	    " -A   Modifies an Album tag\n"
	    " -m   Modifies a Content type tag\n"
	    " -y   Modifies a Year tag\n"
	    " -c   Modifies a Comment tag\n\n"
	    " -h   Displays this help info\n");

}

/* To edit tag details in the ID3 header of MP3 file */
Status edit_mp3_data(char *flag, char *new_content, MP3_Info *mp3Info)
{
    int len = strlen(new_content); // size of new content to be added

    char *frame_name;
    // restrict content size to 100 to avoid header ecceeding padding and entering audio data
    if (len > 100)
    {	
	printf("ERROR : New content is way too long\n");
	return e_failure;
    }

    /* open the destination file to get edited file */
    mp3Info -> dest_filename = "Edited_Audio_File.mp3";
    mp3Info -> dest_fptr = fopen(mp3Info -> dest_filename,"w");
    if(mp3Info -> dest_fptr == NULL)
    {
	printf("ERROR : Failed to create %s file\n", mp3Info -> dest_filename);
	return e_failure;
    }

    int pos = 0, prev_size = 0;

    /*based on CLA passed select frame name, 
      and extract the position and prevous size of frame header content
      before edit is done */
    switch(flag[1])
    {
	case 't' :
	    pos = mp3Info -> TIT2.src_ftell;
	    prev_size = mp3Info -> TIT2.frame_size;
	    frame_name = "Title";

	    break;
	case 'a' :
	    pos = mp3Info -> TPE1.src_ftell;	    
	    prev_size = mp3Info -> TPE1.frame_size;
	    frame_name = "Artist";
	    break;
	case 'A' :
	    pos = mp3Info -> TALB.src_ftell;	    
	    prev_size = mp3Info -> TALB.frame_size;
	    frame_name = "Album";
	    break;
	case 'm' :
	    pos = mp3Info -> TCON.src_ftell;	    
	    prev_size = mp3Info -> TCON.frame_size;
	    frame_name = "Music / Content type";
	    break;
	case 'y' :
	    pos = mp3Info -> TYER.src_ftell;	    
	    prev_size = mp3Info -> TYER.frame_size;
	    frame_name = "Year";
	    break;
	case 'c' :
	    pos = mp3Info -> COMM.src_ftell;	    
	    prev_size = mp3Info -> COMM.frame_size;
	    frame_name = "Comment";
	    break;
	default:
	    printf("ERROR : Failed to find the Tag option\n");
	    return e_failure;

    }

    char buffer[pos];
    /* Copy till the frame header to be changed */
    rewind(mp3Info -> src_fptr);
    
    //printf("pos = %d\n", pos);

    fread(buffer, pos, 1 , mp3Info -> src_fptr);
    fwrite(buffer, pos, 1, mp3Info -> dest_fptr);

    /* Edit new size */
    void *ptr = malloc(4);
    *(int *)ptr = len + 1;
    char temp = *(char *)ptr;
    *(char *)ptr = *(char *)(ptr + 3);
    *(char *)(ptr + 3) = temp;
    temp = *(char *)(ptr + 1);
    *(char *)(ptr + 1) = *(char *)(ptr + 2);
    *(char *)(ptr + 2) = temp;

    fwrite((int *)ptr, 4, 1, mp3Info -> dest_fptr);
    free(ptr);
    fseek(mp3Info -> src_fptr, 4, SEEK_CUR);

    /* Copy the flags (after size) */
    char flagBuf[3];
    int pos1;
    fread(flagBuf, 3, 1, mp3Info -> src_fptr);
    fwrite(flagBuf, 3, 1, mp3Info -> dest_fptr);
    pos1 = ftell(mp3Info -> dest_fptr);

    /* Edit the content */
    for(int i = 0; i < len; i++)
    {
	fwrite(&new_content[i], 1, 1, mp3Info -> dest_fptr);
    }

    printf("---Selected %s change option---\n", frame_name);
    printf("%-20s :   %s\n", frame_name, new_content);

    /* Copy the rest of the extended header within extended header size to be at padding area */

    fseek(mp3Info -> src_fptr, prev_size - 1, SEEK_CUR);
    char ch;
    for(pos = ftell(mp3Info -> src_fptr); pos <= mp3Info -> extended_headersize - 10 - len; pos++)
    {
	fread(&ch, 1, 1, mp3Info -> src_fptr);
	fwrite(&ch, 1, 1,mp3Info -> dest_fptr);

    }

    pos = len - prev_size;

    /* Reposition the sourse file pounter to the same as the output file 
       in the padding area of header so that the header size 
       remains the same as that of original file before editing */
    
    fseek(mp3Info -> src_fptr, pos + 1, SEEK_CUR);

    /* Copy the rest of the file as it is from padding area */
    while(fread(&ch, 1, 1, mp3Info -> src_fptr) > 0)
    {
	fwrite(&ch, 1, 1,mp3Info -> dest_fptr);
    }

    /* close files as file is edited and saved in destination file */
    fclose(mp3Info -> src_fptr);
    fclose(mp3Info -> dest_fptr);

    /* To remove destination file and keep the changes(edits) to the original file */
    /* open the edited file to read and original file to write into. */
    FILE *fptr_s = fopen(mp3Info -> src_filename, "w");
    FILE *fptr_d = fopen(mp3Info -> dest_filename, "r");

    /* copy all the data in the edited file to the original file */
    if((fptr_s != NULL) && (fptr_d != NULL))
    {
	while(fread(&ch, 1, 1, fptr_d) > 0)
	    fwrite(&ch, 1, 1, fptr_s);
    }
    else
    {
	printf("ERROR : Failed to overwrite the changes into original audio file\n");
	return e_failure;
    }
    
    /* Close files */
    fclose(mp3Info -> src_fptr);
    fclose(mp3Info -> dest_fptr);

    /* delete the edited file (as the original file is the same as the edited file) */
    remove(mp3Info -> dest_filename);
    printf("----%s changed successfully----\n", frame_name);

    return e_success;


}


