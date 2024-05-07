#ifndef VIEW_MP3_H
#define VIEW_MP3_H

#include <stdio.h>
#include "common.h"


/* Function declarations */

/* validate mp3 file passed */
Status validate_mp3_file(char *argv);

/* reads the details of mp3 file */
Status read_id3_details(char *argv, MP3_Info *mp3Info);

/* displays the details of mp3 file */
void view_id3_details(MP3_Info *mp3Info);

/* extract size of extended header from the header */
int size_from_header(char header[]);

/* extract frame details to the structure */
void extract_frame_info(char *tag_id, MP3_Info *mp3Info);

/* To keep the record for each frame header's position */
void save_starting_address_for_frames(char *tag_id, MP3_Info *mp3Info);

/* To convert header size by eliminating MSBs of each bytes */
int change_to_header_size(char m, char n, char o, char p);

#endif
