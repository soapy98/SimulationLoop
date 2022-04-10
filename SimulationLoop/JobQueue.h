#pragma once
#include <condition_variable>
#include <queue>
class i_job;
class job_queue
{
public:
	job_queue(int i_id);
	~job_queue();
	job_queue(const job_queue&) = delete;
	job_queue(job_queue&&) = delete;

	job_queue& operator=(const job_queue&) = delete;
	job_queue& operator=(job_queue&&) = delete;
	bool add_job(i_job* i_new_job);
	i_job* get_job();

	void request_shutdown()
	{
		std::lock_guard<std::mutex> lock(shutdown_mutex_);
		shutdown_requested_ = true;
		start_searching_.notify_all();
	}
	bool has_shutdown_been_requested() const noexcept { return shutdown_requested_; }
	const int& get_id() const noexcept { return id_; }
	void set_id(const int& i_id) noexcept { id_ = i_id; }

private:
	int                  id_;
	std::condition_variable                     start_searching_;
	std::mutex                                  shutdown_mutex_;
	bool                                        shutdown_requested_;
	std::mutex                                  queue_mutex_;
	std::queue<i_job*>                   job_queue_;
};

