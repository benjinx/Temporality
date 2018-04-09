#ifndef WINDOW_H
#define WINDOW_H

#include "Common.hpp"

struct GLFWwindow;

class Window
{
public:
    Window(){};
    ~Window(){};

    void Start();
    void Destroy();

	void Clear();
	void Present();

    GLFWwindow* GetWindow() { return _mpWindow; }

    int GetWidth() { return _mWidth; }
    int GetHeight() { return _mHeight; }

private:
    GLFWwindow* _mpWindow;
    int         _mWidth, _mHeight;
};

#endif // WINDOW_H