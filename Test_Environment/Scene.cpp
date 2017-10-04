#include "Scene.h"



Scene::Scene()
{
}


Scene::~Scene()
{
}

void Scene::Init(Context& context)
{
	(void)context;
}

void Scene::Update(sf::Time dt)
{
	m_PhysicsWorld.Update(dt.asSeconds());
}

void Scene::UpdateEvent(const sf::Event& evnt)
{
	(void)evnt;
}

void Scene::Draw(DrawContext& context)
{
	(void)context;
}

void Scene::CleanUp()
{
}
