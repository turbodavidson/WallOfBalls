#ifndef image_H
#define image_H

#include "interop.h"

class Image
{
public:
	enum {
		UnknownPixelFormat,
		Grayscale,
		RGB,
		BGR,
		BGRA,
		ARGB,
		RGBA,
	};

	static inline int GetBitsForFormat(int pixelFormat)
	{
		switch(pixelFormat) {
		case Grayscale:
			return 8;
		case RGB:
		case BGR:
			return 24;
		case BGRA:
		case ARGB:
		case RGBA:
			return 32;
		default:
			return 0;
		} 
	}

	virtual int GetPixelFormat() = 0;
	virtual int GetWidth() = 0;
	virtual int GetHeight() = 0;
	virtual int GetBpp();
	virtual int GetScanAlignment() = 0;

	virtual byte_t *Data() = 0;

	virtual bool ConvertToPixelFormat(int pixelFormat);
};

#endif
