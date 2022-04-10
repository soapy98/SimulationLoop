#include "RenderJob.h"

render_job::render_job(std::vector<game_object*>_gos):_gameObjects(_gos)
{
}

render_job::~render_job()
{
}

void render_job::do_work()
{
	for (auto* game : _gameObjects)
	{
		game->render();
	}
}
