#include "display.h"
#include <iostream>
#include <GL\glew.h>
using namespace std;

Display::Display(int width, int height, const std::string& title)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);


	//Creating a window with SDL and creating the OpenGl context
	m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	m_glContext = SDL_GL_CreateContext(m_window);
	//Creating a surface to pass the splash screen on to.
	m_windowSurface = SDL_GetWindowSurface(m_window);
	SDL_Surface* icon = SDL_LoadBMP("./res/textures/Icon1.bmp");
	SDL_SetWindowIcon(m_window, icon);
	loadSplashScreen();
	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetWindowPosition(m_window,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED);
	

	GLenum status = glewInit();

	if (status != GLEW_OK) {
		std::cerr << "GLEW failed to initialise!" << std::endl;
	}

	//initialise member variables and input flags to false
	m_isClosed = false;
	m_isFullscreen = false;
	m_downPress = false;
	m_upPress = false;
	m_rightPress = false;
	m_leftPress = false;
	firstMouse = true;
	m_ePressed = false;
	m_rPress = false;
	m_keydown = false;
	m_1Pressed = false;
	m_2Pressed = false;
	m_3Pressed = false;
	m_cPressed = false;
	m_newInput = true;
	m_pageDownPress = false;
	m_pageUpPress = false;
	Clear(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glDepthFunc(GL_LESS);
	drawSplashScreen();
	
}

