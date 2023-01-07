#include "path.h"

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#endif

Path::Path()
{
	SetBaseDir(Path::GetImageLocation());
}

Path::Path(String baseDir)
{
	SetBaseDir(baseDir);
}

String Path::GetBaseDir()
{
	return baseDir;
}

void Path::SetBaseDir(String baseDir)
{
	this->baseDir = baseDir;
}

String Path::Transform(String relPath)
{
	return Combine(baseDir, relPath);
}

#if defined(_WIN32) || defined(_WIN64)
String Path::GetImageLocation()
{
	char buffer[1024];
	ZeroMemory(buffer, 1024);
	GetModuleFileNameA(NULL, buffer, 1024);
	return GetDirectory(buffer);
}
#else
#endif

String Path::GetDirectory(String path)
{
	path = path.Replace(Path::AltSep, Path::Sep);
	int index = path.LastIndexOf(Path::Sep);
	if(-1 != index) {
		return path.Sub(0, index);
	} else {
		return path;
	}
}

String Path::GetFileNameWithoutExt(String fileName)
{
	fileName = fileName.Replace(Path::AltSep, Path::Sep);
	int lastDot = fileName.LastIndexOf('.');
	int lastSep = fileName.LastIndexOf(Path::Sep);
	if(lastDot > lastSep) {
		fileName = fileName.Sub(0, lastDot);
	}
	return fileName;
}

String Path::GetFileNameExt(String fileName)
{
	fileName = fileName.Replace(Path::AltSep, Path::Sep);
	int lastDot = fileName.LastIndexOf('.');
	if (-1 != lastDot) {
		lastDot += 1;
		fileName = fileName.Sub(lastDot, fileName.Length() - lastDot);
	}
	return fileName;
}

String Path::GetFileName(String path)
{
	path = path.Replace(Path::AltSep, Path::Sep);
	int lastSep = path.LastIndexOf(Path::Sep);
	if(-1 != lastSep) {
		return path.Sub(lastSep);
	} else {
		return "";
	}
}

String Path::Combine(String p1, String p2)
{
	p1 = p1.Replace(Path::AltSep, Path::Sep);
	p2 = p2.Replace(Path::AltSep, Path::Sep);
	while(p1.EndsWith(Path::Sep)) {
		p1 = p1.Sub(0, p1.Length() - 1);
	}
	while(p2.StartsWith(Path::Sep)) {
		p2 = p2.Sub(1);
	}
	return p1 + Path::Sep + p2;
}

String Path::GetFiles(String path)
{
	HANDLE dir;
    WIN32_FIND_DATA fileData;

	wchar_t* tmpPath = new wchar_t[path.Length() + 3];
	swprintf(tmpPath, path.Length() + 3, L"%S\\*", path.Data());

	String files = "";
    if ((dir = FindFirstFile((tmpPath), &fileData)) == INVALID_HANDLE_VALUE)
        return files;

    do {
        if (fileData.cFileName[0] == '.')
            continue;

        if ((fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
            continue;

		files = files.Append(fileData.cFileName);
		if (files != "")
			files = files.Append(";");
		
    } while (FindNextFile(dir, &fileData));

	FindClose(dir);
	return files;
}

bool Path::Exists(String path)
{
	HANDLE dir;
    WIN32_FIND_DATA fileData;

	wchar_t* tmpPath = new wchar_t[path.Length() + 1];
	swprintf(tmpPath, path.Length() + 1, L"%S", path.Data());

	bool retVal = false;
    if ((dir = FindFirstFile((tmpPath), &fileData)) == INVALID_HANDLE_VALUE)
        return retVal;

	if ((fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
		retVal = true;

	FindClose(dir);
	return retVal;
}

String Path::GetParentDirectory(String path)
{
	return Path::GetDirectory(path);
}
