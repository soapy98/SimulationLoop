#include "Worker.h"
#include "JobQueue.h"
#include "IJob.h"
#include "ThreadManager.h"

worker::worker(uint16_t i_id, job_queue* i_job_queue, int CPU, std::string team) : id_(i_id),
job_queue_(i_job_queue),is_working_(true)
{
	thread_ = std::thread(&worker::do_job, this, job_queue_);
	if (team != "GameTeam")
	{
		const DWORD_PTR a = thread_manager::TheThread.set_processor(CPU);

		DWORD_PTR dw = SetThreadAffinityMask(thread_.native_handle(), a);
	}

}

worker::~worker()
{
	thread_.join();
}

void worker::do_job(job_queue* i_job_queue)
{
	// validate inputs
	bool stop_working = false;
	do
	{
		i_job* job = i_job_queue->get_job();
		if (job)
		{

			job->do_work();
			delete job;

			stop_working = i_job_queue->has_shutdown_been_requested();
		}
		else
		{
			stop_working = i_job_queue->has_shutdown_been_requested();
		}

	} while (stop_working == false);

}
