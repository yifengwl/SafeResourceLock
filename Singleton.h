#ifndef _SINGLETON_
#define _SINGLETON_


template<class T>
class Singleton
{
public:
	Singleton()
	{

	}
	~Singleton()
	{

	}
	Singleton(const Singleton&) = delete;
	Singleton& operator = (const Singleton&) = delete;

	static T& Instance()
	{
		static T t;
		return t;
	}
};

#endif