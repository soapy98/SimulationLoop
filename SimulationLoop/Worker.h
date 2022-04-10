#pragma once
#include <cstdint>
#include <string>
#include <thread>
#include <string>
class job_queue;

class worker
{
public:
	worker(uint16_t i_id, job_queue* i_job_queue, int CPU, std::string team);
	~worker();
	worker(const worker&) = delete;
	worker(worker&&) = delete;
	worker& operator=(const worker&) = delete;
	worker& operator=(worker&&) = delete;
	void do_job(job_queue* i_job_queue);
private:
	std::thread                 thread_;
	uint32_t                    id_;
	job_queue* job_queue_;
	bool is_working_;
}; // class Worker


