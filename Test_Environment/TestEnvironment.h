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
	void Init();
	void Update();
	void CleanUp();

	static constexpr int SCREEN_WIDTH = 1280;
	static constexpr int SCREEN_HEIGHT = 720;

	sf::RenderWindow m_Window;
	Camera m_Camera;

	SceneManager m_SceneManager;
};

