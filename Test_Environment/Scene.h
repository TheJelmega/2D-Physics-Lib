#pragma once
#include "SFML/Graphics.hpp"
#include "Utils/Context.h"
#include "Physics2D.h"

class Scene
{
public:
	Scene();
	virtual ~Scene();

	virtual void Init(Context& context);
	virtual void Update(sf::Time dt);
	virtual void UpdateEvent(const sf::Event& evnt);
	virtual void Draw(DrawContext& context);
	virtual void CleanUp();

protected:

	P2D::World m_PhysicsWorld;
	
};

