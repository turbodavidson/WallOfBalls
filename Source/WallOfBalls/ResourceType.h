#ifndef __RESOURCETYPE_H
#define __RESOURCETYPE_H

#include "Interop.h"

class ResourceType
{
public:
	virtual byte_t *Data(void) = 0;
};

#endif