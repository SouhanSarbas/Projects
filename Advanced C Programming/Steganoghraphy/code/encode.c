#include <stdio.h>
#include "encode.h"
#include "types.h"


#include "common.h"
#include<string.h>


/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");

    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

	return e_failure;
    }

    // No failure return e_success
    return e_success;
}


///////////////////==========================================================////////////////////




/* Check whether Command Line Arguments are passed in correct format for ENCODING */

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    // check for second CLA (command line argument)
    if (argv[2] != NULL)
    {
	
	// Check whether second CLA is a .bmp file

	char *extn = strchr(argv[2],'.');

	if(((extn != NULL) && (strcmp(extn,".bmp") == 0)) || ((extn != NULL) && (strcmp(extn,".BMP") == 0)))
	{

	    printf("Inputfile is bmp file\n");		// print info

	    encInfo->src_image_fname = argv[2];		// save image file name in structure

	    // check whether third CLA is present or not
	    if (argv[3] != NULL)
	    {

	    /* check file extension and file name of secret file */

		extn = strchr(argv[3],'.');
		
		if((extn != NULL) && (strlen(extn) > 1))
		{
		
		    printf("Secret file extension is %s\n",extn);	// print info

		    encInfo->secret_fname = argv[3];			// save secret file name to structure
		    
		    strcpy(encInfo->extn_secret_file, extn);		// save secret file extension to structure

		    // check whether fourth optional CLA is passed
		    if(argv[4] != NULL)
		    {

		    /* check and validate optional output image file name */
			
			extn = strchr(argv[4],'.');
			
			if(extn == NULL)	// when file has no extension
			{
			    /* Add .bmp extension to filename */
			    encInfo->stego_image_fname = argv[4];

			    strcat(encInfo->stego_image_fname, ".bmp");

			    printf("Output filename is %s\n", encInfo->stego_image_fname);
			}
			else if((strcmp(extn,".bmp") == 0) || (strcmp(extn,".BMP") == 0))	// when file has .bmp extension
			{
			    /* Save .bmp file */
			    encInfo->stego_image_fname = argv[4];
			    printf("Output filename is %s\n", encInfo->stego_image_fname);

			}
			else		// when file has other extension (not .bmp)
			{
			    /* When extension given is not .bmp, use default image file name */
			    printf("Warning: output file must be w/o extension or .bmp\n"
				    "  > taking default name -> %s\n", DEFAULT_OUTPUT_FILENAME);

			    encInfo->stego_image_fname = DEFAULT_OUTPUT_FILENAME;
			}
		    }
		    else
		    {
			/* When optional output image file name is not given, take default image file name */
			printf("Optional output filename missing. taking default name -> %s\n", DEFAULT_OUTPUT_FILENAME);
			encInfo->stego_image_fname = DEFAULT_OUTPUT_FILENAME;
		    }

		    return e_success;

		}
		else
		{
		    printf("\nError : Secret file missing valid extension\n");
		    return e_failure;
		}
	    }
	    else
	    {
		printf("\nError : Secret file missing\n");
		return e_failure;
	    }
	}
	else
	{
	    printf("\nError : source image should be in .bmp format\n");
	    return e_failure;
	}

    }
    else
    {
	printf("\nError: input image file missing\n");		
	return e_failure;					
    }
}

/* Encoding secret file into .bmp image file and create new encoded image file */

