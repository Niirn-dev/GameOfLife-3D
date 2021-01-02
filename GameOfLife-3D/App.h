#pragma once
#include "ImguiManager.h"
#include "Window.h"
#include "Camera.h"
#include "TestSphere.h"
#include "PointLight.h"

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
	void DoFrame();

private:
	ImguiManager imguiManager;
	Window wnd;
	Camera cam;
	static constexpr size_t nSpheres = 20;
	std::vector<std::unique_ptr<TestSphere>> spherePtrs;
	PointLight light;
};