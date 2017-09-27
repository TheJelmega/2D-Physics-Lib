#pragma once
#include "SFML/Graphics.hpp"
class DrawContext
{
public:
	DrawContext(sf::RenderWindow& window);
	~DrawContext();

	void Draw(const sf::Drawable& drawable) const;
	void Draw(sf::Vertex* vertices, size_t vertexCount, sf::PrimitiveType type) const;

	void SetDefaultView();
	sf::RenderWindow& GetWindow() const;

private:
	sf::RenderWindow& m_Window;
};