//Display destructor
Display::~Display()
{
	SDL_FreeSurface(m_splashScreen);
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}
//Set the clear colour of the window
void Display::Clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}
//adjust the size of the window and recentre it
void Display::setWindowSize(int width, int height)
{
	SDL_SetWindowSize(m_window, width, height);
	glViewport(0, 0, width, height);
	SDL_SetWindowPosition(m_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}
//use the CPU to load a simple bitmap image for a splash screen
bool Display::loadSplashScreen()
{
	bool loaded = true;
	m_splashScreen = SDL_LoadBMP("./res/textures/splash.bmp");
	if (m_splashScreen == NULL)
	{
		cout << "Splash screen media loading failed" << endl;
		loaded = false;
	}
	return loaded;
}
//Blit the splash screen surface to the window surface.
void Display::drawSplashScreen()
{
	SDL_BlitSurface(m_splashScreen, NULL, m_windowSurface, NULL);
	SDL_UpdateWindowSurface(m_window);
	SDL_Delay(1000);
}

bool Display::IsClosed() 
{
	return m_isClosed;
}

bool Display::IsUpPressed()
{
	return m_upPress;
}

bool Display::IsRightPressed()
{
	return m_rightPress;
}

bool Display::IsLeftPressed()
{
	return m_leftPress;
}

bool Display::IsDownPressed()
{
	return m_downPress;
}
bool Display::IsSpacePressed()
{
	return m_spacePress;
}

float Display::GetMouseX()
{
	return mouseX;
}
float Display::GetMouseY()
{
	return mouseY;
}

 bool& Display::GetFirstMouse()
{
	return firstMouse;
}

 bool Display::IsEPressed()
 {
	 return m_ePressed;
 }

 bool Display::IsRPressed()
 {
	 return m_rPress;
 }

 bool& Display::IsKeydown()
 {
	 return m_keydown;
 }

 bool Display::Is1Pressed() {
	 return m_1Pressed;
 }
 bool Display::Is2Pressed()
 {
	 return m_2Pressed;
 }

 bool Display::Is3Pressed()
 {
	 return m_3Pressed;
 }

 bool& Display::IsPageUpPressed()
 {
	 return m_pageUpPress;
 }

 bool& Display::IsPageDownPressed()
 {
	 return m_pageDownPress;
 }

 bool & Display::IsTabPressed()
 {
	 return m_tabPress;
 }

 bool Display::IsFullScreen()
 {
	 return m_isFullscreen;
 }

 bool& Display::isCPressed()
 {
	 return m_cPressed;
 }

 bool Display::isNewInput()
 {
	 return m_newInput;
 }

 bool Display::IsPlusPressed()
 {
	 return m_plusPress;
 }

 bool Display::IsMinusPressed()
 {
	 return m_minusPress;
 }

 bool &Display::IsMPressed()
 {
	 return m_mPress;
 }

 bool Display::isF1Pressed()
 {
	 return m_f1Press;
 }

 bool Display::isF2Pressed()
 {
	 return m_f2Press;
 }
 /*
	Display update method used for polling for
	SDL keyboard and mouse events 
 */
void Display::Update()
{
	SDL_GL_SwapWindow(m_window);

	SDL_Event e;

	while (SDL_PollEvent(&e)) 
	{

		if (e.type == SDL_KEYDOWN) {
			switch (e.key.keysym.sym) {
			case SDLK_ESCAPE:
				m_isClosed = true;
				break;
			case SDLK_f:
				if (m_isFullscreen == false)
				{
					m_isFullscreen = true;
					SDL_SetWindowFullscreen(m_window, 1);
				}
				else
				{
					m_isFullscreen = false;
					SDL_SetWindowFullscreen(m_window, 0);
				}
				break;
			case SDLK_w:
				m_upPress = true;
				break;
			case SDLK_d:
				m_rightPress = true;
				break;
			case SDLK_s:
				m_downPress = true;
				break;
			case SDLK_a:
				m_leftPress = true;
				break;
			case SDLK_r:
				m_rPress = true;
				break;
			case SDLK_SPACE:
				m_spacePress = true;
				break;
			case SDLK_1:
				if (m_2Pressed || m_3Pressed) {

				}
				else {
					m_1Pressed = true; m_newInput = false;
				}
				break;
			case SDLK_2:
				if (m_1Pressed || m_3Pressed) {

				}
				else {
					m_2Pressed = true; m_newInput = false;
				}
				break;
			case SDLK_3:
				if (m_1Pressed || m_2Pressed) {

				}
				else { m_3Pressed = true; m_newInput = false; 
				}
				break;
			case SDLK_c:
				m_cPressed = true;
				break;

			case SDLK_e:
				m_ePressed = true;
				break;

			case SDLK_EQUALS:
				m_plusPress = true;
				break;
			case SDLK_MINUS:
				m_minusPress = true;
				break;
			case SDLK_m:
				m_mPress = true;			
				break;
			case SDLK_F1: 
				m_f1Press = true;
				break;
			case SDLK_F2:
				m_f2Press = true;
				break;
			case SDLK_PAGEDOWN:
				m_pageDownPress = true;
				break;
			case SDLK_PAGEUP:
				m_pageUpPress = true;
				break;
			case SDLK_TAB:
				m_tabPress = true;
				break;
			}
		}

		if (e.type == SDL_KEYUP) {
			switch (e.key.keysym.sym) {
			case SDLK_w:
				m_upPress = false;
				break;
			case SDLK_d:
				m_rightPress = false;
				break;
			case SDLK_s:
				m_downPress = false;
				break;
			case SDLK_a:
				m_leftPress = false;
				break;
			case SDLK_SPACE:
				m_spacePress = false;
				break;
			case SDLK_r:
				m_rPress = false;
				m_keydown = false;
				break;
			case SDLK_1:
				m_1Pressed = false;
				m_newInput = true;
				break;
			case SDLK_2:
				m_2Pressed = false;
				m_newInput = true;
				break;
			case SDLK_3:
				m_3Pressed = false;
				m_newInput = true;
				break;
			case SDLK_c:
				m_cPressed = false;
				break;

			case SDLK_e:
				m_ePressed = false;
				break;

			case SDLK_EQUALS:
				m_plusPress = false;
				m_keydown = false;
				break;
			case SDLK_MINUS:
				m_minusPress = false;
				m_keydown = false;
				break;
			case SDLK_m:
				m_mPress = false;
				m_keydown = false;
				break;
			case SDLK_F1:
				m_f1Press = false;
				m_keydown = false;
				break;
			case SDLK_F2:
				m_f2Press = false;
				m_keydown = false;
				break;
			case SDLK_PAGEDOWN:
				m_pageDownPress = false;
				m_keydown = false;
				break;
			case SDLK_PAGEUP:
				m_pageUpPress = false;
				m_keydown = false;
				break;
			case SDLK_TAB:
				m_tabPress = false;
				m_keydown = false;
				break;
			}
				
		}

		if (e.type == SDL_QUIT)
		{
			m_isClosed = true;
		}

		if (e.type == SDL_MOUSEMOTION)
		{
			mouseX = e.motion.x;
			mouseY = e.motion.y;

			if (mouseX > 798)
			{
				SDL_WarpMouseInWindow(m_window, 1, mouseY);
				firstMouse = true;
			}
			if (mouseX < 1)
			{
				SDL_WarpMouseInWindow(m_window, 798, mouseY);
				firstMouse = true;
			}
			if (mouseY > 598)
			{
				SDL_WarpMouseInWindow(m_window, mouseX, 1);
				firstMouse = true;
			}
			if (mouseY < 1)
			{
				SDL_WarpMouseInWindow(m_window, mouseX, 598);
				firstMouse = true;
			}
		}

	}
}
