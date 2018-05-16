#include "ResourceManager.h"
#include <chrono>
#include <thread>
int main()
{
	std::shared_ptr<Resource> res(new Resource(1));
	gResourceManager.addResource(res);

	auto t1 = std::thread([]() {
		std::cout << "thread 1 start get resource" << std::endl;
		auto res = gResourceManager.getResourceById(1);
		//std::this_thread::sleep_for(20s);
		std::cout << "thread 1 release resource" << std::endl;


	});
	auto t2 = std::thread([]() {
		std::cout << "thread 2 start get resource" << std::endl;
		auto res = gResourceManager.getResourceById(1);
		std::cout << "thread 2 got resource" << std::endl;
	});

	t1.join();
	t2.join();
	system("pause");
}