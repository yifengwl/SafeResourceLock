#ifndef _TEST_RESOURCE_MANAGER_
#define _TEST_RESOURCE_MANAGER_
#include "SafeResourceLock.h"
#include "Singleton.h"
#include "map"
#include "mutex"

class Resource :public ResourceLock
{
public:
	Resource(int id)
	{
		m_id = id;
	}
	virtual ~Resource() {}

	int m_id = 0;
};


class ResourceManager :public Singleton<ResourceManager>
{
public:
	
	void addResource(std::shared_ptr<Resource> res);
	
	void delResource(int id);

	std::shared_ptr<SafeResourceLock<Resource>> getResourceById(int id);

private:
	std::mutex m_resourceMutex;
	std::map<int, std::shared_ptr<Resource>> m_resources;
};
#define gResourceManager ResourceManager::Instance()



#endif
