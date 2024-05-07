#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "decode.h"


#include<string.h>


int main(int argc, char *argv[])
{
    OperationType ret;

    /* Check whether any CLA (command line argument) is passed or not */
    if(argc < 2)	// when CLA is missing, take operation flag as unsupported 
	ret = e_unsupported;
    else		// otherwise find operation type from first CLA
	ret = check_operation_type(argv[1]);

    /* When CLA is for ENCODING */
    if(ret == e_encode)
    {
	EncodeInfo encInfo;	// create a structure to store info needed for encoding
	
	// Check whether valid CLAs are passed in correct format for encoding
	if(read_and_validate_encode_args(argv, &encInfo) == e_success)
	{
	    printf("Encode validation completed\n");
	    
	    // ENCODE secret file to image file
	    if(do_encoding(&encInfo) == e_success)
	    {
		printf("\nEncoded successfully\n");
		printf(">> Encoded image file : %s\n", encInfo.stego_image_fname);
	    }
	    else // Error Handling
	    {
		printf("Error: Encoding failed\n");
	    }

	}
	else // Error Handling
	{
	    printf("ERROR: Incorrect argumrnt format\n"
		    "usage : ./a.out -e imagename.bmp secretfile.extension [outputimagename]\n");
	}
    }
    /* When CLA is for DECODING */
    else if(ret == e_decode)
    {
	DecodedInfo decoInfo;	// create a structure to store info needed for decoding

	// Check whether valid CLAs are passed in correct format for decoding
	if(read_and_validate_decode_args(argv, &decoInfo) == e_success)
	{
	    printf("Decode validation completed\n");
	    
	    // DECODE secret file from image file
	    if(do_decoding(&decoInfo) == e_success)
	    {
		printf("\nDecoded successfully\n");
		printf(">> Secret file : %s\n", decoInfo.secret_fname);
	    }
	    else // Error Handling
	    {
		printf("Error : Decoding failed\n");
	    }
	}
	else // Error Handling
	{
	    printf("ERROR: Incorrect argumrnt format\n"
		    "usage : ./a.out -d imagename.bmp [decodedfilename]\n");
	}

    }
    /* ERROR Handling when CLA is not passed or is invalid option */
    if(ret == e_unsupported)
    {
	    printf("ERROR: Incorrect argumrnt format\n"
		"for encoding : ./a.out -e imagename.bmp secretfile.extension [outputimagename]\n"
		"for decoding : ./a.out -d imagename.bmp [decodedfilename]\n");
	return 1; // terminated with Error

    }

    return 0; // terminated successfully
}


/* function to find which operation need to be executed from first CLA */
OperationType check_operation_type(char argv[])
{
    /* option "-e" is for ENCODING */
    if(strcmp(argv, "-e") == 0)
    {
	printf("%20s\n","* * *  Encoding  * * *");
	printf("%20s\n","----------------------");
	return e_encode;
    }
    /* option "-d" is for DECODING */
    else if (strcmp(argv, "-d") == 0)
    {
	printf("%20s\n","* * *  Decoding  * * *");
	printf("%20s\n","----------------------");
	return e_decode;
    }
    /* when CLA is invalid option */
    else
    {
	return e_unsupported;
    }
}


