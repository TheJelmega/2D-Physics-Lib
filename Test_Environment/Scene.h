#pragma once
#include "SFML/Graphics.hpp"
#include "Utils/Context.h"
#include "Physics2D.h"

class Scene
{
public:
	Scene(const char* name);
	virtual ~Scene();

	virtual void Init(Context& context);
	virtual void Update(sf::Time dt);
	virtual void UpdateEvent(const sf::Event& evnt);
	virtual void Draw(DrawContext& context);
	virtual void OnGui(Context& context);
	virtual void CleanUp();


	inline const char* GetName() const { return m_Name; }

protected:

	const char* m_Name;

	f32 m_PhysicsTime = 0.f;
	f32 m_PhysicsDrawTime = 0.f;
	bool m_UpdatePhysics = false;
	bool m_StepPhysics = false;

	P2D::World m_PhysicsWorld;
	
};

