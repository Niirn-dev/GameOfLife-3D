#pragma once
#include "Window.h"
#include "Camera.h"
#include "TestSphere.h"

class App
{
public:
	App();
	App( const App& ) = delete;
	App& operator=( const App& ) = delete;

	int Go();
private:
	void DoFrame();

private:
	Window wnd;
	Camera cam;
	TestSphere sphere;
};