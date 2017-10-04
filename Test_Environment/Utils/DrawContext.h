#pragma once
#include "SFML/Graphics.hpp"
#include "Physics2D.h"
#include "../Camera.h"

class DrawContext
{
public:
	DrawContext(sf::RenderWindow& window);
	~DrawContext();

	void Draw(const sf::Drawable& drawable) const;
	void Draw(sf::Vertex* vertices, size_t vertexCount, sf::PrimitiveType type) const;
	void Draw(P2D::Body* pBody);

	void SetDefaultView() const;
	sf::RenderWindow& GetWindow() const;
	Camera& GetCamera() { return m_Camera; }

private:
	void Draw(P2D::Body* pBody, P2D::CircleShape* pShape) const;

	sf::RenderWindow& m_Window;
	static constexpr f32 m_PhysicsRenderScale = 50.f;
	Camera m_Camera;
};

