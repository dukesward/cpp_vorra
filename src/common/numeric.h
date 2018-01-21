#ifndef _NUM_UTILS_H
#define _NUM_UTILS_H

#ifndef _COMMON_LOG_H
	#include "log.h"
#endif

#define BYTE_LIMIT 4

class NumUtils
{
public:
	static void IntToByte(int h, int c)
	{
		int i;
		// the fn can only take up to 4 byte
		int input = h & 0xFF;
		// char output[32];
		int o;
		// BYTE_LIMIT limits the maximum bytes can be processed
		if(c > BYTE_LIMIT)
			c = BYTE_LIMIT;

		if(c > 1)
		{
			NumUtils::IntToByte(h >> 8, c-1);
			// used to separate each byte
			LOG_PRINT(" ");
		}

		for(i=7; i>=0; i--)
		{
			o = (input >> i) & 1;
			LOG_PRINT("%d", o);
			if(i == 4)
				LOG_PRINT(" ");
		}

		if(c == 4) LOG_PRINT("\n");
	}

	static int SimpleHash(const char* c)
	{
		size_t s = strlen(c);
		int o = 0;
		// loop through the string to get ascii for each char
		for(int i=0; i<s; i++)
		{
			char ch = c[i];
			int a = (int)ch;
			// LOG_PRINT("%d ", a);
			o += a*(i+1);
		}
		// LOG_PRINT("output is %d\n", o);

		return o;
	}
};

#endif