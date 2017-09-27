#include "TestScene.h"



TestScene::TestScene()
{
}


TestScene::~TestScene()
{
}

void TestScene::Init()
{
	m_Font.loadFromFile("arial.ttf");
	m_FpsCounter.setFont(m_Font);
	m_FpsCounter.setPosition(0, 0);

	m_Circle.setRadius(20);
	m_Circle.setPosition(640, 360);
}

void TestScene::Update(sf::Time dt)
{
	m_TimePassed += dt.asSeconds();
	++m_Frames;
	if (m_TimePassed >= 1.f)
	{
		m_FpsCounter.setString("Fps:" + std::to_string(m_Frames));
		m_Frames = 0;
		m_TimePassed -= 1.f;
	}
}

void TestScene::UpdateEvent(const sf::Event& evnt)
{
	(void)evnt;
}

void TestScene::Draw(DrawContext& context)
{
	context.Draw(m_Circle);
	context.SetDefaultView();
	context.Draw(m_FpsCounter);
}

void TestScene::CleanUp()
{
}
