#include "ResourceManager.h"

void ResourceManager::addResource(std::shared_ptr<Resource> res)
{
	if (res.get())
	{
		std::lock_guard<std::mutex> l(m_resourceMutex);
		m_resources[res->m_id] = res;
	}
}
void ResourceManager::delResource(int id)
{
	std::lock_guard<std::mutex> l(m_resourceMutex);
	m_resources.erase(id);
}

std::shared_ptr<SafeResourceLock<Resource>> ResourceManager::getResourceById(int id)
{
	std::shared_ptr<Resource> resource;

	{
		std::lock_guard<std::mutex> l(m_resourceMutex);
		auto res = m_resources.find(id);
		if (res != m_resources.end())
		{
			resource = res->second;
		}
	}
	std::shared_ptr<SafeResourceLock<Resource>> safeRes;
	if (resource)
	{
		safeRes.reset(new SafeResourceLock<Resource>(resource));
	}
	return safeRes;
}
