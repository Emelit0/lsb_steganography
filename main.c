// Implementation of a c program tha uses lsb steganography to hide a message in a bitmap image

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "types.h"
#include "encode.h"
#include "decode.h"


int main(int argc, char **argv)
{
        // create secret file
       // createSecret();

            // validate input arguments
            if (argc > 1 && argc < 8) {
                //validate operation

                //Encoding
                if ((check_operation(argv)) == e_encode) {
                    EncodeInfo encInfo;

                    //read file name
                    if (validate_encode_args(argc, argv, &encInfo) == e_success) {
                        //encode secret data
                        if (encode(&encInfo) == e_failure) {
                            fprintf(stderr, "Error: Encoding failed\n", "encode() function");
                            return 1;
                        }
                        fclose(encInfo.fptr_src_image);
                        fclose(encInfo.fptr_secret);
                        fclose(encInfo.fptr_stego_image);
                        printf("INFO: ## Encoding Done Successfully ##\n");
                    } else {
                        fprintf(stderr, "Error: Encoding failed\n", "validate_encode_args() function");
                        return 1;
                    }
                }
                    // Decoding
                else if (check_operation(argv) == e_decode) {
                    DecodeInfo decInfo;

                    // read and store file names
                    if (validate_decode_args(argc, argv, &decInfo) == e_success) {
                        //decode secret data
                        if (decode(&decInfo) == e_success) {
                            fclose(decInfo.fptr_stego_image);
                            fclose(decInfo.fptr_secret);
                            printf("INFO: ## Decoding Done Successfully ##\n");
                        } else {
                            fprintf(stderr, "Error: Decoding failed\n", "decode() function");
                            return 1;
                        }
                    } else {
                        fprintf(stderr, "Error: Decoding failed\n", "validate_decode_args() function");
                        return 1;
                    }
                } else {
                    fprintf(stderr, "Error: Invalid operation\n");
                    printf("Usage:\n");
                    printf("\tEncoding: ./lsb_steg -e <.bmp file> <.txt file> <.bmp file>\n");
                    printf("\tDecoding: ./lsb_steg -d <.bmp file> <.txt file>\n");
                    return 1;
                }
            }
            return 0;
        }
