#include "TestEnvironment.h"
#include "Scenes/TestScene.h"
#include "Imgui/imgui.h"
#include "Imgui/imgui_internal.h"
#include "Imgui/imgui-SFML.h"
#include "Scenes/StressTestScene.h"


TestEnvironment::TestEnvironment()
{
}


TestEnvironment::~TestEnvironment()
{
}

void TestEnvironment::Run()
{
	Context context(m_Window);
	Init(context);
	Update(context);
	CleanUp();

	srand(static_cast<unsigned int>(time(nullptr)));
}

void TestEnvironment::Init(Context& context)
{
	m_Window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "2D Physics Lib");
	m_Window.setFramerateLimit(60);

	ImGui::SFML::Init(m_Window);
		
	context.draw.GetCamera().SetScreenSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	m_SceneManager.AddScene(new TestScene());
	m_SceneManager.AddScene(new StressTestScene());

	m_SceneManager.Init(context);
}

void TestEnvironment::Update(Context& context)
{
	sf::Clock clock;

	while (m_Window.isOpen())
	{
		m_Window.clear(sf::Color(63, 63, 63));

		sf::Event evnt;
		while (m_Window.pollEvent(evnt))
		{
			ImGui::SFML::ProcessEvent(evnt);

			if (evnt.type == sf::Event::Closed)
				m_Window.close();
			else if (!context.draw.GetCamera().UpdateEvent(evnt))
				m_SceneManager.UpdateEvent(evnt);
		}
		sf::Time dt = clock.restart();
		ImGui::SFML::Update(m_Window, dt);
		m_SceneManager.OnGui(context);
		m_SceneManager.Update(dt);

		context.draw.GetCamera().Draw(m_Window);
		m_SceneManager.Draw(context.draw);

		ImGui::EndFrame();

		ImGui::SFML::Render(m_Window);
		m_Window.display();
	}

}

void TestEnvironment::CleanUp()
{
	m_SceneManager.Cleanup();

	ImGui::SFML::Shutdown();
}
