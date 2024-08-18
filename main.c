#include <stdlib.h>
#include <stdio.h>
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

    // Write bmp contents to file
    bwrite(bmp, argv[2]);

    // Free memory
    bclose(bmp);

    return 0;
}
