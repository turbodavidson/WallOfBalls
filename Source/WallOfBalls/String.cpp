#include "String.h"
#include <cstdlib>
#include <cstring>

struct String::Private {
	int references;
	int bufferSize;
	int length;
	unsigned int hash;
	union {
		char *data;
		struct {
			int r0;
			int r1;
		};
	};
};

String::String()
{
	d = 0;
}

String::String(const wchar_t *value)
{
	CopyInto(value);
}

String::String(const char *value)
{
	CopyInto(value);
}

String::String(const String &value)
{
	ReferTo(value);
}

String::~String()
{
	ReleaseReference();
}

const char *String::Data()
{
	if(0 == d) {
		return 0;
	} else {
		return d->data;
	}
}

int String::Length()
{
	if(0 == d) {
		return 0;
	} else {
		return d->length;
	}
}

void String::Allocate(int nchars)
{
	int i, modlen;
	if(nchars > 0) {
		d = (String::Private *)malloc(sizeof(String::Private));
		d->references = 1;
		d->hash = 0;
		d->r1 = 0;
		d->length = nchars;
		i = d->length + 1;
		modlen = i % 4;
		d->bufferSize = i + (4 - modlen);
		d->data = (char *)calloc(d->bufferSize, 1);
	} 
}

void String::CopyInto(const char *value)
{
	int i, modlen;
	if(0 != value) {
		d = (String::Private *)malloc(sizeof(String::Private));
		d->references = 1;
		d->hash = 0;
		d->r1 = 0;
		d->length = strlen(value);
		i = d->length + 1;
		modlen = i % 4;
		d->bufferSize = i + (4 - modlen);
		d->data = (char *)malloc(d->bufferSize);
		memset(d->data + i, 0, (size_t)(d->bufferSize - i));
		memcpy(d->data, value, i);
	} else {
		d = 0;
	}
}

void String::CopyInto(const wchar_t *value)
{
	int i, modlen;
	if(0 != value) {
		d = (String::Private *)malloc(sizeof(String::Private));
		d->references = 1;
		d->hash = 0;
		d->r1 = 0;
		d->length = wcslen(value);
		i = d->length + 1;
		modlen = i % 4;
		d->bufferSize = i + (4 - modlen);
		d->data = (char *)malloc(d->bufferSize);
		memset(d->data + i, 0, (size_t)(d->bufferSize - i));
		wcstombs(d->data, value, i);
	} else {
		d = 0;
	}
}

void String::ReferTo(const String &value)
{
	if(0 != value.d) {
		d = value.d;
		d->references++;
	}
}

void String::ReleaseReference()
{
	if(0 != d) {
		d->references--;
		if(0 == d->references) {
			free(d->data);
			free(d);
			d = 0;
		}
	}
}

unsigned int String::GetHash()
{
	if(0 == d) {
		return 0;
	} else {
		if(0 == d->hash) {
			d->hash = Crc32.Get(d->data, d->bufferSize);
		}
		return d->hash;
	}
}

String String::Append(const char right)
{
	char buffer[2];
	buffer[0] = right;
	buffer[1] = '\0';
	return Append(buffer);
}

String String::Append(const char *right)
{
	String m;
	if((0 == d) && (0 != right)) {
		m = right;
	} else if((0 != d) && (0 == right)) {
		m = (*this);
	} else if((0 != d) && (0 != right)) {
		m.Allocate(d->length + strlen(right));
		memcpy(m.d->data, d->data, d->length);
		memcpy(m.d->data + d->length, right, m.d->length - d->length);
	}
	return m;
}

String String::Append(const wchar_t *right)
{
	String m;
	if((0 == d) && (0 != right)) {
		m = right;
	} else if((0 != d) && (0 == right)) {
		m = (*this);
	} else if((0 != d) && (0 != right)) {
		m.Allocate(d->length + wcslen(right));
		memcpy(m.d->data, d->data, d->length);
		wcstombs(m.d->data + d->length, right, m.d->length - d->length);
	}
	return m;
}

String String::Append(const String &right)
{
	if(0 != right.d) {
		return Append(right.d->data);
	} else {
		return Append((const char *)0);
	}
}

String String::operator +(const char right)
{
	return Append(right);
}

String String::operator +(const char *right)
{
	return Append(right);
}

String String::operator +(const String &right)
{
	return Append(right);
}

const String &String::operator =(const char *right)
{
	ReleaseReference();
	CopyInto(right);
	return (*this);
}

const String &String::operator =(const String &right)
{
	ReleaseReference();
	ReferTo(right);
	return (*this);
}

const String &String::operator =(const wchar_t *right)
{
	ReleaseReference();
	CopyInto(right);
	return (*this);
}

const String &String::operator +=(const char right)
{
	return (*this) = this->Append(right);
}

const String &String::operator +=(const char *right)
{
	return (*this) = this->Append(right);
}

const String &String::operator +=(const String &right)
{
	return (*this) = this->Append(right);
}

bool String::Equals(const char *right)
{
	String t(right);
	return Equals(t);
}

