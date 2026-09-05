#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "image.h"

/*
    BMP file headers.
    The assignment only requires uncompressed 24-bit BMP.
*/
#pragma pack(push, 1)

typedef struct
{
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
} BMPFileHeader;

typedef struct
{
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bits_per_pixel;
    uint32_t compression;
    uint32_t image_size;
    int32_t x_pixels_per_meter;
    int32_t y_pixels_per_meter;
    uint32_t colors_used;
    uint32_t important_colors;
} BMPInfoHeader;

#pragma pack(pop)

Image *load_bmp(const char *filename)
{
    FILE *file;
    BMPFileHeader file_header;
    BMPInfoHeader info_header;
    Image *image;
    unsigned char *row;
    int width;
    int height;
    int bottom_up;
    int row_size;
    int x;
    int y;

    if (filename == NULL)
        return NULL;

    file = fopen(filename, "rb");
    if (file == NULL)
        return NULL;

    if (fread(&file_header, sizeof(file_header), 1, file) != 1 ||
        fread(&info_header, sizeof(info_header), 1, file) != 1)
    {
        fclose(file);
        return NULL;
    }

    if (file_header.type != 0x4D42 ||
        info_header.size < 40 ||
        info_header.planes != 1 ||
        info_header.bits_per_pixel != 24 ||
        info_header.compression != 0 ||
        info_header.width <= 0 ||
        info_header.height == 0)
    {
        fclose(file);
        return NULL;
    }

    width = info_header.width;

    if (info_header.height < 0)
    {
        height = -info_header.height;
        bottom_up = 0;
    }
    else
    {
        height = info_header.height;
        bottom_up = 1;
    }

    image = (Image *)malloc(sizeof(Image));
    if (image == NULL)
    {
        fclose(file);
        return NULL;
    }

    image->width = width;
    image->height = height;
    image->data = (Pixel *)malloc(
        (size_t)width * (size_t)height * sizeof(Pixel)
    );

    if (image->data == NULL)
    {
        free(image);
        fclose(file);
        return NULL;
    }

    row_size = ((width * 3 + 3) / 4) * 4;

    row = (unsigned char *)malloc((size_t)row_size);
    if (row == NULL)
    {
        free_image(image);
        fclose(file);
        return NULL;
    }

    if (fseek(file, (long)file_header.offset, SEEK_SET) != 0)
    {
        free(row);
        free_image(image);
        fclose(file);
        return NULL;
    }

    for (y = 0; y < height; y++)
    {
        int destination_y;

        if (fread(row, 1, (size_t)row_size, file) != (size_t)row_size)
        {
            free(row);
            free_image(image);
            fclose(file);
            return NULL;
        }

       if (bottom_up)
       {
        destination_y = height - 1 - y;
       }
       else
       {
        destination_y = y;
       }

        for (x = 0; x < width; x++)
        {
            Pixel *pixel = &image->data[
                destination_y * width + x
            ];

            /* BMP stores BGR, while the program stores RGB. */
            pixel->b = row[x * 3 + 0];
            pixel->g = row[x * 3 + 1];
            pixel->r = row[x * 3 + 2];
        }
    }

    free(row);
    fclose(file);

    return image;
}

int save_bmp(const char *filename, const Image *image)
{
    FILE *file;
    BMPFileHeader file_header;
    BMPInfoHeader info_header;
    unsigned char *row;
    int row_size;
    int x;
    int y;

    if (filename == NULL ||
        image == NULL ||
        image->data == NULL ||
        image->width <= 0 ||
        image->height <= 0)
    {
        return 0;
    }

    file = fopen(filename, "wb");
    if (file == NULL)
        return 0;

    row_size = ((image->width * 3 + 3) / 4) * 4;

    row = (unsigned char *)malloc((size_t)row_size);
    if (row == NULL)
    {
        fclose(file);
        return 0;
    }

    file_header.type = 0x4D42;
    file_header.size =
        (uint32_t)(sizeof(BMPFileHeader) +
                   sizeof(BMPInfoHeader) +
                   row_size * image->height);
    file_header.reserved1 = 0;
    file_header.reserved2 = 0;
    file_header.offset =
        sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);

    info_header.size = sizeof(BMPInfoHeader);
    info_header.width = image->width;
    info_header.height = image->height;
    info_header.planes = 1;
    info_header.bits_per_pixel = 24;
    info_header.compression = 0;
    info_header.image_size =
        (uint32_t)(row_size * image->height);
    info_header.x_pixels_per_meter = 0;
    info_header.y_pixels_per_meter = 0;
    info_header.colors_used = 0;
    info_header.important_colors = 0;

    if (fwrite(&file_header, sizeof(file_header), 1, file) != 1 ||
        fwrite(&info_header, sizeof(info_header), 1, file) != 1)
    {
        free(row);
        fclose(file);
        return 0;
    }

    /*
        Internal image is top-to-bottom.
        BMP is written bottom-to-top.
    */
    for (y = image->height - 1; y >= 0; y--)
    {
        for (x = 0; x < image->width; x++)
        {
            Pixel pixel =
                image->data[y * image->width + x];

            row[x * 3 + 0] = pixel.b;
            row[x * 3 + 1] = pixel.g;
            row[x * 3 + 2] = pixel.r;
        }

        /* Clear the row padding bytes. */
        for (x = image->width * 3; x < row_size; x++)
            row[x] = 0;

        if (fwrite(row, 1, (size_t)row_size, file) !=
            (size_t)row_size)
        {
            free(row);
            fclose(file);
            return 0;
        }
    }

    free(row);
    fclose(file);

    return 1;
}

Image *copy_image(const Image *source)
{
    Image *copy;
    size_t total_pixels;

    if (source == NULL ||
        source->data == NULL ||
        source->width <= 0 ||
        source->height <= 0)
    {
        return NULL;
    }

    copy = (Image *)malloc(sizeof(Image));
    if (copy == NULL)
        return NULL;

    copy->width = source->width;
    copy->height = source->height;

    total_pixels =
        (size_t)source->width * (size_t)source->height;

    copy->data =
        (Pixel *)malloc(total_pixels * sizeof(Pixel));

    if (copy->data == NULL)
    {
        free(copy);
        return NULL;
    }

    for (size_t i = 0; i < total_pixels; i++)
        copy->data[i] = source->data[i];

    return copy;
}

void free_image(Image *image)
{
    if (image == NULL)
        return;

    free(image->data);
    free(image);
}
