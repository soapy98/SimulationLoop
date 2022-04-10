#pragma once
#include "Renderer.h"
#include "JobsSys.h"
class game
{
public:
	game(HDC hdc, HWND hwnd);
	~game(void){};
	renderer* get_renderer() const noexcept{ return m_rend; }
	void render();
private:
	renderer* m_rend;
	HDC   m_hdc_;
	HWND m_hwnd_;
	jobs_sys* m_job_system_;
};

