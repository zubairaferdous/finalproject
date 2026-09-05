#ifndef IMAGE_H
#define IMAGE_H

/*
    One RGB pixel.
*/
typedef struct
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Pixel;

/*
    Image stored in memory as a top-to-bottom
    array of RGB pixels.
*/
typedef struct
{
    int width;
    int height;
    Pixel *data;
} Image;

Image *load_bmp(const char *filename);
int save_bmp(const char *filename, const Image *image);

Image *copy_image(const Image *source);
void free_image(Image *image);

#endif
