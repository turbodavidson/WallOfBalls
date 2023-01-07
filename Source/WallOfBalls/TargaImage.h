#ifndef __TARGAIMAGE_H
#define __TARGAIMAGE_H

#include "Image.h"
#include "Interop.h"
#include "Stream.h"
#include "ResourceType.h"

class TargaImage : public Image, public ResourceType
{
public:
	enum {
		RgbType = 2,
		RgbCompressedType = 10
	};

private:
	byte_t  identSize;        // size of ID field that follows 18 byte header (0 usually)
    byte_t  colourMapType;    // type of colour map 0=none, 1=has palette
    byte_t  imageType;        // type of image 0=none,1=indexed,2=rgb,3=grey,+8=rle packed

    short colourMapStart;     // first colour map entry in palette
    short colourMapLength;    // number of colours in palette
    byte_t  colourMapBits;     // number of bits per palette entry 15,16,24,32

    short xstart;             // image x origin
    short ystart;             // image y origin
    short width;              // image width in pixels
    short height;             // image height in pixels
    byte_t  bits;             // image bits per pixel 8,16,24,32
    byte_t  descriptor;       // image descriptor bits (vh flip bits)

	int pixelFormat;
	byte_t *data;

private:
	void Dealloc();
	void Allocate(int width, int height, int pixelFormat);
public:
	TargaImage();
	~TargaImage();

	bool Create(int width, int height, int pixelFormat, int originX, int originY);
	bool Load(Stream &source);
	bool Save(Stream &source);
	int GetOriginX();
	int GetOriginY();
	void InvertScans();

	virtual int GetPixelFormat();
	virtual int GetWidth();
	virtual int GetHeight();
	virtual int GetScanAlignment();

	virtual byte_t *Data();
};

#endif