#pragma once
#include "SFML/Graphics.hpp"
#include "Physics2D.h"
#include "../Camera.h"

class DrawContext
{
public:
	DrawContext(sf::RenderWindow& window);
	~DrawContext();

	void Draw(const P2D::World& world);

	void Draw(const sf::Drawable& drawable) const;
	void Draw(sf::Vertex* vertices, size_t vertexCount, sf::PrimitiveType type) const;
	void Draw(const P2D::Body* pBody);

	void SetDefaultView() const;
	sf::RenderWindow& GetWindow() const;
	Camera& GetCamera() { return m_Camera; }

	bool DrawForces() const { return m_DrawForces; }
	void DrawForces(bool draw) { m_DrawForces = draw; }
	bool DrawCollisionPoints() const { return m_DrawCollisionPoints; }
	void DrawCollisionPoints(bool draw) { m_DrawCollisionPoints = draw; }

	static constexpr f32 GetPhysicsRenderScale() { return m_PhysicsRenderScale; }

private:
	void Draw(const P2D::Body* pBody, const P2D::CircleShape* pShape) const;
	void Draw(const P2D::Body* pBody, const P2D::EdgeShape* pShape) const;
	void Draw(const P2D::Body* pBody, const P2D::ChainShape* pShape) const;
	void Draw(const P2D::Body* pBody, const P2D::PolygonShape* pShape) const;

	sf::RenderWindow& m_Window;
	static constexpr f32 m_PhysicsRenderScale = 50.f;
	static constexpr f32 m_ContactSize = 0.05f;
	static constexpr f32 m_NormalLength = 1.f;
	Camera m_Camera;

	const sf::Color m_Color = sf::Color(128, 128, 192);
	const sf::Color m_SleepColor = sf::Color(128, 128, 128);

	//Settings
	bool m_DrawForces = true;
	bool m_DrawCollisionPoints = true;
};

