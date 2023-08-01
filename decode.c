// decoding functions

#include<stdio.h>
#include "types.h"
#include "decode.h"
#include<string.h>

/* Get File names and validate
 * Input: Stego image file name and output file name
 * Output: Status of the operation
 * Description: This function gets the file names from argv and validates them
 */
Status read_and_validate_decode_args(int argc, char *argv[], DecodeInfo *decInfo)
{
        if (argc > 2)
        {
            if (strcmp(strstr(argv[2], "."), ".bmp") == 0)
            {
                decInfo->stego_image_fname = argv[2];
            }
            else
            {
                fprintf(stderr, "Error: %s is not a bmp file\n", argv[2]);
                return e_failure;
            }
        }
        else
        {
            fprintf(stderr, "Error: Missing Arguments\n");
            printf("Encoding: ./lsb_steg -e <.bmp file> <.txt file> [Output file]\n");
            printf("Decoding: ./lsb_steg -d <.bmp file> [Output file]\n");
            return e_failure;
        }
        if (argc > 3)
        {
            strncpy(decInfo->output_file_extn, strstr(argv[3], "."), 4);
            if ((strncmp(decInfo->output_file_extn, ".txt", 4) == 0) || (strncmp(decInfo->output_file_extn, ".c", 2) == 0) || (strncmp(decInfo->output_file_extn, ".sh", 3) == 0))
            {
                decInfo->output_fname = argv[3];
            }
            else
            {
                fprintf(stderr, "Error: output file %s format should be .txt or .c or .sh\n", argv[3]);
                return e_failure;
            }
        }
        else
        {
            decInfo->output_fname = NULL;
        }

        // Password validation
            if (argc > 4)
            {
                     if (strcmp(argv[4], "-p") == 0)
                     {
                             if (argc > 5)
                             {
                                 decInfo->password = argv[5];
                                 decInfo->password_size = strlen(decInfo->password);
                             }
                             else
                             {
                                 fprintf(stderr, "Error: No password found\n");
                                 return e_failure;
                             }
                     }
                     else
                     {
                            fprintf(stderr, "Error: Invalid option %s\n", argv[4]);
                            return e_failure;
                     }
            }
            else
            {
                decInfo->password = NULL;
                decInfo->password_size = 1;
            }

    return e_success;
}


/* Get File pointers for i/p and o/p files
 * Input: Stego image file and Output file
 * Output: File pointers to Stego image and output file
 * Return: e_succcess or e_failure
 */
Status open_files_decode(DecodeInfo *decInfo)
{
        static int open_count = 0;
        if (open_count == 0)
        {
                // Open Stego image file
                decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
                //Error handling
                if (decInfo->fptr_stego_image == NULL)
                {
                    fprintf(stderr, "Error: Unable to open file %s\n", decInfo->stego_image_fname);
                    return e_failure;
                }
                else
                {
                    printf("INFO: Opened %s\n", decInfo->stego_image_fname);
                }
                open_count++;
        }
        else
        {
                //Open output file
                decInfo->fptr_output = fopen(decInfo->output_fname, "w");
                //Error handling
                if (decInfo->fptr_output == NULL)
                {
                    fprintf(stderr, "Error: Unable to open file %s\n", decInfo->output_fname);
                    return e_failure;
                }
                else
                {
                    printf("INFO: Opened %s\n", decInfo->output_fname);
                }
        }

        return e_success;
}


/* Decoding stego image to another file
 * Input: Stego image file and output file
 * Output: Decoded data to output file
 * Return: e_success or e_failure
 */
