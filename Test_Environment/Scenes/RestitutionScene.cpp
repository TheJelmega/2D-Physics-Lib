#include "RestitutionScene.h"



RestitutionScene::RestitutionScene()
	: Scene("restitution")
{
}


RestitutionScene::~RestitutionScene()
{
}

void RestitutionScene::Init(Context& context)
{
	(void)context;

	P2D::BodyDef staticBodyDef;
	staticBodyDef.type = P2D::BodyType::Static;
	P2D::Body* pStaticBody = m_PhysicsWorld.CreateBody(staticBodyDef);

	P2D::EdgeShapeDef edgeDef;
	edgeDef.material.restitution = 1.f;
	edgeDef.v0 = f32v2(-10.f, 0.f);
	edgeDef.v1 = f32v2(10.f,0.f);
	P2D::EdgeShape* pEdge = m_PhysicsWorld.CreateShape(edgeDef);
	pStaticBody->AddShape(pEdge);

	P2D::BodyDef bodyDef;
	bodyDef.position = f32v2(-5.f, 10.f);
	P2D::Body* pBody = m_PhysicsWorld.CreateBody(bodyDef);
	P2D::CircleShapeDef circleDef;
	circleDef.radius = .5f;
	circleDef.material.restitution = 1.f;
	P2D::CircleShape* pCircle = m_PhysicsWorld.CreateShape(circleDef);
	pBody->AddShape(pCircle);

	bodyDef.position = f32v2(0.f, 10.f);
	pBody = m_PhysicsWorld.CreateBody(bodyDef);
	circleDef.material.restitution = .5f;
	pCircle = m_PhysicsWorld.CreateShape(circleDef);
	pBody->AddShape(pCircle);

	bodyDef.position = f32v2(5.f, 10.f);
	pBody = m_PhysicsWorld.CreateBody(bodyDef);
	circleDef.material.restitution = .1f;
	pCircle = m_PhysicsWorld.CreateShape(circleDef);
	pBody->AddShape(pCircle);
}

void RestitutionScene::Update(sf::Time dt)
{
	Scene::Update(dt);
}

void RestitutionScene::UpdateEvent(const sf::Event& evnt)
{
	(void)evnt;
}

void RestitutionScene::Draw(DrawContext& context)
{
	Scene::Draw(context);
}

void RestitutionScene::OnGui(Context& context)
{
	Scene::OnGui(context);
}

void RestitutionScene::CleanUp()
{
}
