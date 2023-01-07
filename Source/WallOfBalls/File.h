#ifndef __FILE_H
#define __FILE_H

#include "String.h"

class File
{
public:
	static bool Exists(String fileName);
	static void Create(String fileName);
	static void Destroy(String fileName);
	static bool Move(String from, String to);
};

#endif