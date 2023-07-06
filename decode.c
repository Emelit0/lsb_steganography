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
        }
}