#include "FileStream.h"
#include "File.h"
#include <iostream>
#include <fstream>

using namespace std;

class FileStream::Private {
public:
	fstream s;
	i64_t length;
	int flags;
	String filename;
};

FileStream::FileStream()
{
	d = new FileStream::Private();
	d->flags = 0;
}

FileStream::~FileStream()
{
	if(IsOpen()) {
		Close();
	}
	delete d;
	d = 0;
}

bool FileStream::Seekable()
{
	return true;
}

bool FileStream::Readable()
{
	return (d->flags & FileStream::ReadFlag) > 0;
}

bool FileStream::Writeable()
{
	return (d->flags & FileStream::WriteFlag) > 0;
}

i64_t FileStream::Length()
{
	return d->length;
}

int FileStream::Read(byte_t *destination, i64_t offset, int count)
{
	i64_t tellg = 0L;
	i64_t bytesRemain = 0L;
	if(offset > 0) {
		SeekGet(offset, SeekCurrent);
	}
	tellg = TellGet();
	if(tellg >= 0) {
		bytesRemain = Length() - tellg;
		if(bytesRemain >= count) {
			d->s.read(reinterpret_cast<char *>(destination), count);
			return count;
		} else {
			d->s.read(reinterpret_cast<char *>(destination), bytesRemain);
			return static_cast<int>(bytesRemain);
		}
	} else {
		return 0;
	}
}

int FileStream::Write(byte_t *source, i64_t offset, int count)
{
	if(offset > 0L) {
		SeekPut(offset, SeekCurrent);
	}
	d->s.write(reinterpret_cast<char *>(source), count);
	return count;
}

void FileStream::SeekGet(i64_t offset, SeekDir dir)
{
	if(IsOpen()) {
		ios_base::seekdir sdir;
		if(SeekBegin == dir) {
			sdir = ios::beg;
		} else if(SeekCurrent == dir) {
			sdir = ios::cur;
		} else if(SeekEnd == dir) {
			sdir = ios::end;
		}
		d->s.seekg(offset, sdir);
	}
}

i64_t FileStream::TellGet()
{
	if(IsOpen()) {
		return static_cast<i64_t>(d->s.tellg());
	} else {
		return -1L;
	}
}

bool FileStream::EofGet()
{
	if(IsOpen()) {
		return d->s.eof();
	} else {
		return true;
	}
}

void FileStream::SeekPut(i64_t offset, SeekDir dir)
{
	if(IsOpen()) {
		ios_base::seekdir sdir;
		if(SeekBegin == dir) {
			sdir = ios::beg;
		} else if(SeekCurrent == dir) {
			sdir = ios::cur;
		} else if(SeekEnd == dir) {
			sdir = ios::end;
		}
		d->s.seekp(offset, sdir);
	}
}

i64_t FileStream::TellPut()
{
	if(IsOpen()) {
		return static_cast<i64_t>(d->s.tellp());
	} else {
		return -1L;
	}
}

bool FileStream::EofPut()
{
	return false;
}

bool FileStream::Open(String fileName, int openMode, int flags)
{
	if(IsOpen()){
		Close();
	}
	if(0 != fileName.Data()) {
		fstream::openmode mode = 0;
		switch(openMode) {
		case FileStream::Append:
			mode = fstream::app;
			break;
		case FileStream::CreateOrOpen:
			if(File::Exists(fileName)){
				File::Destroy(fileName);
			}
			File::Create(fileName);
			break;
		case FileStream::CreateNew:
			if(File::Exists(fileName)){
				return false;
			}
			File::Create(fileName);
			break;
		case FileStream::Truncate:
			mode = fstream::trunc;
			break;
		}
		mode |= fstream::binary;
		if((flags & FileStream::ReadFlag) > 0) {
			mode |= fstream::in;
		} else if((flags & FileStream::WriteFlag) > 0) {
			mode |= fstream::out;
		}
		d->filename = fileName;
		d->s.open(fileName.Data(), mode);
		SeekGet(0, SeekEnd);
		d->length = static_cast<i64_t>(TellGet());
		SeekGet(0, SeekBegin);
		if(IsOpen()) {
			d->flags = flags;
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

void FileStream::Close()
{
	if(IsOpen()) {
		d->s.close();
		d->flags = 0;
	}
}

void FileStream::Flush()
{
	if(IsOpen()) {
		d->s.flush();
	}
}

bool FileStream::IsOpen()
{
	return d->s.is_open();
}

String FileStream::GetFileName(void)
{
	return d->filename;
}