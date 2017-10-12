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

void DrawContext::Draw(P2D::Body* pBody)
{
	for (P2D::Shape* pShape = pBody->GetShapes(); pShape; pShape = pShape->GetNext())
	{
		switch (pShape->GetType())
		{
		case P2D::Shape::Type::Circle:
			Draw(pBody, static_cast<P2D::CircleShape*>(pShape));
			break;
		default:
			break;
		}
	}
}

void DrawContext::SetDefaultView() const
{
	m_Window.setView(m_Window.getDefaultView());
}

sf::RenderWindow& DrawContext::GetWindow() const
{
	return m_Window;
}

void DrawContext::Draw(P2D::Body* pBody, P2D::CircleShape* pShape) const
{
	sf::CircleShape circle;
	f32 radius = pShape->GetRadius() * m_PhysicsRenderScale;
	circle.setRadius(radius);
	f32v2 pos = (pBody->GetPosition() + pShape->GetRelPosition().Rotated(pBody->GetAngle())) * m_PhysicsRenderScale;
	pos.y = -pos.y;
	pos -= radius;
	circle.setPosition(pos.x, pos.y);

	circle.setFillColor(sf::Color(127, 127, 192));
	circle.setOutlineColor(sf::Color(63, 63, 63));
	circle.setOutlineThickness(1.f);

	m_Window.draw(circle);

	//Angle line
	pos += radius;
	f32 angle = pBody->GetAngle();
	sf::VertexArray line(sf::PrimitiveType::Lines, 2);
	line[0].position = sf::Vector2f(pos.x, pos.y);
	line[0].color = sf::Color::Red;
	line[1].position = sf::Vector2f(pos.x + radius * cos(angle), pos.y - radius * sin(angle));
	line[1].color = sf::Color::Red;

	m_Window.draw(line);

}
