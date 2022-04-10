#pragma once
#pragma once
#include <string>
#include <vector>
#include <map>

// forward declarations
class i_job;
class job_queue;
class worker;

class jobs_sys
{
	struct team
	{
		job_queue* job_queue{};
		std::string  name;
		std::vector<worker*> workers;
		int cpu{};
	};
public:
	static jobs_sys the_job_system;
	static void destroy();

	bool create_team(const std::string& i_team_name, const size_t num_workers, const int id,int CPU);
	bool add_job_to_system(i_job* i_job, const std::string& i_team_name);
	void shutdown();

	jobs_sys();
	~jobs_sys();
	jobs_sys(const jobs_sys&) = delete;
	jobs_sys(jobs_sys&&) = delete;
	jobs_sys& operator=(const jobs_sys&) = delete;
	jobs_sys& operator=(jobs_sys&&) = delete;
private:
	std::map<std::string, team*>                             teams_;
	bool                                                                    shutdown_requested_;

};



