#pragma once
#include <SFML/Graphics.hpp>

class Camera
{
public:
	Camera();
	~Camera();

	void Update(sf::Time dt);
	bool UpdateEvent(const sf::Event& evnt);
	void Draw(sf::RenderWindow& window);

	void SetScreenSize(int width, int height);

private:
	sf::View m_View;
	bool m_WasDown;
	sf::Vector2i m_PrevMousePos;
};

