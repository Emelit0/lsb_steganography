
#ifndef LSB_STEGANOGRAPHY_ENCODE_H
#define LSB_STEGANOGRAPHY_ENCODE_H

#include "types.h" // user defined types
#include <stdio.h> // common functions and constants
#include

typedef struct {
    /* Source image info */
    char *src_image_fname; // source image file name
    FILE *fptr_src_image; // source image file pointer
    uint image_capacity; // image capacity in bytes
    uint image_size; // image size in bytes
    char image_data[MAX_IMAGE_BUF_SIZE]; // image data buffer

    /* Secret file info */
    char *secret_fname; // secret file name
    FILE *fptr_secret; // secret file pointer
    uint secret_size; // secret file size in bytes
    char secret_data[MAX_SECRET_BUF_SIZE]; // secret data buffer
    char secret_file_extn[MAX_SECRET_FILE_EXTN_SIZE]; // secret file extension
    long size_secret_file; // size of secret file


    /* Stego image info */
    char *stego_image_fname; // stego image file name
    FILE *fptr_stego_image; // stego image file pointer

    /* password */
    char *password; // password
    uint password_size; // password size in bytes


} EncodeInfo;

#endif //LSB_STEGANOGRAPHY_ENCODE_H
