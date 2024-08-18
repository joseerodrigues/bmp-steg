#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cbmp/cbmp.h"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
        exit(1);
    }

    // Read image into BMP struct
    BMP* bmp = bopen(argv[1]);

    printf("src bit depth: %d\n", bmp->depth);

    BMP* bmp_copy = b_deep_copy(bmp);
    bmp_copy->depth = 32;

    printf("dst bit depth: %d\n", bmp_copy->depth);

    bmp_copy->width = bmp->width * 2;
    bmp_copy->height = bmp->height * 2;


    bmp_copy->pixels = (pixel*) malloc(bmp_copy->width * bmp_copy->height * sizeof(pixel));
    memcpy(bmp_copy->pixels, bmp->pixels, bmp->width * bmp->height * sizeof(pixel));

    bmp_copy->file_byte_number = bmp_copy->file_byte_number * 2 + (bmp_copy->width * bmp_copy->height * sizeof(unsigned char));
    bmp_copy->file_byte_contents = (unsigned char*) malloc(bmp_copy->file_byte_number * sizeof(unsigned char));

    memcpy(bmp_copy->file_byte_contents, bmp->file_byte_contents, bmp->file_byte_number * sizeof(unsigned char));

    /*
    unsigned int x, y, width, height;
    unsigned char r, g, b;

    // Gets image width in pixels
    width = get_width(bmp);

    // Gets image height in pixels
    height = get_height(bmp);

    for (x = 0; x < width; x++)
    {
        for (y = 0; y < height; y++)
        {
            // Gets pixel rgb values at point (x, y)
            get_pixel_rgb(bmp, x, y, &r, &g, &b);

            // Sets pixel rgb values at point (x, y)
            set_pixel_rgb(bmp, x, y, 255 - r, 255 - g, 255 - b);
        }
    }


*/
    // Write bmp contents to file
    bwrite(bmp_copy, argv[2]);

    // Free memory
    bclose(bmp);
    bclose(bmp_copy);

    return 0;
}
