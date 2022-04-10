#include "JobsSys.h"
#include "JobQueue.h"
#include "Worker.h"

// static member initialization
jobs_sys jobs_sys::the_job_system;

jobs_sys::jobs_sys() : shutdown_requested_(false)
{
}

jobs_sys::~jobs_sys()
{
    shutdown();
}

//JobSystem* JobSystem::Create()
//{
//    if (JobSystem::TheJobSystem == nullptr)
//    {
//        JobSystem::TheJobSystem = new JobSystem();
//    }
//    return JobSystem::TheJobSystem;
//}



bool jobs_sys::create_team(const std::string& i_team_name, const size_t num_workers, const int num, int CPU)
{
    // validate inputs
    team* team_ = new team();

    // name this team
    team_->name = i_team_name;
    team_->cpu = CPU;
    // create a job queue for this team
    team_->job_queue = new job_queue(num);

    // create the number of workers requested
    for (uint16_t i = 0; i < num_workers; ++i)
    {
        team_->workers.push_back(new worker(i, team_->job_queue, team_->cpu, team_->name));
    }

    // save the team
    const bool result = teams_.insert(std::pair<std::string, team*>(i_team_name, team_)).second;

    return result;
}

bool jobs_sys::add_job_to_system(i_job* i_job, const std::string& i_team_name)
{
    // validate inputs
    auto team_it = teams_.find(i_team_name);
    if (team_it == teams_.end())
        return false;
    return team_it->second->job_queue->add_job(i_job);
}

void jobs_sys::shutdown()
{
    if (shutdown_requested_)
    {
        return;
    }
    shutdown_requested_ = true;

    for (auto it = teams_.begin(); it != teams_.end(); ++it)
    {
        // ask this team's queue to shutdown
        it->second->job_queue->request_shutdown();

        const size_t num_workers = it->second->workers.size();
        for (size_t i = 0; i < num_workers; ++i)
        {
            // delete each worker in this team
            // each worker joins its thread in its destructor
            delete it->second->workers[i];
        }
        it->second->workers.clear();

        // delete this team's queue
        delete it->second->job_queue;

        // delete this team
        delete it->second;
    }

    teams_.clear();
}