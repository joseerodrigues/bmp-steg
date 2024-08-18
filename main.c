#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "qdbmp/qdbmp.h"

int main(int argc, char **argv)
{
    if (argc != 3)
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

    /* Read an image file */
    bmp = BMP_ReadFile( argv[ 1 ] );
    BMP_CHECK_ERROR( stderr, -1 ); /* If an error has occurred, notify and exit */

    /* Get image's dimensions */
    src_width = BMP_GetWidth( bmp );
    src_height = BMP_GetHeight( bmp );
    src_depth = BMP_GetDepth( bmp );

    printf("src_width = %d, ", src_width);
    printf("src_height = %d, ", src_height);
    printf("src_depth = %d\n", src_depth);

    dst_bmp = BMP_Create(src_width * 2, src_height * 2, 32);
    BMP_CHECK_ERROR( stderr, -1 );

    dst_width = BMP_GetWidth( dst_bmp );
    dst_height = BMP_GetHeight( dst_bmp );
    dst_depth = BMP_GetDepth( dst_bmp );

    printf("dst_width = %d, ", dst_width);
    printf("dst_height = %d, ", dst_height);
    printf("dst_depth = %d\n", dst_depth);

    int dst_capacity = dst_width * dst_height;
    printf("capacity = %d\n", dst_capacity);

    /* Iterate through all the image's pixels */
    for ( x = 0 ; x < src_width ; ++x )
    {
        for ( y = 0 ; y < src_height ; ++y )
        {
            /* Get pixel's RGB values */
            BMP_GetPixelRGB( bmp, x, y, &r, &g, &b );

            /* Invert RGB values */
            BMP_SetPixelRGB( dst_bmp, x, y, r, g, b);
        }
    }

    for ( x = 0 ; x < dst_width ; ++x )
    {
        for ( y = 0 ; y < dst_height ; ++y )
        {
            /* Get pixel's RGB values */
            BMP_GetPixelRGB( dst_bmp, x, y, &r, &g, &b );

            /* Invert RGB values */
            BMP_SetPixelRGBA( dst_bmp, x, y, r, g, b, 'g');
        }
    }

    //
 //   BMP_SetPixelRGB( bmp_copy, 0, 0, 255, 253, 123); //7B FD FF // 0042A440
  //  BMP_SetPixelRGB( bmp_copy, 1, 0, 255, 0, 0); // 00 00 FF
    //

    /* Save result */
    BMP_WriteFile( dst_bmp, argv[ 2 ] );
    BMP_CHECK_ERROR( stderr, -2 );

    /* Free all memory allocated for the image */
    BMP_Free( bmp );
    BMP_Free( dst_bmp );
    return 0;
}
