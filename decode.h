#ifndef LSB_STEGANOGRAPHY_DECODE_H
#define LSB_STEGANOGRAPHY_DECODE_H

#include "types.h"
#include "common.h"

/* Struct for information required for decoding
 * the stego image to new file
 * info about output  */


#define DECODE_FILE_EXTN_SIZE 32

#define DECODE_FILE_SIZE 32

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
    char decoded_data[MAX_SECRET_BUF_SIZE]; // decoded data buffer
    char output_file_extn[MAX_FILE_SUFFIX]; // decoded file extension
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
/* Decode file extension size */
Status decode_output_file_extn_size(DecodeInfo *decInfo);
/* Decode file extension */
Status decode_output_file_extn(uint extn_size, DecodeInfo *decInfo);
/* Decode a array of LSB into byte */
Status decode_lsb_to_byte(char *decode_data, char *image_data);
/* Decoding the stego image */
Status do_decoding(DecodeInfo *decInfo);
/* Open Files required for decoding */
Status open_files_decode(DecodeInfo *decInfo);
/* Read and validate decode arguments argc & argv[] */
Status validate_decode_args(int argc, char *argv[], DecodeInfo *decInfo);


#endif //LSB_STEGANOGRAPHY_DECODE_H
