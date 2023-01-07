#ifndef __PATH_H
#define __PATH_H

#include "String.h"
#include <Windows.h>
#include <stdio.h>

class Path
{
public:
#if defined(_WIN32) || defined(_WIN64)
	const static char Sep = '\\';
	const static char AltSep = '//';
#else
#endif
private:
	String baseDir;
public:
	Path();
	Path(String baseDir);
	String GetBaseDir();
	void SetBaseDir(String baseDir);
	String Transform(String relPath);

	static String GetImageLocation();
	static String GetDirectory(String path);
	static String GetParentDirectory(String path);
	static String GetFileNameWithoutExt(String fileName);
	static String GetFileNameExt(String fileName);
	static String GetFileName(String path);
	static String Combine(String p1, String p2);
	static String GetFiles(String path);
	static bool Exists(String path);
};

#endif