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