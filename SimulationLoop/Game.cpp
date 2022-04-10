#include "Game.h"

game::game(HDC hdc, HWND hwnd) : m_hdc_(hdc),m_hwnd_(hwnd)
{
	m_rend =new renderer(600, 600, hwnd);
	m_job_system_ =new jobs_sys();
}
//**************************Render and display the scene in OpenGL***********************
void game::render()									// Here's Where We Do All The Drawing
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();									// Reset The Current Modelview Matrix
	glEnable(GL_TEXTURE_2D);
	m_rend->loop();
	SwapBuffers(m_hdc_);				// Swap Buffers (Double Buffering)
}
