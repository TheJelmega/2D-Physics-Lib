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

	void SetZoomStep(float step) { m_ZoomStep = step > 0 ? step : 0.5f; }
	void SetZoom(float zoom);
	float GetZoom() { return m_Zoom; }

private:
	sf::View m_View;
	bool m_WasDown;
	float m_Zoom;
	float m_ZoomStep;
	sf::Vector2i m_PrevMousePos;
	sf::Vector2i m_ScreenSize;
};

