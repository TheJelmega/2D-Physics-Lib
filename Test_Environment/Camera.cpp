#include "Camera.h"



Camera::Camera() : m_WasDown(false)
{
}


Camera::~Camera()
{
}

void Camera::Update(sf::Time dt)
{
	(void)dt;
}

bool Camera::UpdateEvent(const sf::Event& evnt)
{
	if (evnt.type == sf::Event::MouseButtonPressed)
	{
		if (evnt.mouseButton.button == sf::Mouse::Middle)
		{
			m_PrevMousePos = sf::Vector2i(evnt.mouseButton.x, evnt.mouseButton.y);
			m_WasDown = true;
			return true;
		}
	}
	else if (evnt.type == sf::Event::MouseButtonReleased)
	{
		if (evnt.mouseButton.button == sf::Mouse::Button::Middle)
			m_WasDown = false;
	}
	else if (evnt.type == sf::Event::MouseMoved)
	{
		if (m_WasDown)
		{
			sf::Vector2i curMousePos(evnt.mouseMove.x, evnt.mouseMove.y);
			sf::Vector2f delta = sf::Vector2f(curMousePos - m_PrevMousePos);
			m_View.move(-delta);
			m_PrevMousePos = curMousePos;
		}
		
	}

	return false;
}

void Camera::Draw(sf::RenderWindow& window)
{
	window.setView(m_View);
}

void Camera::SetScreenSize(int width, int height)
{
	m_View.setSize(float(width), float(height));
	m_View.setCenter(float(width) / 2, float(height) / 2);
}
