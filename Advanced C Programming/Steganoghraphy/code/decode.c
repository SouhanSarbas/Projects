/*

NAME : SOUHAN SARBAS C H
DATE : 20-3-2024
DESCRIPTION :	Functions for decoding image having secret file by steganography
SAMPLE INPUT :
SAMPLE OUTPUT :

 */

#include <stdio.h>
#include "decode.h"
#include "types.h"


#include "common.h"
#include<string.h>

/* Set default file name for decoded secret file */
#define DEFAULT_NAME "decoded_file"


/* Check whether Command Line Arguments are passed in correct format or DECODING */
Status read_and_validate_decode_args(char *argv[], DecodedInfo *decoInfo)
{
    // Check for second CLA (Command line argument)
    if(argv[2] != NULL)
    {
	//check whether second CLA is a .bmp file

	char *extn = strchr(argv[2],'.');

	if(((extn != NULL) && (strcmp(extn,".bmp") == 0)) || 
		((extn != NULL) && (strcmp(extn,".BMP") == 0)))
	{
	    printf("Input image is bmp file confirmed\n");
	    decoInfo->src_image_fname = argv[2];

	    // check whether optional third CLA is passed
	    if(argv[3] != NULL)
	    {
		/* validate whether third CLA is a filename without extension */
		if(strchr(argv[3],'.') == NULL)
		{
		    decoInfo->decoded_fname = argv[3];
		    return e_success;
		}
		else	// error handling
		{
		    printf("Error : Decoded file name must be without extention\n");
		    return e_failure;
		}

	    }
	    else // when third CLA is absent, take default file name
	    {
		printf("Optional argument for decoded file name is missing.. taking defult name %s\n", DEFAULT_NAME);
		decoInfo->decoded_fname = DEFAULT_NAME;
		return e_success;
	    }

	}

	/*  Error Handling  */
	else
	{
	    printf("Error : Input image must be in .bmp format\n");
	    return e_failure;
	}
    }
    else
    {
	printf("Error : Input image file is missing\n");
	return e_failure;
    }
}

/* Decoding the .bmp source image file to get the secret file that was encoded within */
Status do_decoding(DecodedInfo *decoInfo)
{
    /* Open the source image */
    if(open_image(decoInfo) == e_success)
    {
	printf("Image file opened\n");

	/* Read the Magic String from the user */
	printf("Enter the magic string to decode : ");
	char magic_string_var[MAGIC_STR_LEN];
	scanf(" %s", magic_string_var);
	/* save the entered magic string details to the structure */
	decoInfo->magic_string = magic_string_var;
	decoInfo->magic_string_size = strlen(decoInfo->magic_string);

	/* 1. Find encoded size of magic string from 32 bytes and compare with the user entered magic string length */
	if (decode_magic_string_size(decoInfo->magic_string_size, decoInfo) == e_success)
	{
	    printf("Magic string size matches\n");

	    /* 2. Decode magic string and compare with the user entered magic string  */
	    if(decode_magic_string(decoInfo->magic_string, decoInfo) == e_success)
	    {
		printf("Magic string decoded and matched\n"
			"Detected presence of encoded file\n");

		/* 3. Decode secret file extension's length from 32 bytes */
		if(decode_extn_size(decoInfo) == e_success)
		{
		    printf("Secret file extension size decoded : %d\n", decoInfo->extn_size);

		    /* 4. Decode Secret file's Extension */
		    if(decode_extn(decoInfo->extn_size, decoInfo) == e_success)
		    {
			printf("Decoded secret file extension\n"
				"secret file name : %s\n", decoInfo->secret_fname);

			// Create and open the secret file to write data with the decoded extension
			decoInfo->fptr_decoded = fopen(decoInfo->secret_fname, "w");
			// Do Error handling
			if(decoInfo->fptr_decoded == NULL)
			{
			    perror("fopen");
			    fprintf(stderr, "ERROR: Unable to create/open file %s\n",
				    decoInfo->secret_fname);
			    return e_failure;
			}

			/* 5. Decode Secret file size from 32 bytes */
			if(decode_file_size(decoInfo) == e_success)
			{
			    printf("Decoded secret file size : %d\n", decoInfo->file_size);

			    /* 6. Decode Secret file data and store those bytes to secret file */
			    if(decode_file_data(decoInfo->file_size, decoInfo) == e_success)
			    {
				printf("Secret file data decoded\n");
				return e_success; // Decoding done successfully
			    }

			    /* ERROR Handling*/
			    else
			    {
				printf("Error : Failed to decode secret file data\n");
				return e_failure;
			    }
			}
			else
			{
			    printf("Error : Failed to decode secret file size\n");
			    return e_failure;
			}

		    }
		    else
		    {
			printf("Error : Failed to decode secret file extension\n");
			return e_failure;
		    }
		}
		else
		{
		    printf("Error : Failed to decode secret file extension size\n");
		    return e_failure;
		}
	    }
	    else
	    {
		printf("Error : Magic string is not matching\n");
		return e_failure;
	    }
	}
	else
	{
	    printf("Error : Magic string size is not matching\n");
	    return e_failure;

	}
    }
    else
    {
	printf("Error : Image file %s opening failed\n", decoInfo->src_image_fname);
	return e_failure;
    }

}

