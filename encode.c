#include "stdio.h"
#define MAX_MESSAGE_LENGTH 100000000
#include "types.h"
#include "string.h"
#include "encode.h"

/* Check for operation type to be performed
 * Input: argv[] (command line argument)
 * Output: operation type (encode/decode)
 * Return integer value of operation type
 */
OperationType check_operation_type(char *argv[])
{
    if (strcmp(argv[1], "-e") == 0)
    {
        return e_encode;
    }
    else if (strcmp(argv[1], "-d") == 0)
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}

/* Validate command line arguments
 * Input: command line arguments count and arguments with file name info
 * Output: file names stored in encodeInfo structure
 * Return: Status of operation e_success or e_failure
 */
Status validate_encode_args(int argc, char *argv[], EncodeInfo *encInfo)
{
    //check if image is .bmp type
    if (argc > 2)
    {
        if (strcmp(strstr(argv[2], "."), ".bmp") == 0)
        {
            encInfo->src_image_fname = argv[2];
            printf("Image file name: %s\n", encInfo->src_image_fname);
        } else
        {
            fprintf(stderr,"Error: Image file is not .bmp type\n", argv[2]);
            return e_failure;
        }
    }
    else
    {
        fprintf(stderr, "Error: Arguments wrong or missing \n");
        printf("./lsb_steg: Encoding: ./lsb_steg encode <.bmp file> <.txt file> <output file>\n");
        printf("./lsb_steg: Decoding: ./lsb_steg decode <.bmp file> <output file>\n");
        return e_failure;
    }

    if (argc > 3)
    {
        // Copy text file extenstion in secret_file_extn
			strncpy(encInfo->secret_file_extn, strstr(argv[3], "."), 4);
			if ((strncmp(encInfo->secret_file_extn, ".txt", 4) == 0) || (strncmp(encInfo->secret_file_extn, ".c", 2) == 0) || (strncmp(encInfo->secret_file_extn
            , ".sh", 3) == 0))
			{
				encInfo->secret_fname = argv[3];
			}
			else
			{
				fprintf(stderr,"Error: Secrete file %s format should be .txt or .c or .sh\n", argv[3]);
				return e_failure;
			}
		}
		else
		{
				fprintf(stderr,"Error: Arguments are missing\n");
				printf("./lsb_steg: Encoding: ./lsb_steg -e <.bmp file> <.txt file> [Output file]\n");
				printf("./lsb_steg: Deconding: ./lsb_steg -d <.bmp file> [output file]\n");
				return e_failure;
    }
        if (argc > 4)
		{
			if (strcmp(strstr(argv[4],"."), ".bmp") == 0)
			{
				encInfo->stego_image_fname = argv[4];
			}
			else
			{
				fprintf(stderr,"Error: Output image %s format should be .bmp\n", argv[2]);
				return e_failure;
			}
		}
		else
		{
			encInfo->stego_image_fname = NULL;
		}

		/* Passcode validation*/
		if (argc > 5)
		{
			if (strcmp(argv[5], "-p") == 0)
			{
				if (argc > 6)
				{
					encInfo->password = argv[6];
					encInfo->password_size = strlen(encInfo->password);
				}
				else
				{
					fprintf(stderr,"Error: No passcode found\n");
					return e_failure;
				}
			}
			else
			{
				fprintf(stderr,"Error: Invalid option\n");
				return e_failure;
			}
		}
		else
		{
			encInfo->password = NULL;
			encInfo->password_size = 1;
		}

	return e_success;
}

/* Get image size
 * Input: Image file pointer
 * Output: Size of image in bytes (width * height * bits_per_pixel)
 * Description: In Bmp files, width is stored at offset 18,
 * and height is stored after(22) that. size is 4 bytes.
 */
uint image_size_for_bmp(FILE *fptr_image) {
    uint width, height, bits_per_pixel;

    //seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    //read width (an int)
    fread(&width, sizeof(int), 1, fptr_image);

    //read height (an int)
    fread(&height, sizeof(int), 1, fptr_image);

    //Read bits per pixel (short int)
    fseek(fptr_image, 2L, SEEK_CUR);
    fread(&bits_per_pixel, sizeof(short int), 1, fptr_image);
    rewind(fptr_image);

    //return max image capacity
    return width * height * bits_per_pixel;
}

