#pragma once
#include "Window.h"

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
};