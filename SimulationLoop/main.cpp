#include <Windows.h>
#include <corecrt_wstdio.h>
#include "ft2build.h"
#include <memory>
#include "Renderer.h"
#include "Game.h"
#include "ThreadManager.h"
const char TITLE[] = "Window Creation";
std::unique_ptr<game>_game;
float zoom = 10;
int width = 1000, height = 1000;
bool on;
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
//Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
//********************OpenGL Initialization********************************
#include <gl\gl.h>                                // Header File For The OpenGL32 Library
#include <gl\glu.h>                               // Header File For The GLu32 Library
#include <iostream>
HDC			hDC = NULL;		// Private GDI Device Context
//*****************************************************************************
//************************ MESSAGE HANDLER **************************

LRESULT CALLBACK WindowProc(HWND hwnd,
	UINT msg,
	WPARAM wparam,
	LPARAM lparam)
{
	// this is the main message handler of the system
	PAINTSTRUCT		ps;	// used in WM_PAINT
	HDC				hdc;	// handle to a device context

	// what is the message 
	switch (msg)
	{
	case WM_CREATE:
	{
		// do initialization stuff here
		return(0);
	}


	case WM_PAINT:
	{
		// validate the window
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		return(0);
	}

	case WM_KEYDOWN:
	{
		// Handle any non-accelerated key commands
		switch (wparam)
		{
		case '1':
			_game->get_renderer()->add_number_of_spheres(hwnd);
			//addBall
			break;
		case 'R':_game->get_renderer()->clear_spheres_();
			//Reset system
			break;
		case 'P':
			on = !on;
			_game->get_renderer()->on = on;
			break;
		case 'U':
			_game->get_renderer()->add_time_scale(-0.1);
			//Decrease time
			break;
		case 'J':
			_game->get_renderer()->add_time_scale(0.1);
			break;
		case 'I':
			_game->get_renderer()->add_friction(0.1);
			break;
		case 'K':
			_game->get_renderer()->add_friction(-0.1);
			break;
		case 'T':
			_game->get_renderer()->add_sphere_diameter(0.1);
			break;
		case 'B':
			_game->get_renderer()->add_sphere_diameter(-0.1);
			break;
		case 'O':
			_game->get_renderer()->add_restitution(-0.1);
			break;
		case 'L':
			_game->get_renderer()->add_restitution(-0.1);
			break;
		case 'W':
			zoom -= 20;
			//Up
			break;
		case 'S':
			// Calculate The Aspect Ratio Of The Window
			glLoadIdentity();
			gluLookAt(0, 0, -10, 0, 0, 0, 0, 1, 0);
			glTranslatef(0.0, 0.0, -1.0);
			//Down
			break;
		case 'A':
			//Left
			break;
		case 'D':
			//Right
			break;
		case VK_UP:

			//Zoom in
			break;
		case VK_DOWN:

			//Zoom Out
			break;
		case VK_ESCAPE:
		case VK_F12:
			PostMessage(hwnd, WM_CLOSE, 0, 0);
			return (0);

		}
		break;
	}

	case WM_DESTROY:
	{
		// kill the application			
		PostQuitMessage(0);

		return(0);
	}

	default:
		break;

	} // end switch

	// process any messages that we didn't take care of 
	return (DefWindowProc(hwnd, msg, wparam, lparam));

} // end WinProc



//**************************Setup OpenGL***********************
void InitializeOpenGL(HWND hwnd, int width, int height)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	HGLRC		hRC = NULL;		// Permanent Rendering Context

	static	PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		24,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (!(hDC = GetDC(hwnd)))							// Did We Get A Device Context?
	{
		MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
	}

	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
	}

	if (!SetPixelFormat(hDC, PixelFormat, &pfd))		// Are We Able To Set The Pixel Format?
	{
		MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
	}

	if (!(hRC = wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
	}

	if (!wglMakeCurrent(hDC, hRC))					// Try To Activate The Rendering Context
	{
		MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
	}

	ShowWindow(hwnd, SW_SHOW);						// Show The Window
	SetForegroundWindow(hwnd);						// Slightly Higher Priority
	SetFocus(hwnd);									// Sets Keyboard Focus To The Window

	if (height == 0)										// Prevent A Divide By Zero By
	{
		height = 1;										// Making Height Equal One
	}

	glViewport(0, 0, width, height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(150.0f, 0.9, 0.1f, 1000.0f);
	glTranslatef(0.0, -25.0, -15);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//gluLookAt(20, 0, -10, 0, 0, 0, 0, 1, 0);

	// Reset The Modelview Matrix
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

}




//************************ WIN MAIN***********************

int WINAPI WinMain(HINSTANCE hinstance,
	HINSTANCE hprevinstance,
	LPSTR lpcmdline,
	int ncmdshow)
{

	WNDCLASS	winclass;	// this will hold the class we create
	HWND		hwnd;		// generic window handle
	MSG			msg;		// generic message

	// first fill in the window class stucture
	winclass.style = CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc = WindowProc;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = hinstance;
	winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName = NULL;
	winclass.lpszClassName = "WindowCreation";

	// register the window class
	if (!RegisterClass(&winclass))
		return(0);
	// create the window
	if (!((hwnd = CreateWindow("WindowCreation", // class
	                           TITLE,	     // title
	                           WS_OVERLAPPEDWINDOW | WS_VISIBLE,
	                           0,
	                           0,
	                           //Set the size of the window to the size of the screen 
	                           1000,
	                           1000,
	                           //GetSystemMetrics(SM_CXSCREEN),
	                           //GetSystemMetrics(SM_CYSCREEN),
	                           NULL,	   // handle to parent 
	                           NULL,	   // handle to menu
	                           hinstance,	// instance
	                           NULL))))	// creation parms
		return(0);
	glLoadIdentity();
	InitializeOpenGL(hwnd, 1000, 1000);
	_game = std::make_unique<game>(hDC, hwnd);
	_game->get_renderer()->console_update();
	// enter main event loop
	bool quit = false;
	while (!quit)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// test if this is a quit
			if (msg.message == WM_QUIT) quit = true;
			// translate any accelerator keys
			TranslateMessage(&msg);
			// send the message to the window proc
			DispatchMessage(&msg);
		} // end if
		else {
			_game->render();
		}
	} // end while
	//delete _game;
	// return to Windows like this
	return(msg.wParam);
} // end WinMain





//************************ END OF WIN MAIN ***********************