/* Get file size in bytes
 * Input: File pointer
 * Output: Size of file in bytes
 */
uint get_file_size(FILE *fptr)
{
        uint size;
        fseek(fptr, 0L, SEEK_END);
        size = ftell(fptr);
        rewind(fptr);
        return size;
}

/* Check the capacity of source image to handle secret data
 * Input: File info source image, stego image and secret file
 * Output: Get Source image capacity and store in image_capacity
 * Return: e_success or e_failure
 */
Status check_capacity(EncodeInfo *encInfo)
{
	uint bytes_for_secret_file_size = 4;
	if (encInfo->image_capacity >= (MAX_IMAGE_BUF_SIZE * (encInfo->password_size + MAX_FILE_SUFFIX + (uint)sizeof(encInfo->secret_file_extn) + bytes_for_secret_file_size + encInfo->size_secret_file)))
	{
		return e_success;
	}
	return e_failure;
}

/* Encoding the secret data into stego image
 * Input: File info source image, stego image and secret file
 * Output: Encode the secret data into stego image
 * Return: e_success or e_failure
 */
Status do_encoding(EncodeInfo *encInfo)
{
    // open required files
    printf("INFO: Opening required files\n");
    if (open_encode_files(encInfo) == e_success)
    {
        printf("INFO: Opened required files\n");
        printf("INFO: ## Encoding in progress ##\n");
        // check size of secret file
        printf("INFO: Checking for size\n");
        encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
        if (encInfo->size_secret_file)
        {
            printf("INFO: Done. Size = %ld bytes\n", encInfo->size_secret_file);
            printf("INFO: Checking for %s capacity\n", encInfo->secret_fname);
            encInfo->image_capacity = image_size_for_bmp(encInfo->fptr_src_image);
                    if (check_capacity(encInfo) == e_success)
                    {
                        printf("INFO: Done, secret fits\n");

                        // check for output .bmp and open required files
                        if (encInfo->stego_image_fname == NULL)
                        {
                            printf("INFO: Output file not mentioned. Using default file name\n");
                            encInfo->stego_image_fname = "stego.bmp";
                            printf("INFO: Created stego.bmp file as default output file\n");
                                if (open_encode_files(encInfo) == e_failure)
                                {
                                    fprintf(stderr, "ERROR: %s function failed \n", "open_encode_files");
                                    return e_failure;
                                }
                        }
                        else
                        {
                            printf("INFO: Output file mentioned. Using %s as output file\n", encInfo->stego_image_fname);
                                if (open_encode_files(encInfo) == e_failure)
                                {
                                    fprintf(stderr, "ERROR: %s function failed \n", "open_encode_files");
                                    return e_failure;
                                }
                                printf("INFO: Opened required file %s\n", encInfo->stego_image_fname);
                        }

                        // copy bmp image header
                        printf("INFO: Copying image header\n");
                        if (copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
                        {
                            printf("INFO: Done, copied image header\n");

                            //Encode secret string in destination image
                            printf("INFO: Encoding secret string signature\n");
                                if(encode_secret_string(encInfo->password, encInfo) == e_success)
                                {
                                    printf("INFO: Done, encoded secret string signature\n");

                                        //Encode secret file extn size in destination image
                                        printf("INFO: Encoding secret file extension size\n");
                                        if (encode_secret_file_extn_size(encInfo) == e_success)
                                        {
                                            printf("INFO: Done, encoded secret file extension size\n");

                                                //Encode secret file extension
                                                printf("INFO: Encoding secret file extension\n");
                                                if (encode_secret_file_extn(encInfo) == e_success)
                                                {
                                                    printf("INFO: Done, encoded secret file extension\n");

                                                        //Encode secret file size
                                                        printf("INFO: Encoding %s File Size\n", encInfo->secret_fname);
                                                        if (encode_secret_file_size(encInfo) == e_success)
                                                        {
                                                            printf("INFO: Done, encoded %s File Size\n", encInfo->secret_fname);

                                                                //Encode secret file data
                                                                printf("INFO: Encoding %s File data\n", encInfo->secret_fname);
                                                                if (encode_secret_file_data(encInfo) == e_success)
                                                                {
                                                                    printf("INFO: Done, encoded %s File data\n", encInfo->secret_fname);

                                                                        // Copy remianing image data bytes from src to stego image
                                                                        printf("INFO: Copying remaining image data\n");
                                                                        if (copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
                                                                        {
                                                                            printf("INFO: Done, copied remaining image data\n");
                                                                            printf("INFO: ## Encoding done ##\n");
                                                                        }
                                                                        else
                                                                        {
                                                                            fprintf(stderr, "ERROR: %s function failed \n", "copy_remaining_img_data");
                                                                            return e_failure;
                                                                        }
                                                                }
                                                                else
                                                                {
                                                                    fprintf(stderr, "ERROR: %s function failed \n", "encode_secret_file_data");
                                                                    return e_failure;
                                                                }
                                                        }
                                                        else
                                                        {
                                                            fprintf(stderr, "ERROR: %s function failed \n", "encode_secret_file_size");
                                                            return e_failure;
                                                        }
                                                }
                                                else
                                                {
                                                    fprintf(stderr, "ERROR: %s function failed \n", "encode_secret_file_extn");
                                                    return e_failure;
                                                }

                                        }
                                        else
                                        {
                                            fprintf(stderr, "ERROR: %s function failed \n", "encode_secret_file_extn_size");
                                            return e_failure;
                                        }
                                }
                                else
                                {
                                    fprintf(stderr, "ERROR: %s function failed \n", "encode_secret_string");
                                }
                        }
                        else
                        {
                            fprintf(stderr, "ERROR: %s function failed \n", "copy_bmp_header");
                            return e_failure;
                        }
                    }
                    else
                    {
                        fprintf(stderr, "ERROR: %s function failed \n", "check_capacity");
                        return e_failure;
                    }
        }
        else
        {
            fprintf(stderr, "ERROR: %s function failed \n", "get_file_size");
            return e_failure;
        }
    }
    else
    {
        fprintf(stderr, "ERROR: %s function failed \n", "open_encode_files()");
        return 1;
    }

        return e_success;
}


/* Get File pointers for i/p and o/p files
 * Input: Image file, Secret file, Stego Image file
 * Output: File pointers to the above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_encode_files(EncodeInfo *encodeInfo)
{
    static int open_count = 0;
    if (open_count == 0)
    {
            open_count++;
            //open src image file
            encodeInfo->fptr_src_image = fopen(encodeInfo->src_image_fname, "r");
            // Do Error handling
            if (encodeInfo->fptr_src_image == NULL)
            {
                    perror("fopen");
                    fprintf(stderr, "ERROR: Unable to open file %s\n", encodeInfo->src_image_fname);
                    return e_failure;
            }
                else
                {
                    printf("INFO: Opened %s for reading\n", encodeInfo->src_image_fname);
                }
            //Secret file
            encodeInfo->fptr_secret = fopen(encodeInfo->secret_fname, "r");
            // Do Error handling
            if (encodeInfo->fptr_secret == NULL)
            {
                    perror("fopen");
                    fprintf(stderr, "ERROR: Unable to open file %s\n", encodeInfo->secret_fname);
                    return e_failure;
            }
                else
                {
                    printf("INFO: Opened %s for reading\n", encodeInfo->secret_fname);
                }
    }
    else
    {
     //open stego image file
                encodeInfo->fptr_stego_image = fopen(encodeInfo->stego_image_fname, "w");
                // Do Error handling
                if (encodeInfo->fptr_stego_image == NULL)
                {
                      perror("fopen");
                      fprintf(stderr, "ERROR: Unable to open file %s\n", encodeInfo->stego_image_fname);
                      return e_failure;
                }
                 else
                 {
                      printf("INFO: Opened %s for writing\n", encodeInfo->stego_image_fname);
                 }
    }

    // No failure return e_success
    return e_success;
}

/* Copy bmp image header to destination file
 * Input: File pointers to source and destination files
 * Output: Copy the bmp image header to destination file
 * Return Value: e_success or e_failure
 */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    // get source bmp image header
        uint meta_data;
        fseek(fptr_src_image, 10L, SEEK_SET);
        fread(&meta_data, sizeof(int), 1, fptr_src_image);
        rewind(fptr_src_image);

        char header_data[meta_data];
        fread(header_data, sizeof(char), (size_t)meta_data, fptr_src_image);
        if (ferror(fptr_src_image))
        {
                fprintf(stderr, "ERROR: %s function failed while reading data from source image\n", "fread");
                clearerr(fptr_src_image);
                return e_failure;
        }
        fwrite(header_data, sizeof(char), (size_t)meta_data, fptr_dest_image);
        if (ferror(fptr_dest_image))
        {
                fprintf(stderr, "ERROR: %s function failed while writing data to destination image\n", "fwrite");
                clearerr(fptr_dest_image);
                return e_failure;
        }
        return e_success;
}


/* Encode secret string signature into stego image
 * Input: Secret string, EncodeInfo structure
 * Output: Encoded secret string in stego image
 * Return Value: e_success or e_failure, on file errors
 */
Status encode_secret_string(const char *secret_string, EncodeInfo *encInfo)
{
        if (secret_string != NULL)
        {
                for (uint i = 0; i < encInfo->password_size; i++)
                {
                        // copy * to secret_data
                       if ((strncpy(encInfo->secret_data, (secret_string + i), 1)) == NULL)
                        {
                            return e_failure;
                        }
                        // Divide secrete_data to 8 bits i.e MSB at 0th index and LSB at 7th index
                        fread(encInfo->image_data, sizeof(char), MAX_IMAGE_BUF_SIZE, encInfo->fptr_src_image);
                        if (ferror(encInfo->fptr_src_image))
                        {
                            fprintf(stderr,"Error: While reading the data from source image file\n");
                            clearerr(encInfo->fptr_src_image);
                            return e_failure;
                        }
                        if (encode_byte_to_lsb(encInfo->secret_data[0], encInfo->image_data) == e_failure)
                        {
                            fprintf(stderr,"Error: %s function failed\n", "encode_byte_to_lsb()");
                            return e_failure;
                        }
                        fwrite(encInfo->image_data, sizeof(char), MAX_IMAGE_BUF_SIZE, encInfo->fptr_stego_image);
                        if (ferror(encInfo->fptr_stego_image))
                        {
                            fprintf(stderr,"Error: While writing the data to destination image file\n");
                            clearerr(encInfo->fptr_stego_image);
                            return e_failure;
                        }
                }
        }

        // Encode "*" after password or Enccode "*"
        if ((strncpy(encInfo->secret_data, secret_string, 1)) == NULL)
        {
            return e_failure;
        }

        //Divide secrete_data to 8 bits i.e MSB at 0th index and LSB at 7th index
        fread(encInfo->image_data, sizeof(char), MAX_IMAGE_BUF_SIZE, encInfo->fptr_src_image);
        if (ferror(encInfo->fptr_src_image))
        {
            fprintf(stderr,"Error: While reading the data from source image file\n");
            clearerr(encInfo->fptr_src_image);
            return e_failure;
        }
        if (encode_byte_to_lsb(encInfo->secret_data[0], encInfo->image_data) == e_failure)
        {
            fprintf(stderr,"Error: %s function failed\n", "encode_byte_to_lsb()");
            return e_failure;
        }
        fwrite(encInfo->image_data, sizeof(char), MAX_IMAGE_BUF_SIZE, encInfo->fptr_stego_image);
        if (ferror(encInfo->fptr_stego_image))
        {
            fprintf(stderr,"Error: While writing the data to destination image file\n");
            clearerr(encInfo->fptr_stego_image);
            return e_failure;
        }
        return e_success;

}

/* Encodes secret byte data to image 8 byte data
 * Input: Secret 1 byte data and Image 8 byte data
 * Output: Encode data to image_buffer
 * Return: e_success or e_failure
 */
Status encode_byte_to_lsb(char data, char *image_buffer)
{
        unsigned char mask = 0x80;
        for(uint i = 0; i < MAX_IMAGE_BUF_SIZE; i++)
        {
            if ( data & mask)
            {
                image_buffer[i] = (image_buffer[i] | (unsigned char) (0x01));
            }
            else
            {
                image_buffer[i] = (image_buffer[i] & (unsigned char) (~(0x01)));
            }
            mask >>= 1;
        }
        return e_success;
}


/* Encode secret file extension size into stego image
 * Input: EncodeInfo structure
 * Output: Encoded secret file extension size in stego image
 * Return Value: e_success or e_failure, on file errors
 */

//difference
Status encode_secret_file_extn_size(EncodeInfo *encInfo)
{
   uint mask = 0x80000000;
   uint len;
   for (len = 0; (len < MAX_FILE_SUFFIX) && (encInfo->secret_file_extn[len] != '\0'); len++);
   uint size_bits = MAX_IMAGE_BUF_SIZE * (uint)sizeof(int);
   char data[size_bits];
   //divide secret_data to 8 bits i.e MSB at 0th index and LSB at 7th index
    for (uint i = 0; i < size_bits; i++)
    {
         if (len & mask)
         {
              data[i] = (data[i] | (unsigned char) (0x01));
         }
         else
         {
              data[i] = (data[i] & (unsigned char) (~(0x01)));
         }
         mask >>= 1;
         data[i] = (char) ((char) (fgetc(encInfo->fptr_src_image) & (~(0x01))) | data[i]);

    }
    fwrite(data, sizeof(char), size_bits, encInfo->fptr_stego_image);
    if (ferror(encInfo->fptr_stego_image))
    {
        fprintf(stderr,"Error: While writing the data to destination image file\n");
        clearerr(encInfo->fptr_stego_image);
        return e_failure;
    }
    return e_success;
}


/* Encode secret file extension into stego image
 * Input: EncodeInfo structure
 * Output: Copy source image data to stego image data
 * Return Value: e_success or e_failure, on file errors
 */


Status encode_secret_file_extn(EncodeInfo *encInfo)
{
    uint len;
    for (len = 0; (len < MAX_FILE_SUFFIX) && (encInfo->secret_file_extn[len] != '\0'); len++);
	// Divide secrete_data to 8 bits i.e MSB at 0th index and LSB at ((len * 8) - 1)th index

	for (uint i = 0; i < len; i++)
	{
		fread(encInfo->image_data, sizeof(char), MAX_IMAGE_BUF_SIZE, encInfo->fptr_src_image);
		if (ferror(encInfo->fptr_src_image))
		{
			fprintf(stderr,"Error: While reading the data from source nimage file\n");
			clearerr(encInfo->fptr_src_image);
			return e_failure;
		}
		if (encode_byte_to_lsb(encInfo->secret_file_extn[i], encInfo->image_data) == e_failure)
		{
			fprintf(stderr,"Error: %s function failed\n", "encode_byte_to_lsb()");
			return e_failure;
		}
		fwrite(encInfo->image_data, sizeof(char), MAX_IMAGE_BUF_SIZE, encInfo->fptr_stego_image);
		if (ferror(encInfo->fptr_stego_image))
		{
			fprintf(stderr,"Error: While writing the data to destination image file\n");
			clearerr(encInfo->fptr_stego_image);
			return e_failure;
		}
	}
	return e_success;
}

/* Encode secret file size data to stego image
 * Input: Files info
 * Output: Encode secret file size data to stego image
 * Return: e_success or e_failure
 */
Status encode_secret_file_size(EncodeInfo *encInfo)
{
	uint mask = 0x80000000;
	uint len = (uint) encInfo->size_secret_file;
	uint size_bits = MAX_IMAGE_BUF_SIZE * (uint) sizeof(int);
	char data[size_bits];
	// Divide secrete_data to 8 bits i.e MSB at 0th index and LSB at 31th index
	for (int i = 0; i < size_bits; i++)
	{
		if ((uint) len & mask)
		{
			data[i] = 1;
		}
		else
		{
			data[i] = 0;
		}
		mask >>= 1;
		data[i] = (char) ((char) (fgetc(encInfo->fptr_src_image) & ~(0x01))| (data[i]));
	}
	fwrite(data, sizeof(char), (size_t) size_bits, encInfo->fptr_stego_image);
	if (ferror(encInfo->fptr_stego_image))
	{
 		fprintf(stderr,"Error: While writing the data to destination image file\n");
		clearerr(encInfo->fptr_stego_image);
		return e_failure;
	}
	return e_success;
}