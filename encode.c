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




//void write_secret(FILE *secret) {
//    char ch;
//    while ((ch = getchar()) != EOF) {
//        fputc(ch, secret); //write secret into file
//    }
//}
//
//// get size of secret file
//int secret_size(FILE *secret) {
//    int size;
//    fseek(secret, 0, SEEK_END); // seek to end of file
//    size = ftell(secret); // get current file pointer
//    fseek(secret, 0, SEEK_SET); // seek back to beginning of file
//    return size;
//}
//
//
//void readBMPHeader(FILE *filePath, unsigned char *header) {
//    fseek(filePath, 0, SEEK_SET);
//    fread(header, sizeof(unsigned char), 54, filePath);
//};
//
//void writeBMPHeader(FILE *filePath , FILE *outputFile) {
//    fseek(filePath, 0, SEEK_SET);
//    unsigned char header[54];
//    fread(header, sizeof(unsigned char), 54, filePath);
//    fwrite(header, sizeof(unsigned char), 54, outputFile);
//};



