#include "c_list.h"

using namespace std;

C_List::C_List() : _content(nullptr), _next(), _key(nullptr), _has_c(false), _has_next(false), _id(0)
{
	//
}

C_List::C_List(const char* k, int id) : _content(nullptr), _next(), _key(nullptr), _has_c(false), _has_next(false), _id(id)
{
	AddKey(k);
}

// in practise this can be used as a conversion constructor
// make sure to add explicit if C_List l = (char*)c is not desired
C_List::C_List(const char* k, const char* c, int id) : _content(nullptr), _next(), _key(nullptr), _has_c(false), _has_next(false), _id(id)
{
	Chain(k, c);
}

C_List::~C_List()
{
	// memory used as pointers need be freed
	delete _key;
	delete _content;
}

// this allows adding a key first without _content
void C_List::AddKey(const char* key)
{
	if(_key)
	{
		// we do not allow adding key to a list already has a key but no content
		if(_content)
		{
			if(!_next)
			{
				// create a new list using the key
				_next = std::unique_ptr<C_List>(new C_List(key, _id+1));
				_has_next = true;
			}
			else
			{
				_next->AddKey(key);
			}
		}
		else
		{
			// this should never happen as content must be always injected after key
			LOG_PRINT("Trying to add key [%s] to list already having key [%s]\n", key, _key);
			throw std::logic_error("[C_List] Can not add key to the list, key already exists");
		}
	}
	else
	{
		size_t s = strlen(key);
		_key = new char[s + 1]();
		strncpy(_key, key, s);
		_key[s] = '\0';
	}
}

void C_List::Chain(const char* content)
{
	// a content can be only chained when key is initialized
	if(_key)
	{
		if(!_content)
		{
			size_t s = strlen(content);
			// use target _content length + 1 as the base length
			_content = new char[s + 1]();
			// strncpy(dest, src, size) copies the src str to dest with size
			strncpy(_content, content, s);
			// add null char manually
			_content[s] = '\0';
			_has_c = true;
		}
		else
		{
			if(_next)
				_next->Chain(content);
			else
			{
				LOG_PRINT("Trying to add content [%s] to list already having content [%s]\n", content, _content);
				throw std::logic_error("[C_List] Can not chain content to a C_List without key");
			}
		}
	}
	else
	{
		LOG_PRINT("Trying to add content [%s] to list already having content [%s]\n", content, _content);
		throw std::logic_error("[C_List] Can not chain content to a C_List without key");
	}
}

void C_List::Chain(const char* key, const char* content)
{
	LOG_PRINT("chaining processing...\n");
	if(_has_c)
	{
		if(!_next)
		{
			_next = std::unique_ptr<C_List>(new C_List(key, content, _id+1));
			_has_next = true;
		}
		_next->Chain(key, content);
	}
	else
	{
		// use same logic to copy key as content
		AddKey(key);
		// this is the only place a content can be chained directly
		Chain(content);
	}
	LOG_PRINT("test initializing next...\n");
}

// this is provided so cloning from a existing list is possible
void C_List::Clone(C_List* list)
{
	_id = list->_id;

	if(_key)
		list->AddKey(_key);

	if(_content)
		list->Chain(_content);
	// deep clone is needed if _next has been initialized
	if(_next)
		_next->Clone(list);
}

char* C_List::Find(const char* key)
{
	if(strcmp(key, _key) == 0 && _content)
	{
		// LOG_PRINT("Found content based on key [%s] = [%s]\n", _key, _content);
		
	}
	else
	{
		if(_next)
			_next->Find(key);
		else
			LOG_PRINT("No content found based on key [%s]\n", _key);
	}
}

void C_List::PrintList()
{
	LOG_PRINT("id=[%d] | key=[%s] | content=[%s]\n", _id, _key, _content);

	if(_next)
		_next->PrintList();
}