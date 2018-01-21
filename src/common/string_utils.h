#ifndef _STR_UTILS_H
#define _STR_UTILS_H

#ifndef _COMMON_LOG_H
	#include "log.h"
#endif

#ifndef _DATA_STRUCT_H
	#include "data_struct.h"
#endif

#ifndef _C_LIST_H
	#include "c_list.h"
#endif

#define BYTE_LIMIT 4
#define INIT_SIZE  2

class Group
{
public:
	Group() : _pos(-1), _list(), _size(0), _hooked(false), _temp(nullptr), _has_temp(false)
	{
		// set init size to 2 so to leave space for null terminate
		_size = INIT_SIZE;
	}

	Group(C_List* list) : _pos(-1), _list(list), _size(0), _hooked(false), _temp(nullptr), _has_temp(false)
	{
		// set init size to 2 so to leave space for null terminate
		_size = INIT_SIZE;
	}

	// no copy constructor/operator allowed for classes having list member prop
	Group(Group& g) = delete;
	Group& operator=(const Group& g) = delete;

	void hook()
	{
		_list = std::unique_ptr<C_List>(new C_List());
		_hooked = true;
	}

	void hook(C_List* list)
	{
		_list = std::unique_ptr<C_List>(new C_List());
		_list->Clone(list);
		_hooked = true;
	}

	bool hooked() { return _hooked; }
	// if key is ready, store the key from _temp so it can be freed
	void store(const char* key, const char* content)
	{
		store_k(key);
		store_c(content);
	}
	// this is useful if we want to just use string literals
	void store(char&& key, char&& content)
	{
		const char _k = key;
		store_k(&_k);

		const char _c = content;
		store_c(&_c);
	}

	void store_k(const char* key) {
		// LOG_PRINT("debug store key [%s]\n", key);
		_list->AddKey(key); 
	}

	void store_c(const char* content) {
		// LOG_PRINT("debug store content [%s]\n", content);
		_list->Chain(content); 
	}
	// allows group to store a temp char and used for content later
	// when called with no args, check if a temp is ready and store it as a content
	void store_t() { 
		if(_temp) {
			store_c(_temp);
			dump_t();
		}
	};

	void store_t(const char* temp)
	{
		// this should never happen
		if(_has_temp)
		{
			LOG_PRINT("Error: trying to create a new temp [%s] with an existing one [%s]\n", temp, _temp);
			throw std::logic_error("[Group] an existing temp must be used before creating new temp");
		}

		size_t s = strlen(temp);
		// _temp = (char*)malloc((s+1) * sizeof(char));
		_temp = new char[s+1]();

		for(int i=0; i<s; i++)
			_temp[i] = temp[i];
		// add null terminate manually
		_temp[s] = '\0';
		// turn on to indicate a temp is ready for using
		_has_temp = true;
	}
	// to check if the _temp has been initialized
	bool empty() { return (_pos >= 0) ? false : true; }
	bool has_temp() { return _has_temp; }
	char* temp() { return _temp; }

	void debug()
	{
		_list->PrintList();
	}
	// a temp is auto dumped after using
	void dump_t() {
		if(_has_temp)
		{
			delete[] _temp;
			_temp = nullptr;
			// turn on this to inidicate a new temp can be created
			_has_temp = false;
		}
	}

private:
	// used to record the storing position of _temp
	int _pos;
	size_t _size;
	bool _hooked;
	char* _temp;
	bool _has_temp;
	// a list must always be wrapped in a smart ptr
	std::unique_ptr<C_List> _list;
};

