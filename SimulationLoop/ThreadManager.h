#pragma once
#include <thread>
#include <map>
#include <atlcomcli.h>
#include <windows.h>
#include <mutex>
#include <functional>
#include <string>
class RenderSystem;
class game_object;
typedef std::map<std::string, std::thread*> ThreadMap;
class thread_manager final
{
private:
	ThreadMap threads;
	unsigned numCores;
public:
	static std::mutex _TM_mutex;
	static thread_manager TheThread;
	const std::thread* get_thread(const std::string& name) {
		return threads[name];
	}
	void add_thread_and_processor(std::string& name, HANDLE method, int num);
	void release_thread(std::string& name) {
		threads[name]->join();
	}
	std::thread::id get_thread_id(std::string& name)
	{
		return get_thread(name)->get_id();
	}
	std::thread::native_handle_type get_thread_native(std::string& name)
	{
		return threads[name]->native_handle();
	}
	void add_to_queue(const std::function<void()>& func);

	thread_manager();
	~thread_manager();


	thread_manager(thread_manager& TM) = default;
	thread_manager(thread_manager&& other) noexcept = default;

	thread_manager& operator=(const thread_manager& other) = default;;

	thread_manager& operator=(thread_manager&& other)noexcept = default;
	void set_processor(int CPU, std::string& str);
	static DWORD_PTR set_processor(int CPU)noexcept;
};

