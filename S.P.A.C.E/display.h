#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>
#include<SDL2\SDL.h>

class Display
{
public:
	Display(int width, int height, const std::string& title);

	void Clear(float r, float g, float b, float a);
	void setWindowSize(int width, int height);
	void Update();
	void drawSplashScreen();
	bool loadSplashScreen();
	bool IsClosed();
	bool IsUpPressed();
	bool IsRightPressed();
	bool IsLeftPressed();
	bool IsDownPressed();
	bool IsSpacePressed();
	bool IsEPressed();
	bool IsRPressed();
	bool Is1Pressed();
	bool Is2Pressed();
	bool Is3Pressed();
	bool &IsPageUpPressed();
	bool &IsPageDownPressed();
	bool &IsTabPressed();
	bool IsFullScreen();
	bool& isCPressed();
	bool isNewInput();
	bool& IsKeydown();
	virtual ~Display();
	float GetMouseX();
	float GetMouseY();
	bool& GetFirstMouse();
	bool IsPlusPressed();
	bool IsMinusPressed();
	bool &IsMPressed();
	bool isF1Pressed();
	bool isF2Pressed();

protected:
private:
	Display(const Display& other) {}
	void operator=(const Display& other) {}
	float mouseX;
	float mouseY;
	SDL_Window* m_window;
	SDL_Surface* m_windowSurface = NULL;
	SDL_Surface* m_splashScreen = NULL;
	SDL_GLContext m_glContext;
	bool m_isClosed;
	bool m_isFullscreen;
	bool m_upPress;
	bool m_rightPress;
	bool m_leftPress;
	bool m_downPress;
	bool m_spacePress;
	bool firstMouse;
	bool m_ePressed;
	bool m_rPress;
	bool m_keydown;
	bool m_1Pressed;
	bool m_2Pressed;
	bool m_3Pressed;
	bool m_cPressed;
	bool m_newInput;
	bool m_plusPress;
	bool m_minusPress;
	bool m_mPress;
	bool m_f1Press;
	bool m_f2Press;
	bool m_pageDownPress;
	bool m_pageUpPress;
	bool m_tabPress;
};


#endif // DISPLAY_H