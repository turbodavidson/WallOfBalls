#ifndef __RESOURCES_H
#define __RESOURCES_H

#include "Path.h"
#include "FileStream.h"
#include "ResourceType.h"

class Resources
{
private:
	class Private;
	Private *d;

private:
	Path* _Path;

public:
	Resources(Path* path);
	~Resources();

public:
	bool Init(void);
	bool Free(void);
	void ProcResources(void);
	ResourceType *GetResource(int tag);

private:
	bool InitFileSystem(String path);
	bool FreeFileSystem(String path);
	bool VerifyFileSystem(String path);
	bool LoadFileSystem(String path);
	void LoadResourceSection(String resSecPath, String semisepSection);
};

#endif