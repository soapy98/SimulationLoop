#include "ThreadManager.h"
#include "windows.h"
#include "Game.h"
#include <mutex>
thread_manager thread_manager::TheThread;


void thread_manager::add_to_queue(const std::function<void()>& func)
{
	//std::lock_guard<std::mutex>lock(_TM_mutex);
	//executeQueue.push(func);
}
thread_manager::thread_manager()
{
	numCores = std::thread::hardware_concurrency();
}

thread_manager::~thread_manager()
{
	for (auto& t : threads)
		t.second->join();
}

void thread_manager::add_thread_and_processor(std::string& name, HANDLE method , int num) {
	if (threads.find(name) == threads.end())
	{
		//threads[name] = std::thread(&method);
		set_processor(num, name);
	}
}

void thread_manager::set_processor(int CPU, std::string& name)
{
	int pro = 0;
	switch (CPU)
	{
	case 1:
		pro = 0x02;
		break;
	case 2:
		pro = 0x04;
		break;

	case 3:
		pro = 0x08;
		break;
	case 4:
		pro = 0x10;
		break;
	case 5:
		pro = 0x20;
		break;

	case 6:
		pro = 0x40;
		break;
	case 7:
		pro = 0x80;
		break;
	case 8:
		pro = 0x100;
		break;
	default: 
		break;
	}
	const DWORD_PTR processAffinityMask = pro; // which processor
	SetThreadAffinityMask(get_thread_native(name), processAffinityMask);
}

DWORD_PTR thread_manager::set_processor(int CPU)noexcept
{
	int pro = 0;

	/*switch (NetworkManager::TheNetwork->GetHost())
	{
	case true:*/
	switch (CPU)
	{
	case 1:
		pro = 0x02;
		break;
	case 2:
		pro = 0x04;
		break;

	case 3:
		pro = 0x08;
		break;
	case 4:
		pro = 0x10;
		break;
	case 5:
		pro = 0x20;
		break;

	case 6:
		pro = 0x40;
		break;
	case 7:
		pro = 0x80;
		break;
	case 8:
		pro = 0x100;
		break;
		/*	break;
		case false:
			switch (CPU)
			{
			case 5:
				pro = 0x06;
				break;

			case 6:
				pro = 0x07;
				break;

			case 7:
				pro = 0x08;
				break;
			default:
				break;
			}
			break;
		default:
			break;*/
	}
	DWORD_PTR processAffinityMask = pro;
	return processAffinityMask;
 // which processor

	//SetThreadAffinityMask(thr.native_handle(), processAffinityMask);
	//SetProcessAffinityMask(GetCurrentProcess(), processAffinityMask);
	
}

