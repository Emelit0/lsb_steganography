#include "stdio.h"
// fp1 = image file
// fp2 = secret file
// fp3 = output file
#define MAX_MESSAGE_LENGTH 100000000

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


void readBMPHeader(FILE *filePath, unsigned char *header) {
    fseek(filePath, 0, SEEK_SET);
    fread(header, sizeof(unsigned char), 54, filePath);
};

void writeBMPHeader(FILE *filePath , FILE *outputFile) {
    fseek(filePath, 0, SEEK_SET);
    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, filePath);
    fwrite(header, sizeof(unsigned char), 54, outputFile);
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

void hide_secret(const char *inputPath, const char *secretPath, const char *outputPath) {
    FILE *inputFile = fopen(inputPath, "rb");
    FILE *secretFile = fopen(secretPath, "r");
    FILE *outputFile = fopen(outputPath, "wb");

    if (!inputFile || !secretFile || !outputFile) {
        printf("Error opening files.\n");
        return;
    }

    unsigned char header[54];
    readBMPHeader(inputFile, header);
    writeBMPHeader(inputFile, outputFile);

    unsigned char pixel[3];
    int messageIndex = 0;
    int bitIndex = 0;
    unsigned char byte = fgetc(secretFile);
    unsigned char bit;

    while (fread(pixel, sizeof(unsigned char), 3, inputFile) == 3) {
        if (messageIndex < MAX_MESSAGE_LENGTH) { // max size has to be calculated by checking the size of the image !! no hard coding
            bit = (byte >> bitIndex) & 1;
            hideBit(&pixel[0], bit);

            bitIndex++;
            if (bitIndex == 8) {
                bitIndex = 0;
                byte = fgetc(secretFile);
                messageIndex++;
            }
        }

        fwrite(pixel, sizeof(unsigned char), 3, outputFile);
    }

    fclose(inputFile);
    fclose(secretFile);
    fclose(outputFile);
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

