#ifndef __STREAM_H
#define __STREAM_H

#include "Interop.h"

typedef enum SeekDir {
	SeekBegin,
	SeekCurrent,
	SeekEnd
} SeekDir;

class Stream {
public:
	virtual bool Seekable() = 0;
	virtual bool Readable() = 0;
	virtual bool Writeable() = 0;

	virtual i64_t Length() = 0;

	virtual int Read(byte_t *destination, i64_t offset, int count) = 0;
	virtual int Write(byte_t *source, i64_t offset, int count) = 0;

	virtual void SeekGet(i64_t offset, SeekDir dir) = 0;
	virtual i64_t TellGet() = 0;
	virtual bool EofGet() = 0;

	virtual void SeekPut(i64_t offset, SeekDir dir) = 0;
	virtual i64_t TellPut() = 0;
	virtual bool EofPut() = 0;
};

#endif