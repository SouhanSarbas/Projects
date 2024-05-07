#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types


/* Maximum length of extension*/
#define MAX_EXTN_SIZE 10

/* Maximum length of secret file name*/
#define MAX_NAME_SIZE 100

/*
   *Structure to store information required for decoding
   *secret file from source image file (encoded output image file)
   */
typedef struct _DecodeInfo
{
    /* Encoded Image Details */
    FILE *fptr_src_image;
    char *src_image_fname;

    /* Secret File Details */
    char *decoded_fname;
    char secret_fname[MAX_NAME_SIZE];
    char extn[MAX_EXTN_SIZE];
    FILE *fptr_decoded;
    int file_size;
    int extn_size;

    /* Magic String Info */
    int magic_string_size;
    char *magic_string;

} DecodedInfo;


/* Decoding function prototype */

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodedInfo *decoInfo);

/* Perform the decoding*/
Status do_decoding(DecodedInfo *decoInfo);

/* Get file pointer for image */
Status open_image(DecodedInfo *decoInfo);

/* Decode magic string size */
Status decode_magic_string_size(int magic_str_len, DecodedInfo *decoInfo);

/* Decode magic string */
Status decode_magic_string(const char *magic_str, DecodedInfo *decoInfo);

/* Decode secret file extension size */
Status decode_extn_size(DecodedInfo *decoInfo);

/* Decode secret file extension */
Status decode_extn(int extn_size, DecodedInfo *decoInfo);

/* Decode secret file size */
Status decode_file_size(DecodedInfo *decoInfo);

/* Decode secret file data */
Status decode_file_data(int data_size, DecodedInfo *decoInfo);

/* Extract LSB of image data array and form a byte of data */
char extract_lsb_to_byte(char *buffer);

/* Extract LSB of image data array and form an integer number*/
uint extract_lsb_to_int(char *buffer);

#endif
