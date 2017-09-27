#pragma once
#include "../Scene.h"

class TestScene : public Scene
{
public:
	TestScene();
	~TestScene();

	void Init() override;
	void Update(sf::Time dt) override;
	void UpdateEvent(const sf::Event& evnt) override;
	void Draw(DrawContext& context) override;
	void CleanUp() override;

private:
	sf::Font m_Font;
	sf::Text m_FpsCounter;
	float m_TimePassed = 0.f;
	int m_Frames = 0;

	sf::CircleShape m_Circle;
	
};

