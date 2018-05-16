#ifndef _SAFE_RESOURCE_LOCK_
#define _SAFE_RESOURCE_LOCK_

#include <mutex>
#include <thread>
#include <chrono>
#include <iostream>
using namespace std::chrono_literals;

class ResourceLock
{
	template<class T> friend class SafeResourceLock;
public:
	ResourceLock()
	{
	}
	virtual ~ResourceLock()
	{

	}
protected:
	std::thread::id m_lockedThreadId;
	unsigned int m_uLockedSum = 0;
};

template<class T> 
class SafeResourceLock
{
public:
	SafeResourceLock(std::shared_ptr<T> res)
	{
		m_resource = res;
		_lock();
	}
	~SafeResourceLock()
	{
		_unlock();
	}
private:
	void _lock()
	{
		if (m_resource.get() == nullptr || m_isLocked)
		{
			return;
		}
		int lockTime = 0;
		while (true)
		{
			std::unique_lock<std::recursive_mutex> l(m_safeResourceLockMutex);
			auto id = std::this_thread::get_id();
			//����Դδ������
			if (m_resource->m_uLockedSum == 0)
			{
				m_resource->m_lockedThreadId = id;
				++m_resource->m_uLockedSum;
				break;
			}
			//����Դ��ͬһ���߳��ٴ�����	
			if (m_resource->m_lockedThreadId == id)
			{
				++m_resource->m_uLockedSum;
				break;
			}
			//����Դ���ڱ������߳��������ȴ���������
			l.unlock();
			std::this_thread::sleep_for(1ms);
			++lockTime;
			if (lockTime % 5000 == 0)
			{
				/*
					������־��ӡ�����Լ�����ʱ
				*/
				std::cout << "lock resource failed threadid: " << id << std::endl;
				m_resource.reset();
				return;
			}
		}
		m_isLocked = true;
	}
	void _unlock()
	{
		if (!m_isLocked) return;
		std::unique_lock<std::recursive_mutex> l(m_safeResourceLockMutex);
		if (m_resource->m_uLockedSum > 0)
		{
			--m_resource->m_uLockedSum;
		}
		m_isLocked = false;

	}
private:
	bool m_isLocked = false;
	std::shared_ptr<T> m_resource;
	static std::recursive_mutex m_safeResourceLockMutex;
};

template<class T> std::recursive_mutex SafeResourceLock<T>::m_safeResourceLockMutex;

#endif