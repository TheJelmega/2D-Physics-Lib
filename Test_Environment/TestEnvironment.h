#pragma once
#include "SceneManager.h"
#include "Camera.h"

class TestEnvironment
{
public:
	TestEnvironment();
	~TestEnvironment();

	void Run();

private:
	void Init(Context& context);
	void Update(Context& context);
	void CleanUp();

	static constexpr int SCREEN_WIDTH = 1280;
	static constexpr int SCREEN_HEIGHT = 720;

	sf::RenderWindow m_Window;

	SceneManager m_SceneManager;
};

