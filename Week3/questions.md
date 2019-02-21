# Questions

## What's `stdint.h`?

A C library which contains definitions for ints.

## What's the point of using `uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` in a program?

Unsigned integers take only positive numbers, and cannot overflow, instead they wrap around to the next value.

## How many bytes is a `BYTE`, a `DWORD`, a `LONG`, and a `WORD`, respectively?

A BYTE is one byte (8 bits), DWORD and LONG are 4 bytes (32 bit), and WORD is 2 bytes  (16 bit).

## What (in ASCII, decimal, or hexadecimal) must the first two bytes of any BMP file be? Leading bytes used to identify file formats (with high probability) are generally called "magic numbers."

First two bytes are BM (ASCII) and these are specified in the bitmap file header.

## What's the difference between `bfSize` and `biSize`?

bfSize is the size of the file and biSize is the size of the file header.

## What does it mean if `biHeight` is negative?

The image is bottom up is the biHeight is negative.

## What field in `BITMAPINFOHEADER` specifies the BMP's color depth (i.e., bits per pixel)?

biBitCount specifies the bits per pixel.

## Why might `fopen` return `NULL` in `copy.c`?

fopen returns NULL when it cannot open the file, one possibility is due to read rights.

## Why is the third argument to `fread` always `1` in our code?

We are reading 1 byte at a time (reading each pixel at a time)

## What value does `copy.c` assign to `padding` if `bi.biWidth` is `3`?

When bi.biWidth is 3, then padding is assigned a value of 3.
int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

## What does `fseek` do?

Sets new position based on the offset of the origin value.

## What is `SEEK_CUR`?

Sets current position as the location of the origin to be offset.
