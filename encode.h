
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


/* Check operation type */
OperationType check_operation_type(char *argv[]);
/* Check for encode operation */
Status check_operation(char *argv[]);
/* Read and validate Encode args from argv */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo);
/* Perform the encoding */
Status do_encoding(EncodeInfo *encInfo);
/* Get File pointers for i/p and o/p files */
Status open_encode_files(EncodeInfo *encInfo);
/* Get image size */
uint get_image_size_for_bmp(FILE *fptr_image);
/* Get file size */
uint get_file_size(FILE *fptr);
/* Copy bmp image header */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image);
/* Encode Magic String */
Status encode_secret_string(const char *magic_string, EncodeInfo *encInfo);
/* Encode secret file extenstion */
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo);
/* Encode secret file size */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo);
/* Encode password */
Status encode_password(const char *password, EncodeInfo *encInfo);
/* Encode secret file data*/
Status encode_secret_file_data(EncodeInfo *encInfo);
/* Encode a byte into LSB of image data array */
Status encode_byte_tolsb(char data, char *image_buffer);
/* Encode secret file data size */
Status encode_secret_file_data_size(long file_size, EncodeInfo *encInfo);
/* Encode a byte into LSB of image data array */
Status encode_byte_tolsb(char data, char *image_buffer);
/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest);
/* Encode a byte into LSB of image data array */
Status encode_byte_tolsb(char data, char *image_buffer);

#endif //LSB_STEGANOGRAPHY_ENCODE_H
