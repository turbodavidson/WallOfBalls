#ifndef __Font_h__
#define __Font_h__

#include <windows.h>
#include <gl/gl.h>

GLvoid BuildFont(GLvoid);
GLvoid KillFont(GLvoid);
GLvoid glPrint(GLint x, GLint y, int set, const char *fmt, ...);

#endif