Status do_decoding(DecodeInfo *decInfo)
{
        printf("INFO: ## Decoding steged file ##\n");
        printf("INFO: Opening required files\n");

        // Open required files
        if (open_files_decode(decInfo) == e_success)
        {
                printf("INFO: Done\n");
                uint raster_data;
                fseek(decInfo->fptr_stego_image, 10L, SEEK_SET);
                fread(&raster_data, sizeof(int), 1, decInfo->fptr_stego_image);
                fseek(decInfo->fptr_stego_image, raster_data, SEEK_SET);
                printf("INFO: Decoding Magic String\n");

                // Decoding secret string signature
                if (decode_secret_string(decInfo->password, decInfo) == e_success)
                {
                        printf("INFO: Done decoding secret string\n");

                        //Check if output file is provided or not
                        if (decInfo->output_fname == NULL)
                        {

                                if (strncmp(decInfo->output_file_extn, ".txt", 4) == 0)
                                {
                                        decInfo->output_fname = "decoded.txt";
                                        printf("INFO: Output file not provided. Creating decoded.txt\n");
                                }
                                else if (strncmp(decInfo->output_file_extn, ".c", 2) == 0)
                                {
                                        decInfo->output_fname = "decoded.c";
                                        printf("INFO: Output file not provided. Creating decoded.c\n");
                                }
                                else
                                {
                                        decInfo->output_fname = "decoded.sh";
                                        printf("INFO: Output file not provided. Creating decoded.sh\n");
                                }
                                if (open_files_decode(decInfo) == e_success)
                                {
                                        printf("INFO: Opened %s\n", decInfo->output_fname);
                                }
                                else
                                {
                                    fprintf(stderr, "Error: %s function failed\n", "open_files_decode");
                                    return e_failure;
                                }
                        }
                        else
                        {
                                printf("INFO: Output file is provided\n");
                                if(open_files_decode(decInfo) == e_success)
                                {
                                        printf("INFO: Opened %s\n", decInfo->output_fname);
                                }
                                else
                                {
                                    fprintf(stderr, "Error: %s function failed\n", "open_files_decode");
                                    return e_failure;
                                }
                        }
                        printf("INFO: Opened all required files\n");

                        // Decoding file Size
                        printf("INFO: Decoding file size\n");
                        if (decode_file_size(decInfo) == e_success)
                        {
                               printf("INFO: Done decoding file size\n");

                               // Decoding File Data
                               printf("INFO: Decoding file data\n");
                               if (decode_data_to_file(decInfo) == e_failure)
                               {
                                   fprintf(stderr, "Error: %s function failed\n", "decode_data_to_file");
                                       return e_failure;
                               }
                        }
                        else
                        {
                                fprintf(stderr, "Error: %s function failed\n", "decode_file_size");
                                return e_failure;
                        }
                }
                else
                {
                        fprintf(stderr, "Error: %s function failed\n", "decode_secret_string");
                        return e_failure;
                }

        }
        else
        {
                fprintf(stderr, "Error: %s function failed\n", "open_files_decode");
                return e_failure;
        }
        printf("INFO: Done, decode process completed\n");
        return e_success;
}


/* Decoding secret string from stego image
 * Input: seccret string and Stego image file and output file
 * Output: Proceed to decoding process if secret string is matched
 * Return: e_success or e_failure
 */
Status decode_secret_string(char *password, DecodeInfo *decInfo)
{
    uint i;
    if (password != NULL)
    {
            for (i=0; i < decInfo->password_size; i++)
            {
                fread(decInfo->image_data, sizeof(char), 8, decInfo->fptr_stego_image);
                if (ferror(decInfo->fptr_stego_image))
                {
                    fprintf(stderr, "Error: Error reading the data from stego image\n");
                    clearerr(decInfo->fptr_stego_image);
                    return e_failure;
                }

                    if (decode_lsb_to_byte(decInfo->decode_data, decInfo->image_data) == e_success)
                    {
                            if (decInfo->decode_data[0] == magic_string[i])
                            {
                                continue;
                            }
                            else
                            {
                                fprintf(stderr,"Error: Incorrect passcode\n");
                                return e_failure;
                            }
                    }
                    else
                    {
                        fprintf(stderr, "Error: %s function failed\n","decode_lsb_to_byte()");
                        return e_failure;
                    }
                }
            }
            fread(decInfo->image_data, sizeof(char), 8, decInfo->fptr_stego_image);
            if (ferror(decInfo->fptr_stego_image))
            {
                fprintf(stderr,"Error: While reading the data from stego image file\n");
                clearerr(decInfo->fptr_stego_image);
                return e_failure;
            }
            if (decode_lsb_to_byte(decInfo->decode_data, decInfo->image_data) == e_success)
            {
                if (decInfo->decode_data[0] == '*')
                {
                        return e_success;
                }
                else
                {
                        fprintf(stderr,"Error: Incorrect passcode\n");
                        return e_failure;
                }
    }
    else
    {
        fprintf(stderr, "Error: %s function failed\n","decode_lsb_to_byte()");
        return e_failure;
    }
}


/* Decode file extension size from stego image
 * Input: stego image info and output file
 * Output: decode the extension size from stego image and store in image_data_size
 * Return: e_success or e_failure
 */
