#include "FrictionScene.h"



FrictionScene::FrictionScene()
	: Scene("Friction and inertia scene")
{
}


FrictionScene::~FrictionScene()
{
}

void FrictionScene::Init(Context& context)
{
	(void)context;

	P2D::BodyDef staticBodyDef;
	staticBodyDef.type = P2D::BodyType::Static;
	P2D::Body* pStaticBody = m_PhysicsWorld.CreateBody(staticBodyDef);

	P2D::EdgeShapeDef edgeDef;
	edgeDef.material.staticFriction = 1.f;
	edgeDef.material.dynamicFriction = .25f;
	edgeDef.v0 = f32v2(-10.f, 5.5f);
	edgeDef.v1 = f32v2(-4.f, 4.5f);
	P2D::EdgeShape* pEdge = m_PhysicsWorld.CreateShape(edgeDef);
	pStaticBody->AddShape(pEdge);

	edgeDef.v0 = f32v2(-10.f, .5f);
	edgeDef.v1 = f32v2(-4.f, -.5f);
	pEdge = m_PhysicsWorld.CreateShape(edgeDef);
	pStaticBody->AddShape(pEdge);

	edgeDef.v0 = f32v2(-10.f, -4.5f);
	edgeDef.v1 = f32v2(-4.f, -5.5f);
	pEdge = m_PhysicsWorld.CreateShape(edgeDef);
	pStaticBody->AddShape(pEdge);

	edgeDef.v0 = f32v2(4.f, 4.5f);
	edgeDef.v1 = f32v2(10.f, 5.5f);
	pEdge = m_PhysicsWorld.CreateShape(edgeDef);
	pStaticBody->AddShape(pEdge);

	edgeDef.v0 = f32v2(4.f, -.5f);
	edgeDef.v1 = f32v2(10.f, .5f);
	pEdge = m_PhysicsWorld.CreateShape(edgeDef);
	pStaticBody->AddShape(pEdge);

	edgeDef.v0 = f32v2(4.f, -5.5f);
	edgeDef.v1 = f32v2(10.f, -4.5f);
	pEdge = m_PhysicsWorld.CreateShape(edgeDef);
	pStaticBody->AddShape(pEdge);

	P2D::BodyDef bodyDef;
	bodyDef.angle = P2D::Math::ToRadians(-12.5f);
	bodyDef.position = f32v2(-9.5f, 6.5f);
	P2D::Body* pBody = m_PhysicsWorld.CreateBody(bodyDef);
	P2D::PolygonShapeDef polyDef;
	polyDef.material.staticFriction = 1.f;
	polyDef.material.dynamicFriction = 1.f;
	P2D::PolygonShape* pPoly = m_PhysicsWorld.CreateShape(polyDef);
	pPoly->SetAsBox(1.f, 1.f);
	pBody->AddShape(pPoly);

	bodyDef.position = f32v2(-9.5f, 1.5f);
	pBody = m_PhysicsWorld.CreateBody(bodyDef);
	polyDef.material.staticFriction = 0.5f;
	polyDef.material.dynamicFriction = 0.5f;
	pPoly = m_PhysicsWorld.CreateShape(polyDef);
	pPoly->SetAsBox(1.f, 1.f);
	pBody->AddShape(pPoly);

	bodyDef.position = f32v2(-9.5f, -3.5f);
	pBody = m_PhysicsWorld.CreateBody(bodyDef);
	polyDef.material.staticFriction = 0.1f;
	polyDef.material.dynamicFriction = 0.1f;
	pPoly = m_PhysicsWorld.CreateShape(polyDef);
	pPoly->SetAsBox(1.f, 1.f);
	pBody->AddShape(pPoly);

	bodyDef.position = f32v2(9.5f, 6.5f);
	pBody = m_PhysicsWorld.CreateBody(bodyDef);
	P2D::CircleShapeDef circleDef;
	circleDef.radius = .5f;
	circleDef.material.staticFriction = 1.f;
	circleDef.material.dynamicFriction = 1.f;
	circleDef.material.density = 100.f;
	P2D::CircleShape* pCircle = m_PhysicsWorld.CreateShape(circleDef);
	pBody->AddShape(pCircle);

	bodyDef.position = f32v2(9.5f, 1.5f);
	pBody = m_PhysicsWorld.CreateBody(bodyDef);
	circleDef.material.staticFriction = 0.5f;
	circleDef.material.dynamicFriction = 0.5f;
	circleDef.material.density = 10.f;
	pCircle = m_PhysicsWorld.CreateShape(circleDef);
	pBody->AddShape(pCircle);

	bodyDef.position = f32v2(9.5f, -3.5f);
	pBody = m_PhysicsWorld.CreateBody(bodyDef);
	circleDef.material.staticFriction = 0.1f;
	circleDef.material.dynamicFriction = 0.1f;
	circleDef.material.density = 1.f;
	pCircle = m_PhysicsWorld.CreateShape(circleDef);
	pBody->AddShape(pCircle);
}

void FrictionScene::Update(sf::Time dt)
{
	Scene::Update(dt);
}

void FrictionScene::UpdateEvent(const sf::Event& evnt)
{
	(void)evnt;
}

void FrictionScene::Draw(DrawContext& context)
{
	Scene::Draw(context);
}

void FrictionScene::OnGui(Context& context)
{
	Scene::OnGui(context);
}

void FrictionScene::CleanUp()
{
}