/* Open the source image */
Status open_image(DecodedInfo *decoInfo)
{
    // Src Image file
    decoInfo->fptr_src_image = fopen(decoInfo->src_image_fname, "r");
    // Do Error handling
    if (decoInfo->fptr_src_image == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", decoInfo->src_image_fname);

	return e_failure;
    }
    else
    {
	/* Confirm image data is for valid BMP file */
	char B,M;
	FILE *img_ptr = decoInfo->fptr_src_image;

	if(fread(&B, 1, 1, img_ptr) < 1 )
	    return e_failure;
	if(fread(&M, 1, 1, img_ptr) < 1 )
	    return e_failure;
	if((B != 'B') || (M != 'M'))
	    return e_failure;
	fseek(img_ptr, 0, SEEK_END);
	if(ftell(img_ptr) < 54)
	    return e_failure;
	rewind(img_ptr);
	return e_success;
    }
}

/* 1. Find encoded size of magic string from 32 bytes and compare with the user entered magic string length */
Status decode_magic_string_size(int magic_str_len, DecodedInfo *decoInfo)
{
    fseek(decoInfo->fptr_src_image, 54, SEEK_SET);
    int decoded_size;

    char buffer[INT_SIZE];

    fread(buffer, INT_SIZE, 1, decoInfo->fptr_src_image);

    // decoding from image for magic string length
    decoded_size = extract_lsb_to_int(buffer);

    // Check whether entered magic string's length is same as encoded length of magic string
    if(magic_str_len == decoded_size)
	return e_success;
    else
	return e_failure;


}

/* 2. Decode magic string and compare with the user entered magic string  */
Status decode_magic_string(const char *magic_string, DecodedInfo *decoInfo)
{

    int magic_str_len = strlen(magic_string);

    char buffer[magic_str_len * BYTE_SIZE], deco_byte[magic_str_len];

    fread(buffer, magic_str_len * BYTE_SIZE, 1, decoInfo->fptr_src_image);
    // Decoding from image for magic string
    for(int i = 0; i < magic_str_len; i++)
    {
	deco_byte[i] = extract_lsb_to_byte(&buffer[i * BYTE_SIZE]);
    }

    // Check whether entered magic string match with decoded string
    for(int i = 0; i < magic_str_len; i++)
    {
	if(magic_string[i] == deco_byte[i])
	    continue;
	else
	    return e_failure;
    }

    return e_success;

}

/* Extract lsb of 8 bytes to make 8 bits to form one byte (or character) */
char extract_lsb_to_byte(char *buffer)
{
    char deco_byte = 0;

    for(int i = 0; i < BYTE_SIZE; i++)
    {
	deco_byte = deco_byte | ((buffer[i] & 1) <<(BYTE_SIZE - 1 - i));
    }

    return deco_byte;
}

/* 3. Decode secret file extension's length from 32 bytes */
Status decode_extn_size(DecodedInfo *decoInfo)
{
    char buffer[INT_SIZE];

    fread(buffer, INT_SIZE, 1, decoInfo->fptr_src_image);

    decoInfo->extn_size = extract_lsb_to_int(buffer);

    return e_success;

}

/* Extract lsb of 32 bytes to make 32 bits of an integer number */
uint extract_lsb_to_int(char *buffer)
{
    int size = 0;

    for(int i = 0; i < INT_SIZE; i++)
    {
	size = size | ((buffer[i] & 1) <<(INT_SIZE - 1 - i));
    }

    return size;

}

/* 4. Decode Secret file's Extension */
Status decode_extn(int extn_size, DecodedInfo *decoInfo)
{
    char buffer[extn_size * BYTE_SIZE];

    fread(buffer, extn_size * BYTE_SIZE, 1, decoInfo->fptr_src_image);

    for(int i = 0; i < extn_size; i++)
    {
	decoInfo->extn[i] = extract_lsb_to_byte(&buffer[i * BYTE_SIZE]);
    }
    decoInfo->extn[extn_size] = '\0';

    sprintf(decoInfo->secret_fname,"%s%s",decoInfo->decoded_fname,decoInfo->extn);

    return e_success;
}

/* 5. Decode Secret file size from 32 bytes */
Status decode_file_size(DecodedInfo *decoInfo)
{
    char buffer[INT_SIZE];

    fread(buffer, INT_SIZE, 1, decoInfo->fptr_src_image);

    decoInfo->file_size = extract_lsb_to_int(buffer);

    return e_success;
}

/* 6. Decode Secret file data and store those bytes to secret file */
Status decode_file_data(int data_size, DecodedInfo *decoInfo)
{
    char buffer[BYTE_SIZE],ch;

    for(int i = 0; i < data_size; i++)
    {
	fread(buffer,BYTE_SIZE,1,decoInfo->fptr_src_image);

	ch = extract_lsb_to_byte(buffer);

	fwrite(&ch,1,1,decoInfo->fptr_decoded);
    }

    return e_success;

}



