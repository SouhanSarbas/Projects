#ifndef COMMON_H
#define COMMON_H

/*  type definitions */

typedef enum
{
    e_view,
    e_edit,
    e_help,
    e_invalid
} Operation_type;

typedef enum
{
    e_success,
    e_failure
} Status;

/* structure to store tag details */

typedef struct _MP3
{
    /* Output MP3 file Info */
    char *dest_filename;
    FILE *dest_fptr;
    /* Source MP3 file Info */
    char *src_filename;
    FILE *src_fptr;

    /*ID3 V2.4 header : 10 bytes*/
    int extended_headersize;

    /* ID3 Frame header */
    struct frame
    {
	char frame_id[5];
	int frame_size;
	char frame_str[160];
	int src_ftell; // position in source file
    } TPE1, TIT2, TALB, TYER, TCON, COMM;


} MP3_Info;

/* Function delarations */

/* Check the operation type */
Operation_type check_operation(char *argv);





#endif
