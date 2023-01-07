#ifndef __FILESTREAM_H
#define __FILESTREAM_H

#include "Stream.h"
#include "String.h"

class FileStream : public Stream
{
private:
	class Private;
	Private *d;

public:
	enum {
		Append = 0x01,
		CreateOrOpen = 0x02,
		CreateNew = 0x04,
		OpenExisting = 0x08,
		Truncate = 0x0B,
	};

	enum {
		ReadFlag = 0x01,
		WriteFlag = 0x02
	};

	FileStream();
	~FileStream();

	/* 
	======================
	IStream methods
	======================
	*/
	virtual bool Seekable();
	virtual bool Readable();
	virtual bool Writeable();

	virtual i64_t Length();

	virtual int Read(byte_t *destination, i64_t offset, int count);
	virtual int Write(byte_t *source, i64_t offset, int count);

	virtual void SeekGet(i64_t offset, SeekDir dir);
	virtual i64_t TellGet();
	virtual bool EofGet();

	virtual void SeekPut(i64_t offset, SeekDir dir);
	virtual i64_t TellPut();
	virtual bool EofPut();

	/*
	========================
	FileStream methods
	========================
	*/
	
	bool Open(String fileName, int openMode, int flags);
	void Close();
	void Flush();
	String GetFileName(void);

	bool IsOpen();
};

#endif
