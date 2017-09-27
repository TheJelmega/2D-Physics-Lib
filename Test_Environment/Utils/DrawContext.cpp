#include "DrawContext.h"



DrawContext::DrawContext(sf::RenderWindow& window) : m_Window(window)
{
}


DrawContext::~DrawContext()
{
}

void DrawContext::Draw(const sf::Drawable& drawable) const
{
	m_Window.draw(drawable);
}

void DrawContext::Draw(sf::Vertex* vertices, size_t vertexCount, sf::PrimitiveType type) const
{
	m_Window.draw(vertices, vertexCount, type);
}

void DrawContext::SetDefaultView()
{
	m_Window.setView(m_Window.getDefaultView());
}

sf::RenderWindow& DrawContext::GetWindow() const
{
	return m_Window;
}
