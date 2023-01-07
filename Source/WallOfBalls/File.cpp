#if defined(_WIN32) || defined(_WIN64)

#include "File.h"
#include <Windows.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

bool File::Exists(String fileName)
{
	return 0 != PathFileExistsA(fileName.Data());
}

void File::Create(String fileName)
{
	HANDLE handle = CreateFileA(
		fileName.Data(), 
		0, 
		FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, 
		0, 
		CREATE_ALWAYS, 
		NULL, 
		NULL);
	CloseHandle(handle);
}

void File::Destroy(String fileName)
{
	DeleteFileA(fileName.Data());
}

bool File::Move(String from, String to)
{
	return 0 != MoveFileA(from.Data(), to.Data());
}

#endif
