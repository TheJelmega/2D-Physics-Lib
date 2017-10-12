#include "Camera.h"



Camera::Camera() : m_WasDown(false)
                 , m_Zoom(1.f)
                 , m_ZoomStep(.5f)
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
	switch (evnt.type)
	{
	case sf::Event::MouseButtonPressed:
	{
		if (evnt.mouseButton.button == sf::Mouse::Middle)
		{
			m_PrevMousePos = sf::Vector2i(evnt.mouseButton.x, evnt.mouseButton.y);
			m_WasDown = true;
			return true;
		}
	}
	case sf::Event::MouseButtonReleased:
	{
		if (evnt.mouseButton.button == sf::Mouse::Button::Middle)
			m_WasDown = false;
		return false;
	}
	case sf::Event::MouseMoved:
	{
		if (m_WasDown)
		{
			sf::Vector2i curMousePos(evnt.mouseMove.x, evnt.mouseMove.y);
			sf::Vector2f delta = sf::Vector2f(curMousePos - m_PrevMousePos);
			m_View.move(-delta);
			m_PrevMousePos = curMousePos;
		}
		return false;
	}
	case sf::Event::MouseWheelScrolled:
	{
		if (evnt.mouseWheelScroll.delta > 0)
		{
			m_Zoom -= m_ZoomStep;
			if (m_Zoom < 1.f)
				m_Zoom = 1.f;
			m_View.setSize(sf::Vector2f(m_ScreenSize) * m_Zoom);
		}
		else if (evnt.mouseWheelScroll.delta < 0)
		{
			m_Zoom += m_ZoomStep;
			m_View.setSize(sf::Vector2f(m_ScreenSize) * m_Zoom);
		}
		return false;
	}

	default:
		return false;
	}
}

void Camera::Draw(sf::RenderWindow& window)
{
	window.setView(m_View);
}

void Camera::SetScreenSize(int width, int height)
{
	m_ScreenSize = sf::Vector2i(width, height);
	m_View.setSize(sf::Vector2f(m_ScreenSize) * m_Zoom);
	m_View.setCenter(0, 0);
}

void Camera::SetZoom(float zoom)
{
	m_Zoom = zoom < 1.f ? 1.f : zoom;
	m_View.setSize(sf::Vector2f(m_ScreenSize) * m_Zoom);
}