bool String::Equals(String &right)
{
	if(d == right.d) {
		return true;
	} else {
		if((0 == d) || (0 == right.d)) {
			return false;
		} else {
			if(GetHash() == right.GetHash()) {
				return 0 == memcmp(d->data, right.d->data, d->bufferSize);
			} else {
				return false;
			}
		}
	}
}

bool String::operator ==(const char *right)
{
	return Equals(right);
}

bool String::operator !=(const char *right)
{
	return !Equals(right);
}

bool String::operator ==(String &right)
{
	return (*this).Equals(right);
}

String String::ToLower()
{
	String m;
	char *lpd;
	if(0 != d) {
		m = d->data;
		for(lpd = m.d->data; lpd != m.d->data + m.d->length; lpd++) {
			if(64 < *lpd && 91 > *lpd) {
				*lpd += 32;
			}
		}
	}
	return m;
}

String String::Replace(char oldChar, char newChar)
{
	String m;
	char *lpd;
	if(0 != d) {
		m = d->data;
		for(lpd = m.d->data; lpd != m.d->data + m.d->length; lpd++) {
			if(oldChar == *lpd) {
				*lpd = newChar;
			}
		}
	}
	return m;
}

int String::IndexOf(int index, char c)
{
	if(0 != d) {
		if(index < d->length) {
			for(int i=index; i<d->length; i++) {
				if(c == d->data[i]) {
					return i;
				}
			}
			return -1;
		}else {
			return -1;
		}
	} else {
		return -1;
	}
}

int String::FirstIndexOf(char c)
{
	if(0 != d) {
		for(int i=0; i<d->length; i++) {
			if(c == d->data[i]) {
				return i;
			}
		}
		return -1;
	} else {
		return -1;
	}
}

int String::LastIndexOf(char c)
{
	if(0 != d) {
		for(int i=d->length - 1; i>=0; i--) {
			if(c == d->data[i]) {
				return i;
			}
		}
		return -1;
	} else {
		return -1;
	}
}

bool String::StartsWith(char c)
{
	if(0 != d) {
		return (c == d->data[0]);
	} else {
		return false;
	}
}

bool String::EndsWith(char c)
{
	if(0 != d) {
		return (c == d->data[d->length - 1]);
	} else {
		return false;
	}
}

String String::Sub(int index)
{
	return Sub(index, INT_MAX);
}

String String::Sub(int index, int length)
{
	if(0 != d) {
		int charsRemain = d->length - index;
		if(length > charsRemain) {
			length = charsRemain;
		}
		String m;
		m.Allocate(length);
		memcpy(m.d->data, d->data+index, length);
		return m;
	} else {
		return "";
	}
}

#define POLYNOMIAL (0x04C11DB7)
CRC32 Crc32;
CRC32::CRC32()
{
	const int topBit = (1 << 31);

	unsigned int remainder = 0;
	for(unsigned int dividend = 0; dividend < 256; dividend++) {
		remainder = dividend << (24);
		for(unsigned char bit = 8; bit > 0; bit--) {
			if(remainder & topBit) {
				remainder = (remainder << 1) ^ POLYNOMIAL;
			} else {
				remainder = (remainder << 1);
			}
		}
		table[dividend] = remainder;
	}
}

unsigned int CRC32::Get(char *data, int length)
{
	unsigned char *lp8;
	unsigned short *lp16;
	unsigned int *lp32;
	unsigned int remainder;
	unsigned char data8;
	remainder = 0;
	if(0 == length % 4) {
		for(lp32 = (unsigned int *)data; lp32 != (unsigned int *)(data + length); lp32++) {
			data8 = ((*lp32 >> 0) & 0xFF) ^ (remainder >> (32 - 8));
			remainder = table[data8] ^ (remainder << 8);
			data8 = ((*lp32 >> 8) & 0xFF) ^ (remainder >> (32 - 8));
			remainder = table[data8] ^ (remainder << 8);
			data8 = ((*lp32 >> 16) & 0xFF) ^ (remainder >> (32 - 8));
			remainder = table[data8] ^ (remainder << 8);
			data8 = ((*lp32 >> 24) & 0xFF) ^ (remainder >> (32 - 8));
			remainder = table[data8] ^ (remainder << 8);
		}
	} else if(0 == length % 2) {
		for(lp16 = (unsigned short *)data; lp16 != (unsigned short *)(data + length); lp16++) {
			data8 = ((*lp16 >> 0) & 0xFF) ^ (remainder >> (32 - 8));
			remainder = table[data8] ^ (remainder << 8);
			data8 = ((*lp16 >> 8) & 0xFF) ^ (remainder >> (32 - 8));
			remainder = table[data8] ^ (remainder << 8);
		}
	} else {
		
		for(lp8 = (unsigned char *)data; lp8 != (unsigned char *)(data + length); lp8++) {
			data8 = *lp8 ^ (remainder >> (32 - 8));
			remainder = table[data8] ^ (remainder << 8);
		}
	}
	return remainder;
}