Status do_encoding(EncodeInfo *encInfo)
{

    /* Open the source image file and secret file. also create output image file */
    if((open_files(encInfo) == e_success) && (confirm_BMP(encInfo->fptr_src_image) == e_success))
    {
	printf("Files opened successfully\n");

	/* Read Magic String from the user */
	printf("Enter magic string to encode the secret file : ");
	char magic_string_var[MAGIC_STR_LEN];
	scanf(" %s", magic_string_var);
	// store the entered magic string details to the structure
	encInfo->magic_string = magic_string_var;
	encInfo->magic_string_size = strlen(encInfo->magic_string);


	/* Check whether image file has enough bytes (capacity) to accomodate encoding */
	if(check_capacity(encInfo) == e_success)
	{
	    printf("Passed capacity check\n");

	    /* Copy .bmp source image file's header to output image file */
	    if(copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
	    {
		printf("Image header copied\n");
		
		/* 1. Encode magic string length in 32 bytes */
		if(encode_magic_string_size(encInfo->magic_string_size, encInfo) == e_success)
		{
		    printf("Magic string length encoded\n");

		    /* 2. Encode magic string */
		    if(encode_magic_string(encInfo->magic_string, encInfo) == e_success)
		    {
			printf("Magic string encoded\n");
			
			/* 3. Encode secret file name's extension size in 32 bytes */
			if(encode_secret_file_extn_size(strlen(encInfo->extn_secret_file), encInfo) == e_success)
			{
			    printf("Extension size encoded\n");
			    
			    /* 4. Encode secret file's extention */
			    if(encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == e_success)
			    {
				printf("Secretfile extension encoded\n");
				
				/* 5. Encode secret file size in 32 bytes */
				if(encode_secret_file_size(encInfo->size_secret_file, encInfo) == e_success)
				{
				    printf("Secret file size encoded\n");
				    
				    /* 6. Encode secret file data */
				    if(encode_secret_file_data(encInfo) == e_success)
				    {
					printf("Secret file data encoded\n");
					
					/* Copy remaining bytes of data in source image file to output image file */
					if(copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
					{
					    printf("Remining image data copied\n");
					    return e_success; // Encoding done successfully
					}


					/* ERROR handling */
					else
					{
					    printf("Error : Failure in copying remining image data\n");
					    return e_failure;
					}
				    }
				    else
				    {
					printf("Error : secret file data encoding failed\n");
					return e_failure;
				    }
				}
				else
				{
				    printf("Error : Secretfile size encoding failed\n");
				    return e_failure;
				}

			    }
			    else
			    {
				printf("Error : Extension encoding failed\n");
				return e_failure;
			    }

			}
			else
			{
			    printf("Error : Extension size encoding failed\n");
			    return e_failure;
			}
		    }
		    else
		    {
			printf("Error : Magic string encoding failed\n");
			return e_failure;
		    }
		}
		else
		{
		    printf("Error : Magic string length encoding failed\n");
		    return e_failure;
		}
	    }
	    else
	    {
		printf("Error : image header copying failed..\n");
		return e_failure;
	    }
	}
	else
	{
	    printf("Error : size needed exceeds image capacity\n");
	    return e_failure;
	}
    }
    else
    {
	printf("Error : failed at opening related files\n");
	return e_failure;
    }
}

/* Check whether image file has enough bytes (capacity) to accomodate encoding */
Status check_capacity(EncodeInfo *encInfo)
{
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image); // no. of encodable bytes in image file
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);	// no. of bytes in secret file 

    /* checking whether image capacity is larger than total bytes needed to encode secret file */
    if(encInfo->image_capacity > ((encInfo->magic_string_size + strlen(encInfo->extn_secret_file) +
		    encInfo->size_secret_file)*BYTE_SIZE + INT_SIZE * 3 ))
	return e_success;
    else
	return e_failure;
}

/* Find the toatal no. of bytes of data in a file. ie file size */
uint get_file_size(FILE *fptr)
{
    fseek(fptr, 0, SEEK_END);
    return ftell(fptr);
}

/* Confirm the file is BMP image */
Status confirm_BMP(FILE *img_ptr)
{
    char B,M;
    if(fread(&B, 1, 1, img_ptr) < 1 )
	return e_failure;
    if(fread(&M, 1, 1, img_ptr) < 1 )
	return e_failure;
    if((B != 'B') || (M != 'M'))
	return e_failure;
    if(get_file_size(img_ptr) < 54)
	return e_failure;
    rewind(img_ptr);
    return e_success;
}

/* Copy .bmp source image file's header to output image file */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    rewind(fptr_src_image);	// goto beginning of the file
    rewind(fptr_dest_image);

    char buffer[54];
    fread(buffer,54,1,fptr_src_image);
    fwrite(buffer,54,1,fptr_dest_image);

    return e_success;
}

