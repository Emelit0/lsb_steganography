#include "stdio.h"
// fp1 = image file
// fp2 = secret file
// fp3 = output file

int size_of_image(FILE *filePath) {
    int width, height;

    fseek(filePath, 0x12, SEEK_SET); // seek to width position
    fread(&width, sizeof(int), 1, filePath); // read width
    fread(&height, sizeof(int), 1, filePath); // read height
    printf("The size of the image is: %d x %d\n", width, height);
    fseek(filePath, 0, SEEK_SET); // seek back to beginning of file

    return ((width * height) * 3 / 8);
}

void write_secret(FILE *secret) {
    char ch;
    while ((ch = getchar()) != EOF) {
        fputc(ch, secret); //write secret into file
    }
}

// get size of secret file
int secret_size(FILE *secret) {
    int size;
    fseek(secret, 0, SEEK_END); // seek to end of file
    size = ftell(secret); // get current file pointer
    fseek(secret, 0, SEEK_SET); // seek back to beginning of file
    return size;
}

void hide_secret(const char *inputPath, const char *secretPath, const char *outputPath) {

};

void readBMPHeader(FILE *filePath) {
};

void writeBMPHeader(FILE *filePath) {
};

// read a byte from a pixel
unsigned char readByte(const unsigned char *pixel) {
};

//read a bit from a pixel
unsigned char readBit(unsigned char byte) {
};

//hide trhe byte in the pixel
void hideByte(unsigned char *pixel, unsigned char byte) {
};

//hide the bit in the byte
void hideBit(unsigned char *byte, unsigned char bit) {
};

int Encoding(char *filePath, char *secretMessage) {

    //open the file
    FILE *imageFile = fopen(filePath, "rb");
    if (imageFile == NULL) {
        printf("Error: could not open file 1\n");
        return 1;
    }



    return 0;
}

