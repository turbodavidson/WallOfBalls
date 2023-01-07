#ifndef __STRING_H
#define __STRING_H

class String
{
private:
	struct Private;
	Private *d;

private:
	void Allocate(int nchars);
	void CopyInto(const char *value);
	void CopyInto(const wchar_t *value);
	void ReferTo(const String &value);
	void ReleaseReference();

public:
	String();
	String(const char *value);
	String(const String &value);
	String(const wchar_t *value);

	~String();

	const char *Data();
	int Length();

	const String &operator =(const char *right);
	const String &operator =(const String &right);
	const String &operator =(const wchar_t *right);

	String Append(const char right);
	String Append(const char *right);
	String Append(const String &right);
	String Append(const wchar_t *right);
	String operator +(const char right);
	String operator +(const char *right);
	String operator +(const String &right);

	bool Equals(const char *right);
	bool Equals(String &right);
	bool operator ==(const char *right);
	bool operator ==(String &right);
	bool operator !=(const char *right);

	const String &operator +=(const char right);
	const String &operator +=(const char *right);
	const String &operator +=(const String &right);

	int IndexOf(int index, char c);
	int FirstIndexOf(char c);
	int LastIndexOf(char c);
	bool StartsWith(char c);
	bool EndsWith(char c);

	String ToLower(void);

	String Replace(char oldChar, char newChar);
	
	String Sub(int index);
	String Sub(int index, int length);

	unsigned int GetHash();
};


class CRC32
{
private:
	unsigned int table[256];

public:
	CRC32();
	unsigned int Get(char *data, int length);
};

extern CRC32 Crc32;

#endif