/* 1. Encode magic string length in 32 bytes */
Status encode_magic_string_size(int magic_string_size, EncodeInfo * encInfo)
{
    char buffer[INT_SIZE];
    fread(buffer, INT_SIZE, 1, encInfo->fptr_src_image);
    if(encode_size_to_lsb(magic_string_size, buffer) == e_success)
    {
	fwrite(buffer, INT_SIZE, 1, encInfo->fptr_stego_image);
	return e_success;
    }
    else
	return e_failure;
}

/* 2. Encode magic string */
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    char buffer[BYTE_SIZE];
    for (int i = 0; i < strlen(magic_string); i++)
    {
	fread(buffer, BYTE_SIZE, 1, encInfo->fptr_src_image);

	if(encode_byte_to_lsb(magic_string[i],buffer) == e_success)
	    fwrite(buffer, BYTE_SIZE, 1, encInfo->fptr_stego_image);
	else
	    return e_failure;
    }
    return e_success;
}

/* Encode lsb of 8 bytes with bits of a byte (or character) (i.e. 8 bits) */
Status encode_byte_to_lsb(char data, char *image_buffer)
{

    for(int i = 0; i < BYTE_SIZE; i++)
    {
	image_buffer[i] = (image_buffer[i] & 0xfe) | ((data >> (BYTE_SIZE - 1 - i)) & 1);
    }
    return e_success;

}

/* 3. Encode secret file name's extension size in 32 bytes */
Status encode_secret_file_extn_size(int file_extn_size, EncodeInfo *encInfo)
{
    char buffer[INT_SIZE];
    fread(buffer, INT_SIZE, 1, encInfo->fptr_src_image);
    if(encode_size_to_lsb(file_extn_size,buffer) == e_success)
    {
	fwrite(buffer, INT_SIZE, 1, encInfo->fptr_stego_image);
	return e_success;
    }
    else
	return e_failure;

}

/* Encode lsb of 32 bytes with bits of an integer number (32 bits) */
Status encode_size_to_lsb(int file_extn_size, char *image_buffer)
{
    for(int i = 0; i < INT_SIZE; i++)
    {
	image_buffer[i] = ((image_buffer[i] & ~((int) 1)) |
		((file_extn_size >> (INT_SIZE - 1 - i)) & 1));
    }
    return e_success;
}

/* 4. Encode secret file's extention */
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    char buffer[BYTE_SIZE];
    for(int i = 0; i < strlen(file_extn); i++)
    {
	fread(buffer, BYTE_SIZE, 1, encInfo->fptr_src_image);
	encode_byte_to_lsb(file_extn[i],buffer);
	fwrite(buffer, BYTE_SIZE, 1, encInfo->fptr_stego_image);
    }
    return e_success;
}

/* 5. Encode secret file size in 32 bytes */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    char buffer[INT_SIZE];
    fread(buffer, INT_SIZE, 1, encInfo->fptr_src_image);

    if(encode_size_to_lsb((int)file_size,buffer) == e_success)
    {
	fwrite(buffer, INT_SIZE, 1, encInfo->fptr_stego_image);
	return e_success;
    }
    else
	return e_failure;

}

/* 6. Encode secret file data */
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char buffer[encInfo->size_secret_file], temp[BYTE_SIZE];
    rewind(encInfo->fptr_secret);

    fread(buffer, encInfo->size_secret_file, 1, encInfo->fptr_secret);

    for(int i = 0; i < encInfo->size_secret_file; i++)
    {
	fread(temp,BYTE_SIZE,1,encInfo->fptr_src_image);
	encode_byte_to_lsb(buffer[i],temp);
	fwrite(temp, BYTE_SIZE,1,encInfo->fptr_stego_image);
    }
    return e_success;
}

/* Copy remaining bytes of data in source image file to output image file */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;
    while(fread(&ch, 1,1, fptr_src) > 0)	// till end of file
	fwrite(&ch,1,1,fptr_dest);
    return e_success;
}





