#ifndef LSB_STEGANOGRAPHY_DECODE_H
#define LSB_STEGANOGRAPHY_DECODE_H

#include "types.h"
#include <stdio.h>

/* Struct for information required for decoding
 * the stego image to new file
 * info about output  */

// file extension size by decoding 32 bytes
#define DECODE_FILE_EXTN_SIZE 32
// get file size by decoding 32 bytes
#define DECODE_BUF_SIZE 32

typedef struct
{
//    Encoded stego image info
    char *stego_image_fname; // stego image file name
    FILE *fptr_stego_image; // stego image file pointer
    uint image_capacity; // image capacity in bytes
    uint image_size; // image size in bytes
    char image_data[MAX_IMAGE_BUF_SIZE]; // image data buffer

//    Decoded file info
    char *output_fname; // decoded file name
    FILE *fptr_output; // decoded file pointer
    char decoded_data[MAX_DECODE_BUF_SIZE]; // decoded data buffer
    char output_file_extn[MAX_DECODE_FILE_EXTN_SIZE]; // decoded file extension
    long size_decoded_file; // size of decoded file

//    Password
    char *password; // password
    uint password_size; // password size in bytes

} DecodeInfo;



/* Decode secret string */
Status decode_secret_string(char *secret_string, DecodeInfo *decInfo);
/* Decode the file size */
Status decode_file_size(DecodeInfo *decInfo);
/* Decode the data to the output file */
Status decode_data_to_file(DecodeInfo *decInfo);



#endif //LSB_STEGANOGRAPHY_DECODE_H