class StringUtils
{
public:
	static char* SafeCopy(const char* src)
	{
		size_t s = strlen(src);
		// make sure the null terminate position is left
		char* o = new char[s + 1]();
		strncpy(o, src, s);
		// add the null terminate to finish
		o[s] = '\0';

		return o;
	}
	// takes a source string and seek out the words based on the hint
	static void Search(const char* src, const char* hint, Group* g)
	{
		size_t s = strlen(hint);
		// used to indicate the begining of matching point
		int* rec = new int(0);
		// used to indicate where the src position is
		int* pos = new int(0);
		// if use pure list must clone the list before hooking to prevent mem leak
		if(!g->hooked())
			g->hook();
		// indicates whether we started recording or not
		bool* rec_on = new bool(false);
		// indicates whether we started matching or not
		bool* match_on = new bool(false);

		DataStruct::Each(
			// go through each char to grab the hint
			SafeCopy(hint),
			// the each fn injects dependency for each char with index, and breaks if return false
			[g, rec, pos, rec_on, match_on, hint, src](char h, int i) -> bool
			{
				// start searching either next ref appears or end of hint is arrived
				if((h=='{' && i>0) || (i==strlen(hint)-1))
				{
					// if we have arrived the end of hint, just catch whatever left in the src
					// the loop will auto end here, so no return needed
					if(i==strlen(hint)-1)
					{
						// the very special case where last symbol in hint is }
						if(h == '}')
						{
							// turn off rec to stop ref recording and update matching pt
							*rec_on = false;
							// the key will be the ref spanning from recorded index to the previous
							g->store_k(StringUtils::SubString(hint, *rec, i-1));
							// catch the content from last ref to the end
							const char* ref = StringUtils::SubString(src, *pos, strlen(src)-1);
							g->store_c(ref);
						}
						else
						{
							// catch the content from last ref to end, but exclude the hint
							const char* match = StringUtils::SubString(hint, *rec, i);
							int ii = StringUtils::IndexOf(src, match, *pos);
							// the ref should end at the start position of the match to exclude the hint
							const char* ref = StringUtils::SubString(src, *pos, ii);
							g->store_c(ref);
						}
					}
					else
					{
						// get the phrase in hint between two refs
						const char* match = StringUtils::SubString(hint, *rec, (h=='{' ? (i-1) : i));
						// get index of the phrase in src after the current position if exits
						int ii = StringUtils::IndexOf(src, match, *pos);
						if(ii>0 && ii>*pos && *rec>0)
						{
							// if phrase matched, store the ref val directly into group
							// the current search position must be ahead of the next hint, otherwise break
							// LOG_PRINT("debug h = %c | src = %s | match = %s | rec = %d | ii = %d | pos = %d\n", h, src, match, *rec, ii, *pos);
							const char* ref = StringUtils::SubString(src, *pos, ii-1);
							// LOG_PRINT("debug ref 1: %s\n", ref);
							g->store_c(ref);
							// update the position so that the match does not go into the content
							*pos = ii + strlen(match);
						}
						else if(ii==0 && *pos==0)
						{
							// if temp does not exist in src, return false, otherwise keep match as temp
							int ti = StringUtils::IndexOf(src, match);
							if(ti < 0)
								return false;
							else
							{
								g->store_t(match);
								// update pos to be the end of match
								*pos = ti + strlen(match);
							}
						}
						else
							return false;
					}
					// record ref name locally for back ref
					if(h == '{')
					{
						*rec_on = true;
						*rec = i + 1;
					}
				}
				else if(h == '{')
				{
					*rec_on = true;
					// because content must be added after the key, this step is necessary
					if(g->has_temp())
					{
						// match phrase from last { to the current }
						const char* match = StringUtils::SubString(hint, *rec, i);
						// find the phrase pos in src, otherwise match the whole left
						int ii = StringUtils::IndexOf(src, match);
						if(ii > 0)
						{
							// get the ref from last phrase to the current phrase
							const char* ref = StringUtils::SubString(src, *pos, ii);
							// update the pos to end of the current phrase
							*pos = ii + strlen(match);
							// use the ref as the content
							// LOG_PRINT("debug ref 2: %s\n", ref);
							g->store_c(ref);
							g->dump_t();
						}
						else
						{
							// if current phrase not found, use the rest of the src
							const char* ref = StringUtils::SubString(src, *pos, strlen(src)-1);
							// update the pos to end of the current phrase
							*pos = ii + strlen(match);
							// use the ref as the content
							// LOG_PRINT("debug ref 2: %s\n", ref);
							g->store_c(ref);
							g->dump_t();
						}
					}
					*rec = i + 1;
				}
				else if(h == '}')
				{
					// turn off rec to stop ref recording and update matching pt
					*rec_on = false;
					// the key will be the ref spanning from recorded index to the previous
					g->store_k(StringUtils::SubString(hint, *rec, i-1));
					// update rec used for the match
					*rec = i + 1;
				}
				// this point indicates the loop should continue
				// dump the temp again to make sure no temp left in the group
				g->dump_t();
				return true;
			}
		);
		// g->debug();
	}
	// an overload accepts hint to be a rval reference
	static void Search(const char* src, char&& hint, Group* g)
	{
		const char _c = hint;
		StringUtils::Search(src, &_c, g);
	}

	static int IndexOf(const char* src, const char* c)
	{
		size_t s = strlen(src);
		// used to indicate the searching position of c
		int pos = 0;
		int index = -1;
		// if p == len, we have got the wanted, stop looping
		for(int i=0; i<s&&pos<strlen(c); i++)
		{
			if(src[i] == c[pos])
			{
				// move position for next char
				// record index for the first matched char
				if(pos++ == 0) index = i;
			}
			else if(pos > 0)
			{
				// if pos has been changed, reset it
				pos = 0;
			}
		}
		// this happends either p is reset or not all chars matched
		if(pos != strlen(c))
		{
			index = -1;
		}
		// a negative index indecates c is not in src
		return index;
	}

	static int IndexOf(const char* src, char&& c)
	{
		const char _c = c;
		return StringUtils::IndexOf(src, &_c);
	}
	// this allows searching to start at a specified position
	static int IndexOf(const char* src, const char* c, int start)
	{
		int i = StringUtils::IndexOf(SubString(src, start, strlen(src)-1), c);
		// must add the start position back if index is not -1
		return (i < 0) ? i : (i + start);
	}

	static int IndexOf(const char* src, char&& c, int start)
	{
		int i = StringUtils::IndexOf(SubString(src, start, strlen(src)-1), std::forward<char>(c));
		return (i < 0) ? i : (i + start);
	}

	static const char* SubString(const char* src, int s, int e)
	{
		if(s < 0)
			s = 0;
		else if(s > strlen(src) - 1)
			throw std::logic_error("[StringUtils::SubString] start position must not exceed length of src");

		if(e > strlen(src) - 1)
			e = strlen(src) - 1;
		else if(e < 0)
			throw std::logic_error("[StringUtils::SubString] end position must not be negative");

		if(e < s)
			e = s;

		char* sub = new char[e-s+2]();

		for(int i=s; i<=e+1; i++)
		{
			sub[i-s] = src[i];
		}
		// add null terminate manually to prevent leak
		sub[e-s+1] = '\0';
		// LOG_PRINT("debug substring size [%d] output [%s]\n", e-s, sub);
		return sub;
	}
};

#endif