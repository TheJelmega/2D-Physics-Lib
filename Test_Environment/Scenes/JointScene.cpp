#include "JointScene.h"



JointScene::JointScene()
	: Scene("Joint scene")
{
}


JointScene::~JointScene()
{
}

void JointScene::Init(Context& context)
{
	(void)context;

	P2D::BodyDef staticBodyDef;
	staticBodyDef.type = P2D::BodyType::Static;
	P2D::Body* pStaticBody = m_PhysicsWorld.CreateBody(staticBodyDef);

	float halfSize = 10.f;
	P2D::EdgeShapeDef edgeDef;

	edgeDef.v0 = f32v2(-halfSize, -halfSize);
	edgeDef.v1 = f32v2(halfSize, -halfSize);
	P2D::EdgeShape* pEdge = m_PhysicsWorld.CreateShape(edgeDef);
	pStaticBody->AddShape(pEdge);

	edgeDef.v0 = f32v2(-halfSize, 0.2f);
	edgeDef.v1 = f32v2(halfSize, 0.2f);
	pEdge = m_PhysicsWorld.CreateShape(edgeDef);
	pStaticBody->AddShape(pEdge);
	edgeDef.v0 = f32v2(halfSize, -0.2f);
	edgeDef.v1 = f32v2(-halfSize, -0.2f);
	pEdge = m_PhysicsWorld.CreateShape(edgeDef);
	pStaticBody->AddShape(pEdge);

	edgeDef.v0 = f32v2(halfSize, halfSize);
	edgeDef.v1 = f32v2(-halfSize, halfSize);
	pEdge = m_PhysicsWorld.CreateShape(edgeDef);
	pStaticBody->AddShape(pEdge);

	edgeDef.v0 = f32v2(-halfSize, halfSize);
	edgeDef.v1 = f32v2(-halfSize, -halfSize);
	pEdge = m_PhysicsWorld.CreateShape(edgeDef);
	pStaticBody->AddShape(pEdge);

	edgeDef.v0 = f32v2(-0.2f, -halfSize);
	edgeDef.v1 = f32v2(-0.2f, halfSize);
	pEdge = m_PhysicsWorld.CreateShape(edgeDef);
	pStaticBody->AddShape(pEdge);
	edgeDef.v0 = f32v2(0.2f, halfSize);
	edgeDef.v1 = f32v2(0.2f, -halfSize);
	pEdge = m_PhysicsWorld.CreateShape(edgeDef);
	pStaticBody->AddShape(pEdge);

	edgeDef.v0 = f32v2(halfSize, -halfSize);
	edgeDef.v1 = f32v2(halfSize, halfSize);
	pEdge = m_PhysicsWorld.CreateShape(edgeDef);
	pStaticBody->AddShape(pEdge);

	// Fixed joint
	P2D::BodyDef bodyDef;
	bodyDef.position = f32v2(-5.f, 3.f);
	P2D::Body* pBody0 = m_PhysicsWorld.CreateBody(bodyDef);

	P2D::PolygonShapeDef polyDef;
	P2D::PolygonShape* pPoly = m_PhysicsWorld.CreateShape(polyDef);
	pPoly->SetAsBox(4.f, 1.5f);
	pBody0->AddShape(pPoly);

	bodyDef.position = f32v2(-2.75f, 5.25f);
	bodyDef.angle = P2D::Math::ToRadians(90.f);
	P2D::Body* pBody1 = m_PhysicsWorld.CreateBody(bodyDef);
	pPoly = m_PhysicsWorld.CreateShape(polyDef);
	pPoly->SetAsBox(4.f, 1.5f);
	pBody1->AddShape(pPoly);

	P2D::FixedJointDef fixedJointDef;
	fixedJointDef.pBody0 = pBody0;
	fixedJointDef.pos0 = f32v2(1.75f, .5f);
	fixedJointDef.pBody1 = pBody1;
	fixedJointDef.pos1 = f32v2(-1.75f, .5f);
	fixedJointDef.angle = P2D::Math::ToRadians(90.f);

	m_PhysicsWorld.CreateJoint(fixedJointDef);

	// Revolute joint
	bodyDef.position = f32v2(5.f, 3.f);
	bodyDef.angle = P2D::Math::ToRadians(0.f);
	pBody0 = m_PhysicsWorld.CreateBody(bodyDef);

	pPoly = m_PhysicsWorld.CreateShape(polyDef);
	pPoly->SetAsBox(4.f, 1.5f);
	pBody0->AddShape(pPoly);

	bodyDef.position = f32v2(7.25f, 5.25f);
	bodyDef.angle = P2D::Math::ToRadians(90.f);
	pBody1 = m_PhysicsWorld.CreateBody(bodyDef);
	pPoly = m_PhysicsWorld.CreateShape(polyDef);
	pPoly->SetAsBox(4.f, 1.5f);
	pBody1->AddShape(pPoly);

	P2D::RevoluteJointDef revoluteJointDef;
	revoluteJointDef.pBody0 = pBody0;
	revoluteJointDef.pos0 = f32v2(1.75f, .5f);
	revoluteJointDef.pBody1 = pBody1;
	revoluteJointDef.pos1 = f32v2(-1.75f, .5f);

	m_PhysicsWorld.CreateJoint(revoluteJointDef);

	// Fixed joint
	bodyDef.position = f32v2(-5.f, -7.f);
	pBody0 = m_PhysicsWorld.CreateBody(bodyDef);

	pPoly = m_PhysicsWorld.CreateShape(polyDef);
	pPoly->SetAsBox(4.f, 1.5f);
	pBody0->AddShape(pPoly);

	bodyDef.position = f32v2(-2.75f, -4.75f);
	pBody1 = m_PhysicsWorld.CreateBody(bodyDef);
	pPoly = m_PhysicsWorld.CreateShape(polyDef);
	pPoly->SetAsBox(4.f, 1.5f);
	pBody1->AddShape(pPoly);

	P2D::DistanceJointDef distanceJointDef;
	distanceJointDef.pBody0 = pBody0;
	distanceJointDef.pos0 = f32v2(1.75f, .5f);
	distanceJointDef.pBody1 = pBody1;
	distanceJointDef.pos1 = f32v2(-1.75f, .5f);
	distanceJointDef.distance = 3.f;

	m_PhysicsWorld.CreateJoint(distanceJointDef);

	// prismatic joint
	bodyDef.position = f32v2(5.f, -7.f);
	bodyDef.angle = P2D::Math::ToRadians(0.f);
	pBody0 = m_PhysicsWorld.CreateBody(bodyDef);

	pPoly = m_PhysicsWorld.CreateShape(polyDef);
	pPoly->SetAsBox(4.f, 1.5f);
	pBody0->AddShape(pPoly);

	bodyDef.position = f32v2(7.25f, -3.f);
	bodyDef.angle = P2D::Math::ToRadians(90.f);
	pBody1 = m_PhysicsWorld.CreateBody(bodyDef);
	pPoly = m_PhysicsWorld.CreateShape(polyDef);
	pPoly->SetAsBox(4.f, 1.5f);
	pBody1->AddShape(pPoly);

	P2D::PrismaticJointDef prismaticJointDef;
	prismaticJointDef.pBody0 = pBody0;
	prismaticJointDef.pos0 = f32v2(1.75f, .5f);
	prismaticJointDef.pBody1 = pBody1;
	prismaticJointDef.pos1 = f32v2(-1.75f, .5f);
	prismaticJointDef.axis = f32v2::AxisY;
	prismaticJointDef.minValue = 5.f;

	m_PhysicsWorld.CreateJoint(prismaticJointDef);
}

void JointScene::Update(sf::Time dt)
{
	Scene::Update(dt);
}

void JointScene::UpdateEvent(const sf::Event& evnt)
{
	(void)evnt;
}

void JointScene::Draw(DrawContext& context)
{
	Scene::Draw(context);
}

void JointScene::OnGui(Context& context)
{
	Scene::OnGui(context);
}

void JointScene::CleanUp()
{
}
