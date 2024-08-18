#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "qdbmp/qdbmp.h"

char* read_file(const char* filename, long* size) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Could not open file %s\n", filename);
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    *size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* buffer = malloc(*size + 1);
    if (!buffer) {
        printf("Could not allocate memory\n");
        return NULL;
    }
    fread(buffer, *size, 1, file);
    fclose(file);
    return buffer;
}

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s <input file> <output file> <secret file>\n", argv[0]);
        exit(1);
    }

    BMP*    bmp;
    BMP*    dst_bmp;
    UCHAR   r, g, b;
    UINT    src_width, src_height, src_depth;
    UINT    dst_width, dst_height, dst_depth;
    UINT    x, y;

    char *src_filename = argv[1];
    char *dst_filename = argv[2];
    char *secret_filename = argv[3];

    long secret_size;
    char* secret_buffer = read_file(secret_filename, &secret_size);

    printf("secret size: %ld\n", secret_size);

    /* Read an image file */
    bmp = BMP_ReadFile( src_filename );
    BMP_CHECK_ERROR( stderr, -1 ); /* If an error has occurred, notify and exit */

    /* Get image's dimensions */
    src_width = BMP_GetWidth( bmp );
    src_height = BMP_GetHeight( bmp );
    src_depth = BMP_GetDepth( bmp );
    int src_capacity = src_width * src_height;

    printf("src_width = %d, ", src_width);
    printf("src_height = %d, ", src_height);
    printf("src_depth = %d, ", src_depth);
    printf("src_capacity = %d\n", src_capacity);

    int tile_count = 2; // must be multiple of 2

    if (secret_size > src_capacity) {
        int multiplier = 2;
        while (secret_size > (src_capacity * multiplier)) {
            multiplier += 2;
        }
        tile_count = 2;
    }
    printf("tile_count = %d\n", tile_count);

    dst_bmp = BMP_Create(src_width * (tile_count / 2), src_height * (tile_count / 2), 32);
    BMP_CHECK_ERROR( stderr, -1 );

    dst_width = BMP_GetWidth( dst_bmp );
    dst_height = BMP_GetHeight( dst_bmp );
    dst_depth = BMP_GetDepth( dst_bmp );

    printf("dst_width = %d, ", dst_width);
    printf("dst_height = %d, ", dst_height);
    printf("dst_depth = %d\n", dst_depth);

    int dst_capacity = dst_width * dst_height;
    printf("capacity = %d\n", dst_capacity);

    /* Iterate through all the image's pixels and tile them*/
    int x_tile_i = 0;
    int y_tile_i = 0;
    int secret_buffer_i = 0;

    for (x_tile_i = 0; x_tile_i < (tile_count / 2); x_tile_i++) {
        for (y_tile_i = 0; y_tile_i < (tile_count / 2); y_tile_i++) {
            for ( x = 0; x < src_width ; ++x ) {
                for ( y = 0 ; y < src_height ; ++y )
                {
                    /* Get pixel's RGB values */
                    BMP_GetPixelRGB( bmp, x, y, &r, &g, &b );

                    /* Invert RGB values */
                    int new_x = x + (x_tile_i * src_width);
                    int new_y = y + (y_tile_i * src_height);
                    UCHAR a = 0;

                    if (secret_buffer_i < secret_size) {
                        a = secret_buffer[secret_buffer_i++];
                    }

                    BMP_SetPixelRGBA( dst_bmp, new_x, new_y, r, g, b, a);
                }
            }
        }
    }

    //
    //BMP_SetPixelRGB( dst_bmp, 0, 0, 255, 253, 123); //7B FD FF // 0042A440
   // BMP_SetPixelRGB( dst_bmp, 1, 0, 255, 0, 0); // 00 00 FF
    //

    /* Save result */
    BMP_WriteFile( dst_bmp, dst_filename );
    BMP_CHECK_ERROR( stderr, -2 );

    /* Free all memory allocated for the image */
    BMP_Free( bmp );
    BMP_Free( dst_bmp );
    free(secret_buffer);
    return 0;
}