Status decode_output_file_extn_size(DecodeInfo *decInfo)
{
    char file_size[DECODE_FILE_EXTN_SIZE];
	fread(file_size, sizeof(char), DECODE_FILE_EXTN_SIZE, decInfo->fptr_stego_image);
	if (ferror(decInfo->fptr_stego_image))
    {
         fprintf(stderr,"Error: While reading the data from stego image file\n");
         clearerr(decInfo->fptr_stego_image);
         return e_failure;
    }

	decInfo->image_size = 0;
	for (uint i = 0; i < DECODE_FILE_EXTN_SIZE; i++)
	{
		decInfo->image_size <<= 1;
		decInfo->image_size |= (uint) (file_size[i] & 0x01);
	}
	if (decode_output_file_extn(decInfo->image_size, decInfo) == e_failure)
	{
		fprintf(stderr,"Error: %s function failed\n","decode_output_file_extn()");
		return e_failure;
	}
	return e_success;
}

/* Decode file extenstion from stego image
 * Input: Extenstion Size and File info of stego image
 * Output: Decodes the file extenstion and store in extn_output_file
 * Return: e_success or e_failure
 */
Status decode_output_file_extn(uint extn_size, DecodeInfo *decInfo)
{
	for (uint i = 0; i < extn_size; i++)
	{
		fread(decInfo->image_data, sizeof(char), MAX_IMAGE_BUF_SIZE, decInfo->fptr_stego_image);
		if (ferror(decInfo->fptr_stego_image))
    	{
        	fprintf(stderr,"Error: While reading the data from stego image file\n");
         	clearerr(decInfo->fptr_stego_image);
         	return e_failure;
    	}
		if (decode_lsb_to_byte(decInfo->decoded_data, decInfo->image_data) == e_success)
		{
			decInfo->output_file_extn[i] = decInfo->decoded_data[0];
		}
	}
	return e_success;
}


/* Decode file size from stego image
 * Input: FILE info of stego image and output file
 * Output: Decodes the file image and store in image_data_size
 * Return: e_success or e_failure
 */
Status decode_file_size(DecodeInfo *decInfo)
{
	char file_size[DECODE_FILE_SIZE];
	fread(file_size, sizeof(char), DECODE_FILE_SIZE, decInfo->fptr_stego_image);
	if (ferror(decInfo->fptr_stego_image))
    {
         fprintf(stderr,"Error: While reading the data from stego image file\n");
         clearerr(decInfo->fptr_stego_image);
         return e_failure;
    }
	decInfo->image_size = 0;
	for (uint i = 0; i < DECODE_FILE_SIZE; i++)
	{
		decInfo->image_size <<= 1;
		decInfo->image_size |= (uint) (file_size[i] & 0x01);
	}
	return e_success;
}


/* Decode file data from stego image
 * Input: FILE info of stego image and output decode file
 * Output: Write decode data in the output file
 * Return: e_success or e_failure
 */
Status decode_data_to_file(DecodeInfo *decInfo)
{
	for (uint i = 0; i < decInfo->image_size; i++)
	{
		fread(decInfo->image_data, sizeof(char), MAX_IMAGE_BUF_SIZE, decInfo->fptr_stego_image);
		if (ferror(decInfo->fptr_stego_image))
    	{
        	 fprintf(stderr,"Error: While reading the data from stego image file\n");
         	clearerr(decInfo->fptr_stego_image);
         	return e_failure;
    	}
		if(decode_lsb_to_byte(decInfo->decoded_data, decInfo->image_data) == e_success)
		{
			fwrite(decInfo->decoded_data, sizeof(char), 1, decInfo->fptr_output);
			if (ferror(decInfo->fptr_stego_image))
    		{
        		fprintf(stderr,"Error: While reading the data from stego image file\n");
         		clearerr(decInfo->fptr_stego_image);
         		return e_failure;
    		}

		}
		else
		{
			fprintf(stderr, "Error: %s function failed\n","decode_lsb_to_byte()");
			return e_failure;
		}
	}
	return e_success;
}


/* Get lsb bit from stego image byte
 * Input: Image_data array and decode_data character
 * Output: Decode the image_data and stores the 1 byte data in decode_data
 * Return: e_success or e_failure
 */
Status decode_lsb_to_byte(char *decode_data, char *image_data)
{
	decode_data[0] = 0;
	for (uint i = 0; i < MAX_IMAGE_BUF_SIZE; i++)
	{
		decode_data[0] <<= 1;
		decode_data[0] |=  (image_data[i] & 0x01);
	}
	return e_success;
}