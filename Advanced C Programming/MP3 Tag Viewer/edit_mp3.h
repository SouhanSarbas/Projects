#ifndef EDIT_MP3_H
#define EDIT_MP3_H
#include<stdio.h>
#include<string.h>
#include "common.h"

/* Function declarations */  

/* displays the help menu for command line arguments to be passed */
void help_menu(char *argv);

/* To edit tag details in the ID3 header of MP3 file */
Status edit_mp3_data(char *flag, char *new_content, MP3_Info *mp3Info);

#endif
