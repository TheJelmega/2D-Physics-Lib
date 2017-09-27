#include "TestEnvironment.h"
#include "Scenes/TestScene.h"


TestEnvironment::TestEnvironment()
{
}


TestEnvironment::~TestEnvironment()
{
}

void TestEnvironment::Run()
{
	Init();
	Update();
	CleanUp();
}

void TestEnvironment::Init()
{
	m_SceneManager.AddScene(new TestScene());

	m_SceneManager.Init();

	m_Window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "2D Physics Lib");
	m_Window.setFramerateLimit(60);
	m_Camera.SetScreenSize(SCREEN_WIDTH, SCREEN_HEIGHT);
}

void TestEnvironment::Update()
{
	sf::Clock clock;
	DrawContext drawContext(m_Window);

	while (m_Window.isOpen())
	{
		m_Window.clear(sf::Color::Black);

		sf::Event evnt;
		while (m_Window.pollEvent(evnt))
		{
			if (evnt.type == sf::Event::Closed)
				m_Window.close();
			else if (!m_Camera.UpdateEvent(evnt))
				m_SceneManager.UpdateEvent(evnt);
		}

		sf::Time dt = clock.restart();
		m_SceneManager.Update(dt);

		m_Camera.Draw(m_Window);
		m_SceneManager.Draw(drawContext);

		m_Window.display();
	}

}

void TestEnvironment::CleanUp()
{
	m_SceneManager.Cleanup();
}
