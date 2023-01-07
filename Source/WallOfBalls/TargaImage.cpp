#include "TargaImage.h"
#include <cstdlib>
#include <cstring>

TargaImage::TargaImage()
{
	data = 0;
	Dealloc();
}

TargaImage::~TargaImage()
{
	Dealloc();
}

void TargaImage::Dealloc()
{
	if(0 != data) {
		free(data);
		data = 0;
	}

	pixelFormat = 0;

	identSize = 0x00;
    colourMapType = 0x00;
    imageType = 0x00;

    colourMapStart = 0;
    colourMapLength = 0;
    colourMapBits = 0x00;

    xstart = 0;
    ystart = 0;
    width = 0;
    height = 0;
    bits = 0x00;
    descriptor = 0x00;
}

void TargaImage::Allocate(int width, int height, int pixelFormat)
{
	int bpp = Image::GetBitsForFormat(pixelFormat);
	if(0 == bpp % 8) {
		int bypp = bpp / 8;
		int stride = width * bypp;
		int bufferLength = stride * height;
		this->width = width;
		this->height = height;
		this->pixelFormat = pixelFormat;
		this->bits = bpp;
		data = reinterpret_cast<byte_t *>(malloc(bufferLength));
	}
}

bool TargaImage::Load(Stream &source)
{
	byte_t chunkHeader = 0x00;
	int currentPixel = 0;
	int totalPixels = 0;
	if(source.Readable()) {
		Dealloc();

		source.Read(&identSize, 0, 1);
		source.Read(&colourMapType, 0, 1);
		source.Read(&imageType, 0, 1);

		source.Read(reinterpret_cast<byte_t *>(&colourMapStart), 0, 2);
		source.Read(reinterpret_cast<byte_t *>(&colourMapLength), 0, 2);
		source.Read(&colourMapBits, 0, 1);

		source.Read(reinterpret_cast<byte_t *>(&xstart), 0, 2);
		source.Read(reinterpret_cast<byte_t *>(&ystart), 0, 2);
		source.Read(reinterpret_cast<byte_t *>(&width), 0, 2);
		source.Read(reinterpret_cast<byte_t *>(&height), 0, 2);
		source.Read(&bits, 0, 1);
		source.Read(&descriptor, 0, 1);

		if((TargaImage::RgbType == imageType) || (TargaImage::RgbCompressedType == imageType)) {
			if(24 == bits) {
				Allocate(width, height, Image::BGR);
			} else if(32 == bits) {
				Allocate(width, height, Image::BGRA);
			} else {
				Dealloc();
				return false;
			}
			int bypp = GetBpp() / 8;

			if(imageType == TargaImage::RgbType) {
				source.Read(data, 0, GetWidth() * GetHeight() * bypp);
			} else if(imageType == TargaImage::RgbCompressedType) {
				totalPixels = width * height;
				while(currentPixel < totalPixels) {
					if(source.EofGet()) {
						break;
					} else {
						source.Read(&chunkHeader, 0, 1);
						if(chunkHeader >= 128) {
							// rle chunk
							int runLength = (static_cast<int>(chunkHeader) - 127);
							int rlePixel = currentPixel;
							source.Read(data + (currentPixel * bypp), 0, bypp);
							currentPixel++;
							for(int i=0; i<(runLength - 1); i++) {
								data[(currentPixel * bypp) + 0] = data[(rlePixel * bypp) + 0];
								data[(currentPixel * bypp) + 1] = data[(rlePixel * bypp) + 1];
								data[(currentPixel * bypp) + 2] = data[(rlePixel * bypp) + 2];
								if(4 == bypp) {
									data[(currentPixel * bypp) + 3] = data[(rlePixel * bypp) + 3];
								}
								currentPixel++;
							}
						} else {
							// raw chunk
							int length = (static_cast<int>(chunkHeader) + 1);
							source.Read(data + (currentPixel * bypp), 0, bypp * length);
							currentPixel += length;
						}
					}
				}
			}
			imageType = TargaImage::RgbType;
			return true;
		} else {
			Dealloc();
			return false;
		}
	} else {
		Dealloc();
		return false;
	}
}

bool TargaImage::Save(Stream &source)
{
	byte_t rgbType = static_cast<byte_t>(TargaImage::RgbType);
	bool flipBack = false;
	int pixelFormatSave = GetPixelFormat();

	if(24 == GetBpp()) {
		ConvertToPixelFormat(Image::BGR);
	} else if(32 == GetBpp()) {
		ConvertToPixelFormat(Image::BGRA);
	} else {
		return false;
	}

	if(0 < GetOriginY()) {
		flipBack = true;
		InvertScans();
	}

	if(source.Writeable()) {

		source.Write(&identSize, 0, 1);
		source.Write(&colourMapType, 0, 1);
		source.Write(&rgbType, 0, 1);

		source.Write(reinterpret_cast<byte_t *>(&colourMapStart), 0, 2);
		source.Write(reinterpret_cast<byte_t *>(&colourMapLength), 0, 2);
		source.Write(&colourMapBits, 0, 1);

		source.Write(reinterpret_cast<byte_t *>(&xstart), 0, 2);
		source.Write(reinterpret_cast<byte_t *>(&ystart), 0, 2);
		source.Write(reinterpret_cast<byte_t *>(&width), 0, 2);
		source.Write(reinterpret_cast<byte_t *>(&height), 0, 2);
		source.Write(&bits, 0, 1);
		source.Write(&descriptor, 0, 1);

		source.Write(data, 0, GetWidth() * GetHeight() * (GetBpp() / 8));
		return true;
	}

	if(flipBack) {
		InvertScans();
	}

	ConvertToPixelFormat(pixelFormatSave);
}

int TargaImage::GetPixelFormat()
{
	return pixelFormat;
}

int TargaImage::GetWidth()
{
	return width;
}

int TargaImage::GetHeight()
{
	return height;
}

byte_t *TargaImage::Data()
{
	return data;
}

int TargaImage::GetOriginX()
{
	return xstart;
}

int TargaImage::GetOriginY()
{
	return ystart;
}

int TargaImage::GetScanAlignment()
{
	return 1;
}

void TargaImage::InvertScans()
{
	if(0 != data) {
		int bypp = Image::GetBitsForFormat(pixelFormat) / 8;
		byte_t *invertedData = (byte_t *)malloc(width * height * bypp);
		int stride = width * bypp;
		for(int i=0; i<height; i++) {
			memcpy(invertedData + ((height - i - 1) * stride), data + (i * stride), stride);
		}
		free(data);
		data = invertedData;
		if(ystart == height) {
			ystart = 0;
		} else {
			ystart = height;
		}
	}
}

bool TargaImage::Create(int width, int height, int pixelFormat, int originX, int originY)
{
	Dealloc();
	if(	(pixelFormat == Image::RGB) || (pixelFormat == Image::BGR) ||
		(pixelFormat == Image::RGBA) || (pixelFormat == Image::BGRA) || (pixelFormat == Image::ARGB)) {
		Allocate(width, height, pixelFormat);
		this->width = width;
		this->height = height;
		this->bits = Image::GetBitsForFormat(pixelFormat);
		this->pixelFormat = pixelFormat;
		this->xstart = originX;
		this->ystart = originY;
		return true;
	} else {
		return false;
	}
}