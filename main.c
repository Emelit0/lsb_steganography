// Implementation of a c program tha uses lsb steganography to hide a message in a bitmap image

#define MAX_MESSAGE_LENGTH 10000
#define SECRET_KEY_LENGTH 32
#define MAX_FILENAME_LENGTH 100
#define MAX_FILE_EXTENSION_LENGTH 10
#define MAX_FILEPATH_LENGTH 256
#define MAX_FILESIZE 1000000


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>



int main() {
 char filePath[MAX_FILEPATH_LENGTH];

    printf("Enter the path of the image file: ");
    fgets(filePath, MAX_FILESIZE, stdin);

    //remove trailing newline
    filePath[strcspn(filePath, "\n")] = '\0';

    //open the file
    FILE *imageFile = fopen(filePath, "rb");
    if (imageFile == NULL) {
        printf("Error: could not open file\n");
        return 1;
    }

    //read the file data
    fseek(imageFile, 0, SEEK_END);
    long fileSize = ftell(imageFile);
    fseek(imageFile, 0, SEEK_SET);

    //allocate memory for the file content
    unsigned char *fileContent = malloc(fileSize);
    if (fileContent == NULL) {
        printf("Error: could not allocate memory for file\n");
        return 1;
    }

    size_t result = fread(fileContent, 1, fileSize, imageFile);
    if (result != fileSize) {
        printf("Error: could not read file\n");
        fclose(imageFile);
        free(fileContent);
        return 1;
    }

    fclose(imageFile);
    free(fileContent);

    return 0;


}

