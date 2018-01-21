#ifndef _COMMON_MSG_H
#define _COMMON_MSG_H

#ifndef _COMMON_LOG_H
	#include "log.h"
#endif

#ifndef _BYTE_BUFFER_H
	#include "byte_buffer.h"
#endif

class MessageBuffer
{
	typedef std::vector<uint8>::size_type size_type;

public:
	MessageBuffer() : _size(0), _wpos(0), _rpos(0), _storage()
    {
        // initialize the storage vector with default size
        _storage.resize(_READ_BUF_SIZE_);
    }

    uint8* GetBasePointer() { return _storage.data(); }

    size_t GetSize() { return _storage.size(); }

    void SetSize(size_t size) { _size = size; }

    uint8* GetReadPointer() { return GetBasePointer() + _rpos; }

    uint8* GetWritePointer() { return GetBasePointer() + _wpos; }

    void ReadCompleted(size_type bytes) { _rpos += bytes; }

    void WriteCompleted(size_type bytes) { _wpos += bytes; }

    size_type GetActiveSize() const { return _wpos - _rpos; }

    size_type GetRemainingSpace() const { return _storage.size() - _wpos; }

    void PrintState()
    {
    	LOG_PRINT("++++ Read Buffer State ++++\n");
    	LOG_PRINT("Active Size: %zu\n", GetActiveSize());
    	LOG_PRINT("Base Pointer: %zu\n", GetBasePointer());
    	LOG_PRINT("Read Pointer: %zu\n", GetReadPointer());
    	LOG_PRINT("Write Pointer: %zu\n", GetWritePointer());
    }

    void PrintStorage()
    {
        std::ostringstream o;
        o << "Printing Request: [size=%zu]\n" << GetSize();

        for(uint32 i=0; i<GetSize(); ++i)
        {
            o << ReadBuffer<uint8>(i);
        }

        LOG_PRINT(o.str().c_str());
    }

    void Resize(size_type bytes)
    {
    	_storage.resize(bytes);
    }

    void Normalize()
    {
    	if(_rpos)
    	{
    		if(_rpos != _wpos)
    		{
    			memmove(GetBasePointer(), GetReadPointer(), GetActiveSize());
    		}
    		_wpos = _rpos;
    		_rpos = 0;
    	}
    }

    void EnsureFreeSpace()
    {
        LOG_PRINT("Release space for reading buffer\n");
    	// to make sure the wpos is freed normalize must be called first
    	Normalize();

    	if(GetRemainingSpace() == 0)
    	{
    		LOG_PRINT("Make free space for async reader: %zu\n", _size*2);
    		Resize(_size * 2);
    	}
    }

    template<typename T>
    void ReadWithSplit(const char* split, std::function<void(std::string&& token)> callback)
    {
        std::ostringstream o;
        // get size of the split as the max
        size_t s = strlen(split);
        // we use this to indicate the position of split
        int p = 0;
        while((_rpos + sizeof(T)) <= _size && p >= 0)
        // for(int i=0; i<50; ++i)
        {
            T t = ReadBuffer<T>();
            // start checking process if passes symbol matching
            if((T)split[p] == t)
            {
                // if we have arrived the end of split, take it as matched
                if(p == s-1)
                {
                    // LOG_PRINT("position = %d\n", _rpos);
                    // refresh the position indicator for next match
                    p = -1;
                    // use the matched token as argument
                    callback(o.str());
                    // refresh the ostream for next token
                    o.clear();
                    // LOG_PRINT("test ostream after flushing = %s", o.str().c_str());
                    ReadWithSplit<T>(split, callback);
                }
                else
                {
                    // keep increading the indicator if the end is not arrived
                    p++;
                }
            }
            else
            {
                // store the symbol into the ostream
                o << t;
                // if already matched symbols, removed them
                if(p > 0)
                    p = 0;
            }
        }
    }

    template<typename T>
    T ReadBuffer()
    {
        // reads buffer with default read position = _rpos
        T t = ReadBuffer<T>(_rpos);
        // update the default read position so to be readed again
        _rpos += sizeof(T);
        return t;
    }

    template<typename T>
    T ReadBuffer(size_t p)
    {
        // in cases when reading a type may cause overflow, do not read
        if(_rpos + sizeof(T) > GetSize())
            LOG_PRINT("Type size [%zu] out of the size of buffer [%zu]\n", sizeof(T), GetSize());
        // read from default position and convert the read into a const pointer
        T t = *((T* const&)(&_storage[p]));        // check if endian is incorrect, and apply a conversion if needed
        // EndConvert(t);
        return t;
    }

    template<typename T>
    void WriteBuffer(const T* t, size_t s)
    {
        if(s)
        {
            memcpy(GetWritePointer(), (void*)t, s);
            // update the wpos to indicate the writing size
            WriteCompleted(s);
        }
    }

private:
	size_type _size;
	size_type _wpos;
    size_type _rpos;
    std::vector<uint8> _storage;
};

#endif