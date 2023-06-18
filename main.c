// Implementation of a c program tha uses lsb steganography to hide a message in a bitmap image

#define MAX_MESSAGE_LENGTH 10000
#define SECRET_KEY_LENGTH 32
#define MAX_FILENAME_LENGTH 100
#define MAX_FILE_EXTENSION_LENGTH 10
#define MAX_FILEPATH_LENGTH 256
#define MAX_FILESIZE 100000000


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>


int readImageFile(char *filePath) {

    //open the file
    FILE *imageFile = fopen(filePath, "rb");
    if (imageFile == NULL) {
        printf("Error: could not open file 1\n");
        return 1;
    }

    //read the file data
    fseek(imageFile, 0, SEEK_END);
    long fileSize = ftell(imageFile);
    fseek(imageFile, 0, SEEK_SET);

    //allocate memory for the file content
    unsigned char *fileContent = malloc(fileSize);
    if (fileContent == NULL) {
        printf("Error: could not allocate memory for file 2\n");
        return 1;
    }

    size_t image_data = fread(fileContent, 1, fileSize, imageFile);
    if (image_data != fileSize) {
        printf("Error: could not read file 3\n");
        fclose(imageFile);
        free(fileContent);
        return 1;
    }

    printf("file successfully read\n");

    fclose(imageFile);
    free(fileContent);
}

char readTextInput() {
    char path[] = "/home/emelito/Downloads/";
    char message[MAX_MESSAGE_LENGTH];
    printf("Enter the message you want to hide: ");
    fgets(message, MAX_MESSAGE_LENGTH, stdin);

    //remove trailing newline
    message[strcspn(message, "\n")] = '\0';
}

int writeTextFile(char *message) {

}

int main() {
    char filePath[MAX_FILEPATH_LENGTH];
    FILE *destination_file;

    printf("Enter the path of the image file: ");
    fgets(filePath, MAX_FILESIZE, stdin);

    printf("Enter the destination path of the manipulated image file:");
    scanf("%s", filePath);

    //need to make a copy of the selected file and store the manipulated file in a different location!!!
    //open the destination file for writing
    destination_file = fopen(filePath, "wb");
    if (destination_file == NULL) {
        printf("Error: could not open file 1\n");
        fclose(destination_file);
        exit(EXIT_FAILURE);
    }

    //remove trailing newline
    filePath[strcspn(filePath, "\n")] = '\0';

    printf("%s", filePath);

    readImageFile(filePath);

    char secret = readTextInput();
    int *message = (int *) &secret;

    writeTextFile((char *) message);


    return 0;
}

