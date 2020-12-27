#pragma once
#include "ImguiManager.h"
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
	ImguiManager imguiManager;
	Window wnd;
	Camera cam;
	TestSphere sphere;
};