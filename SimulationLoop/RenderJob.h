#pragma once
#include <vector>

#include "GameObject.h"
#include "IJob.h"
class render_job final :
    public i_job
{
private:
	std::vector<game_object*>_gameObjects;
public:
	render_job() = default;
	render_job(std::vector<game_object*>);
	render_job(const render_job&) = delete;
	render_job(render_job&&) = delete;
	render_job& operator=(const render_job&) = delete;
	render_job& operator=(render_job&&) = delete;
	~render_job()override;
	void do_work() override;
};

