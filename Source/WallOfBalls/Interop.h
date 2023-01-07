#ifndef __INTEROP_H
#define __INTEROP_H

typedef unsigned char byte_t;

#if defined(_WIN32) || defined(_WIN64)
typedef __int64 i64_t;
#else
typedef long long i64_t;
#endif

#endif
