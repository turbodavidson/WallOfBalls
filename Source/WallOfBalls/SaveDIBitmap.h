#include <windows.h>
#include <wingdi.h>

#include <conio.h>
#include <iomanip>
#include <iostream>
#include <new.h>
#include <typeinfo>

void SaveDIB(char *filename)
{
    // grab the current viewport
    GLint viewport[4]; // x, y, width, height
    DriverGL11::GetIntegerv( GL_VIEWPORT, viewport );

    std::auto_ptr<BITMAPINFO> info( reinterpret_cast<BITMAPINFO*>( new BITMAPINFOHEADER ) );


    long width = viewport[2] * 3; // Real width of scanline
    width = (width + 3) & ~3; // Align to 4 bytes
    long bitsize = width * viewport[3];
    GLubyte* pbits = new GLubyte [ bitsize ];

    DriverGL11::Finish();
    DriverGL11::PixelStorei( GL_PACK_ALIGNMENT, 4 ); // force 4-byte alignment
    DriverGL11::PixelStorei( GL_PACK_ROW_LENGTH, 0 ); // default
    DriverGL11::PixelStorei( GL_PACK_SKIP_ROWS, 0 ); // default
    DriverGL11::PixelStorei( GL_PACK_SKIP_PIXELS, 0 ); // default

    DriverGL11::ReadPixels( 0, 0, viewport[2], viewport[3], GL_RGB, GL_UNSIGNED_BYTE, pbits );

    // Swap the red and blue bits
    for( int i = 0; i < viewport[3]; i++ )
    {
        GLubyte* rgb = pbits + i * width;
        for( int j = 0; j < viewport[2]; j++, rgb += 3 )
        {
            std::swap( rgb[0], rgb[2] );
        }
    }

    info->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    info->bmiHeader.biWidth = viewport[2];
    info->bmiHeader.biHeight = viewport[3];
    info->bmiHeader.biPlanes = 1;
    info->bmiHeader.biBitCount = 24;
    info->bmiHeader.biCompression = BI_RGB;
    info->bmiHeader.biSizeImage = bitsize;
    info->bmiHeader.biXPelsPerMeter = 2952;
    info->bmiHeader.biYPelsPerMeter = 2952; // 75 dpi
    info->bmiHeader.biClrUsed = 0; // no colour indices
    info->bmiHeader.biClrImportant = 0;


    // Save the DIB
    FILE* fp = fopen( filename, "wb" );
    if( fp == NULL )
        return;

    long infosize = sizeof(BITMAPINFOHEADER);
    long size = sizeof(BITMAPFILEHEADER) + infosize + bitsize;

    BITMAPFILEHEADER bmfh;
    bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + infosize;
    bmfh.bfReserved1 = 0;
    bmfh.bfReserved2 = 0;
    bmfh.bfSize = size;
    bmfh.bfType = 'MB';

    fwrite( &bmfh, 1, sizeof( BITMAPFILEHEADER ), fp );
    fwrite( info.get(), 1, infosize, fp );
    fwrite( pbits, 1, bitsize, fp );
    fclose( fp );

    delete [] pbits;
}