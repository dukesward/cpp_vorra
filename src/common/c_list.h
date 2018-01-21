#ifndef _C_LIST_H
#define _C_LIST_H

 /* C_List ver1.0.5
 /  provided Clone method so that cloning a list is possible now
*/

#ifndef _COMMON_LOG_H
	#include "log.h"
#endif

#ifndef _GLOB_DEFINE_H
	#include "define.h"
#endif

#ifndef _NUM_UTILS_H
	#include "numeric.h"
#endif

class C_List
{
public:
	C_List();
	C_List(const char* k, int id);
	C_List(const char* k, const char* c, int id);
	// must delete the following as copy is not allowed
	C_List(const C_List& list)            = delete;
	C_List& operator=(const C_List& list) = delete;

	~C_List();

	void AddKey(const char* key);
	void Chain(const char* content);
	void Chain(const char* key, const char* content);

	void Clone(C_List* list);
	char* Find(const char* key);

	bool HasKey() { return _key ? true : false; };
	bool HasContent() { return _content ? true : false; };

	char* GetKey() { return _key; };
	char* GetContent() { return _content; };

	void PrintList();

private:
	char* _key;
	// size_t _size;
	char* _content;
	bool _has_c;
	bool _has_next;
	int _id;
	// must use a unique ptr so a new list can be assigned to _next
	// without being destroyed out of the scope it's created
	std::unique_ptr<C_List> _next;
};

#endif