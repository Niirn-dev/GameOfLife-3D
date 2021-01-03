#pragma once
#include "Timer.h"
#include "ImguiManager.h"
#include "Window.h"
#include "Camera.h"
#include "PointLight.h"
#include "Planet.h"

#include <vector>
#include <memory>

class App
{
public:
	App();
	App( const App& ) = delete;
	App& operator=( const App& ) = delete;

	int Go();
private:
	void DoFrame( float dt );

private:
	ImguiManager imguiManager;
	Window wnd;
	Timer timer;
	Camera cam;
	Planet p;
	PointLight light;
};