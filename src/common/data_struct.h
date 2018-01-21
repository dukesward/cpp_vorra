#ifndef _DATA_STRUCT_H
#define _DATA_STRUCT_H

#ifndef _COMMON_LOG_H
	#include "log.h"
#endif

#ifndef _GLOB_DEFINE_H
	#include "define.h"
#endif

class DataStruct
{
public:
	static void Each(char* arr, std::function<bool(char c, int i)> callback)
	{
		size_t s = strlen(arr);

		for(int i=0; i<s; i++)
		{
			if(!callback(arr[i], i))
				break;
		}
	}
};


#endif