
#ifndef LSB_STEGANOGRAPHY_STEG_HEADER_H
#define LSB_STEGANOGRAPHY_STEG_HEADER_H
#include <stdio.h>

//read size of image file
int image_size(FILE *fp1);

//read size of secret file
int secret_size(FILE *fp2);

//bits of secret file
int get_txt_bits(char byte, int bit);



#endif //LSB_STEGANOGRAPHY_STEG_HEADER_H
