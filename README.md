# Lsb_steganography
This project demonstrates a proof of concept for hiding files inside images using the Least Significant Bit (LSB) 
steganography technique. It's a simple, yet powerful method of data hiding that's difficult for the human eye to detect.

### Description
The project is designed to hide files inside images by manipulating the least significant bits of the image's pixel 
data. This technique is particularly effective because it's difficult for the human eye to detect the changes made to 
the image, making it an ideal method for hiding sensitive information.

### Usage
To use the project, you'll need to provide the path to the image file and 
the file you want to hide inside the image. The program will then create a new image file with the hidden file
embedded inside it. To extract the hidden file, you'll need to use the appropriate command line arguments.

##### Encoding
```angular2html
lsb_steg -e <image_path> <file_to_hide> <output_image_name>
```
##### Decoding
```angular2html
lsb_steg -d <image_path> <output_file_name>
```

