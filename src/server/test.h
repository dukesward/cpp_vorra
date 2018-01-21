#ifndef _GLOB_DEFINE_H
	#include "define.h"
#endif

#ifndef _COMMON_LOG_H
	#include "log.h"
#endif

template<typename T>
T&& forward_with_deduction(T&& t)
{
	return static_cast<T&&>(t);
}

// we expect this to be called when lval ref is passed
void test(int& i)
{
	LOG_PRINT("Test called with lvalue reference = %d\n", i);
}

void test(const int& i)
{
	LOG_PRINT("Test called with const lvalue reference = %d\n", i);
}

void test(int&& i)
{
	LOG_PRINT("Test called with rvalue reference = %d\n", i);
}

// we implement a forwarder so that f(arg) should be equivalent to e(arg)
// we define f -> forward and e -> test
// template<typename T>
// void forwarder(T& t)
// {
	// this will fail the case for f(1) as rval cannot be cast to lval arg
	// test(t);
// }

// template<typename T>
// void forwarder(const T& t)
// {
	// this is ok for case f(1) as a temporary will be created from 1 and used for t
	// in brief, the const qualifier simply extends the lifespan of an rvalue
	// however, this forces test to take only const args
	// test(t);
// }

// this forces us to use a rval reference which deduces T& && -> T& and T&& && -> T&&
template<typename T>
void forwarder(T&& t)
{
	// this treats t always as a lval ref, but now it can take either lval or rval args
	// therefore we expect t(int&) to be called in both cases
	test(t);
	// since we don't want t to be always used as a lval ref, we use forward to fetch back its type
	test(std::forward<T>(t));
}

class Wrapper
{
public:
	Wrapper(char* src) : resrc(src)
	{
		// nothing to do here
	};
	// a copy constructor
	Wrapper(Wrapper& w)
	{
		//
	};
	// a move constructor
	Wrapper(Wrapper&& w)
	{

	}

private:
	char* resrc;
};

// call test by passing a lval => expect to call test(int&)
// test(forward_with_deduction(x));

// call test by passing an rval => expect to call test(int&&)
// test(forward_with_deduction(5));

// forwarder(x);
// forwarder(5);