#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "qdbmp/qdbmp.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input file>\n", argv[0]);
        exit(1);
    }

    BMP *bmp;
    UCHAR r, g, b, a;
    UINT src_depth;
    UINT dst_width, dst_height, dst_depth;

    char *src_filename = argv[1];
    char *dst_filename = argv[2];

    /* Read an image file */
    bmp = BMP_ReadFile(src_filename);
    BMP_CHECK_ERROR(stderr, -1); /* If an error has occurred, notify and exit */

    /* Get image's dimensions */
    UINT src_width = BMP_GetWidth(bmp);
    UINT src_height = BMP_GetHeight(bmp);

    for (UINT x = 0; x < src_width; ++x) {
        for (UINT y = 0; y < src_height; ++y) {
            /* Get pixel's RGB values */
            BMP_GetPixelRGBA(bmp, x, y, &r, &g, &b, &a);

            if (a) {
                printf("%c", a);
            }
        }
    }

    /* Free all memory allocated for the image */
    BMP_Free(bmp);
    return 0;
}
