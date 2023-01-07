#include "Resources.h"
#include <Windows.h>
#include <stdio.h>

#include "TargaImage.h"

class Resources::Private
{
public:
	struct RawResourceItem
	{
		int tag;
		FileStream f;
	};
	int rawItemCount;
	RawResourceItem rawItems[1024];

	struct ResourceItem
	{
		int tag;
		String type;
		ResourceType* r;
	};
	int resItemCount;
	ResourceItem resItems[1024];
};

Resources::Resources(Path* path)
{
	_Path = path;
}

Resources::~Resources()
{
	delete d;
}

bool Resources::Init(void)
{
	bool retVal = true;
	if (!InitFileSystem(_Path->GetBaseDir())) {
		retVal = false; // SegFault
	}

	if (!VerifyFileSystem(_Path->GetBaseDir())) {
		retVal = false;
	}

	d = new Resources::Private();
	d->rawItemCount = 0;
	d->resItemCount = 0;
	if (!LoadFileSystem(_Path->GetBaseDir())) {
		retVal = false;
	}

	// Initial ProcResources
	ProcResources();
	return retVal;
}

bool Resources::Free(void)
{
	bool retVal = true;
	for (int i = 0; i < d->rawItemCount; i++) {
		d->rawItems[i].tag = 0;
		d->rawItems[i].f.Close();
	}

	for (int i = 0; i < d->resItemCount; i++) {
		d->resItems[i].tag = 0;
		if (d->resItems[i].type == "tga") {
			delete static_cast<TargaImage*>(d->resItems[i].r);
		}
		d->resItems[i].r = 0;
	}

	if (!FreeFileSystem(_Path->GetBaseDir())) {
		retVal = false;
	}
	return retVal;
}

void Resources::ProcResources(void)
{
	for (int i = 0; i < d->rawItemCount; i++) {
		// Load if not Processed
		if (d->rawItems[i].tag != d->resItems[i].tag) {
			d->resItems[i].tag = d->rawItems[i].tag;
			d->resItems[i].type = Path::GetFileNameExt(d->rawItems[i].f.GetFileName()).ToLower();
			d->resItemCount ++;
			if (d->resItems[i].type == "tga") {
				d->resItems[i].r = new TargaImage();
				static_cast<TargaImage*>(d->resItems[i].r)->Load(d->rawItems[i].f);
				static_cast<TargaImage*>(d->resItems[i].r)->ConvertToPixelFormat(static_cast<TargaImage*>(d->resItems[i].r)->GetPixelFormat() == Image::BGR ? Image::RGB : Image::RGBA);
			}
		}
	}
}

ResourceType *Resources::GetResource(int tag)
{
	for (int i = 0; i < d->resItemCount; i++) {
		if (d->resItems[i].tag == tag) {
			return d->resItems[i].r;
		}
	}
	return 0;
}

bool Resources::LoadFileSystem(String basePath)
{
	// Images
	String resSecPath = _Path->Combine(basePath, "Images");
	String semisepFiles = Path::GetFiles(resSecPath);
	LoadResourceSection(resSecPath, semisepFiles);

	// Models
	resSecPath = _Path->Combine(basePath, "Models");
	semisepFiles = Path::GetFiles(resSecPath);
	LoadResourceSection(resSecPath, semisepFiles);

	// Scripts
	resSecPath = _Path->Combine(basePath, "Scripts");
	semisepFiles = Path::GetFiles(resSecPath);
	LoadResourceSection(resSecPath, semisepFiles);

	// Shaders
	resSecPath = _Path->Combine(basePath, "Shaders");
	semisepFiles = Path::GetFiles(resSecPath);
	LoadResourceSection(resSecPath, semisepFiles);

	// Sounds
	resSecPath = _Path->Combine(basePath, "Sounds");
	semisepFiles = Path::GetFiles(resSecPath);
	LoadResourceSection(resSecPath, semisepFiles);
	return true;
}

void Resources::LoadResourceSection(String resSecPath, String semisepSection)
{
	int idx = semisepSection.FirstIndexOf(';');
	if (idx > 0) {
		int start = 0;
		while((idx > 0) && (idx <= semisepSection.Length())) {
			String fn = semisepSection.Sub(start, idx - start);
			d->rawItems[d->rawItemCount].tag = atoi(fn.Sub(0, 4).Data());
			if (d->rawItems[d->rawItemCount].tag > 0) {
				if (d->rawItems[d->rawItemCount].f.Open(_Path->Combine(resSecPath, fn), FileStream::OpenExisting, FileStream::ReadFlag)) {
					d->rawItemCount ++;
				}
				else {
					d->rawItems[d->rawItemCount].tag = 0;
					d->rawItems[d->rawItemCount].f.Close();
				}
			}
			idx = semisepSection.IndexOf((start = (idx + 1)), ';');
		}
	}
}

bool Resources::InitFileSystem(String path)
{
	return Path::Exists(path);
}

bool Resources::FreeFileSystem(String path)
{
	return Path::Exists(path);
}

bool Resources::VerifyFileSystem(String path)
{
	bool retVal = true;
	String tmpPath = _Path->Combine(_Path->GetBaseDir(), "Images");
	if (!Path::Exists(tmpPath)) {
		retVal = false;
	}

	tmpPath = _Path->Combine(_Path->GetBaseDir(), "Models");
	if (!Path::Exists(tmpPath)) {
		retVal = false;
	}

	tmpPath = _Path->Combine(_Path->GetBaseDir(), "Sounds");
	if (!Path::Exists(tmpPath)) {
		retVal = false;
	}

	tmpPath = _Path->Combine(_Path->GetBaseDir(), "Scripts");
	if (!Path::Exists(tmpPath)) {
		retVal = false;
	}

	tmpPath = _Path->Combine(_Path->GetBaseDir(), "Shaders");
	if (!Path::Exists(tmpPath)) {
		retVal = false;
	}

	return retVal;
}