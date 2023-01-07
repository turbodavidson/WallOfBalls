#include "image.h"

typedef void (*ConvertPixelDelegate)(byte_t *);

static inline void ConvertPixelSwapRedBlue(byte_t *pix)
{
	byte_t ctmp = *(pix);
	*(pix) = *(pix + 2);
	*(pix + 2) = ctmp;
}

static inline void ConvertPixelBGRAARGB(byte_t *pix)
{
	byte_t ctmp = *(pix);
	*(pix) = *(pix + 3);
	*(pix + 3) = ctmp;
	ctmp = *(pix + 1);
	*(pix + 1) = *(pix + 2);
	*(pix + 2) = ctmp;
}

static inline void ConvertPixelARGBToRGBA(byte_t *pix)
{
	byte_t pixbuf[4];
	pixbuf[0] = *(pix);
	pixbuf[1] = *(pix + 1);
	pixbuf[2] = *(pix + 2);
	pixbuf[3] = *(pix + 3);
	*(pix) = pixbuf[1];
	*(pix + 1) = pixbuf[2];
	*(pix + 2) = pixbuf[3];
	*(pix + 3) = pixbuf[0];
}

static inline void ConvertPixelRGBAToARGB(byte_t *pix)
{
	byte_t pixbuf[4];
	pixbuf[0] = *(pix);
	pixbuf[1] = *(pix + 1);
	pixbuf[2] = *(pix + 2);
	pixbuf[3] = *(pix + 3);
	*(pix) = pixbuf[3];
	*(pix + 1) = pixbuf[0];
	*(pix + 2) = pixbuf[1];
	*(pix + 3) = pixbuf[2];
}

void Convert(int width, int height, int alignment, int colorBits, byte_t *data, ConvertPixelDelegate converter)
{
	const int bypp = colorBits / 8;
	int scanWidth = width * bypp;
	if(0 != (scanWidth % alignment)) {
		scanWidth = scanWidth + (alignment - (scanWidth % alignment));
	}
	for(int scanLine = 0; scanLine < height; scanLine++) {
		for(int ipixel = 0; ipixel < width; ipixel++) {
			(*converter)(data + (scanLine * scanWidth) + (ipixel * bypp));
		}
	}
}

bool Image::ConvertToPixelFormat(int pixelFormat)
{
	if(GetPixelFormat() == pixelFormat) {
		return true;
	} else if(0 != Data()) {
		if(((GetPixelFormat() == Image::RGB) && (Image::BGR == pixelFormat)) || ((GetPixelFormat() == Image::BGR) && (Image::RGB == pixelFormat))) {
			Convert(GetWidth(), GetHeight(), GetScanAlignment(), 24, Data(), ConvertPixelSwapRedBlue);
			return true;
		} else if(((GetPixelFormat() == Image::RGBA) && (Image::BGRA == pixelFormat)) || ((GetPixelFormat() == Image::BGRA) && (Image::RGBA == pixelFormat))) {
			Convert(GetWidth(), GetHeight(), GetScanAlignment(), 32, Data(), ConvertPixelSwapRedBlue);
			return true;
		} else if(((GetPixelFormat() == Image::BGRA) && (Image::ARGB == pixelFormat)) || ((GetPixelFormat() == Image::ARGB) && (Image::BGRA == pixelFormat))) {
			Convert(GetWidth(), GetHeight(), GetScanAlignment(), 32, Data(), ConvertPixelBGRAARGB);
			return true;
		} else if((GetPixelFormat() == Image::ARGB) && (Image::RGBA == pixelFormat)) {
			Convert(GetWidth(), GetHeight(), GetScanAlignment(), 32, Data(), ConvertPixelARGBToRGBA);
			return true;
		} else if((GetPixelFormat() == Image::RGBA) && (Image::ARGB == pixelFormat)) {
			Convert(GetWidth(), GetHeight(), GetScanAlignment(), 32, Data(), ConvertPixelRGBAToARGB);
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

int Image::GetBpp()
{
	return Image::GetBitsForFormat(GetPixelFormat());
}