#include "JobQueue.h"
#include "IJob.h"

job_queue::job_queue(int i_id) : id_(i_id),
shutdown_requested_(false)
{
}

job_queue::~job_queue()
{
#ifdef BUILD_DEBUG
    size_t num_unfinished_jobs = 0;
#endif
    while (!job_queue_.empty())
    {

        delete job_queue_.front();
        job_queue_.pop();
    }

}

bool job_queue::add_job(i_job* i_new_job)
{
    // validate inputs

    bool result = false;

    if (shutdown_requested_ == false)
    {
        {
            std::lock_guard<std::mutex> lock(queue_mutex_);
            job_queue_.push(i_new_job);
        }
        result = true;
        start_searching_.notify_one();
    }

    return result;
}

i_job* job_queue::get_job()
{
    if (shutdown_requested_)
    {
        return nullptr;
    }

    i_job* job = nullptr;

    if (job_queue_.empty())
    {
        std::unique_lock<std::mutex> u_lock(queue_mutex_);
        start_searching_.wait(u_lock);
        u_lock.unlock();
    }
    if (shutdown_requested_)
    {
        return nullptr;
    }
    std::lock_guard<std::mutex> lock(queue_mutex_);
    if (!job_queue_.empty())
    {
        job = job_queue_.front();
        job_queue_.pop();
    }

    return job;
}
