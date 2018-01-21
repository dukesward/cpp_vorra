#ifndef _BYTE_BUFFER_H
#define _BYTE_BUFFER_H

#ifndef _GLOB_DEFINE_H
	#include "define.h"
#endif

#ifndef _COMMON_LOG_H
	#include "log.h"
#endif

#ifndef _NET_UTILS_
#define _NET_UTILS_

enum EndType
{
	LTL_END,
	BIG_END,
	PDP_END,
	UNK_END
};

inline int EndTest()
{
	int e;
	long i = 0x34333231;
    char* j = (char*)&i;

    if(strcmp(j, "1234") == 0)
    {
    	LOG_PRINT("Detected Big Endian\n");
    	e = BIG_END;
    }
    else
    {
    	LOG_PRINT("Detected Small Endian\n");
    	e = LTL_END;
    }

    return e;
}

#define SYSTEM_END EndTest()

#endif

#endif