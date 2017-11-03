#include "TestScene.h"
#include <iostream>


TestScene::TestScene()
	: Scene("Test Scene")
{
}


TestScene::~TestScene()
{
}

void TestScene::Init(Context& context)
{
	(void)context;

	//m_Circle.setRadius(20);
	//m_Circle.setPosition(640, 360);

	m_PhysicsWorld.SetTimeStep(1.f / 60.f);
	//m_PhysicsWorld.SetGravity(f32v2::Zero);

	P2D::BodyDef bodyDef;
	bodyDef.position.y = 5.f;
	bodyDef.position.x = 1.f;// 5.75f;
	//bodyDef.angle = P2D::Math::ToRadians(45.01f);
	//bodyDef.active = false;
	P2D::Body* pBody0 = m_PhysicsWorld.CreateBody(bodyDef);

	P2D::PolygonShapeDef polyDef;
	f32v2 polyPoints[] = { f32v2(-1.25f, .5f), f32v2(-1.25f, -.5f), f32v2(1.25f, -.5f), f32v2(1.25f, .5f) };
	//f32v2 polyPoints[] = { f32v2(-.5f, .5f), f32v2(-.5f, -.5f), f32v2(2.f, -.5f), f32v2(2.f, .5f) };
	polyDef.points = polyPoints;
	polyDef.numPoints = 4;
	//polyDef.material.restitution = 1.f;
	//polyDef.material.staticFriction = 0.f;
	//polyDef.material.density = 10.f;
	P2D::PolygonShape* pPoly = m_PhysicsWorld.CreateShape(polyDef);
	//pPoly->SetAsBox(2.5f, 2.5f);
	//pPoly->SetAsRegularPolygon(5, 1.5f);
	//pPoly->CheckWinding();
	pBody0->AddShape(pPoly);

	//pBody->ApplyAngularImpulse(-20.f);

	bodyDef.position += f32v2(1.76f, 3.f);
	bodyDef.angle = 0.01f;
	P2D::Body* pBody1 = m_PhysicsWorld.CreateBody(bodyDef);

	P2D::CircleShapeDef circleDef;
	circleDef.radius = .75f;
	circleDef.material.density = 10.f;
	//circleDef.material.restitution = 10.f;
	circleDef.material.staticFriction = 0.2f;
	circleDef.material.dynamicFriction = .2f;
	P2D::Shape* pCircle = m_PhysicsWorld.CreateShape(circleDef);
	pBody1->AddShape(pCircle);

	//circleDef.radius = .25f;
	//circleDef.relpos = f32v2::Zero;
	//circleDef.material.density = .1f;
	//pCircle = m_PhysicsWorld.CreateShape(circleDef);
	//m_pBody->AddShape(pCircle);

	//m_pBody->ApplyImpulse(f32v2(20.f, 0.f));
	//m_pBody->ApplyAngularImpulse(20.f);
	//m_pBody->ApplyImpulse(f32v2(20.f, 0.f), f32v2(0.f, -1.f));

	//context.draw.GetCamera().SetZoom(8.f);

	P2D::BodyDef staticBodyDef;
	staticBodyDef.type = P2D::BodyType::Static;
	//staticBodyDef.position = f32v2(2.f, 0.f);
	staticBodyDef.position.y = -5.f;
	//staticBodyDef.angle = P2D::Math::ToRadians(5.f);
	//staticBodyDef.position.x = -1.f;
	P2D::Body* pStaticBody = m_PhysicsWorld.CreateBody(staticBodyDef);

	/*P2D::CircleShapeDef staticCircleDef;
	staticCircleDef.radius = 1.5f;
	//staticCircleDef.material.restitution = 0.5f;
	staticCircleDef.material.staticFriction = 0.2f;
	staticCircleDef.material.dynamicFriction = 1.f;
	P2D::Shape* staticCircle = m_PhysicsWorld.CreateShape(staticCircleDef);
	pStaticBody->AddShape(staticCircle);*/

	/*P2D::EdgeShapeDef edgeDef;
	edgeDef.v0 = f32v2(-5.f, 0.f);
	edgeDef.v1 = f32v2(5.f, 0.f);
	edgeDef.material.staticFriction = 0.2f;
	edgeDef.material.dynamicFriction = 1.f;
	edgeDef.material.restitution = 1.f;

	edgeDef.v0 = f32v2(-5.f, 0.f);
	edgeDef.v1 = f32v2(5.f, 0.f);

	P2D::EdgeShape* pEdge = m_PhysicsWorld.CreateShape(edgeDef);
	pStaticBody->AddShape(pEdge);*/

	/*P2D::ChainShapeDef chainDef;
	f32v2 chainPoints[] = {f32v2(-5.f, .5f), f32v2(0.f, 0.f), f32v2(5.f, .5f)};
	chainDef.numPoints = 3;
	chainDef.points = chainPoints;
	chainDef.material.restitution = 1.f;
	chainDef.material.staticFriction = 0.2f;
	chainDef.material.dynamicFriction = 1.f;
	P2D::ChainShape* pChain = m_PhysicsWorld.CreateShape(chainDef);
	pStaticBody->AddShape(pChain);*/

	//staticBodyDef.position = f32v2(2.f, 0.f);
	/*staticBodyDef.position= f32v2(4.f, -7.8f);
	pStaticBody = m_PhysicsWorld.CreateBody(staticBodyDef);

	//staticCircleDef.material.restitution = 1.25f;
	staticCircle = m_PhysicsWorld.CreateShape(staticCircleDef);
	m_pStaticBody1->AddShape(staticCircle);*/

	/*pEdge = m_PhysicsWorld.CreateShape(edgeDef);
	pStaticBody->AddShape(pEdge);
	staticBodyDef.angle = P2D::Math::ToRadians(-5.f);
	//staticBodyDef.position.x = -1.f;
	pStaticBody = m_PhysicsWorld.CreateBody(staticBodyDef);*/

	P2D::PolygonShapeDef staticPolyDef;
	//staticPolyDef.material.restitution = 1.f;
	//pPoly = m_PhysicsWorld.CreateShape(staticPolyDef);
	//pPoly->SetAsBox(10.f, 2.f);

	//pStaticBody->AddShape(pPoly);

	P2D::ChainShapeDef chainDef;
	f32v2 chainArr[4] = { f32v2(-7.5f, 5.f), f32v2(-5.f, 0.f), f32v2(5.f, 0.f), f32v2(7.5f, 5.f)};
	chainDef.points = chainArr;
	chainDef.numPoints = 4;

	P2D::ChainShape* pChain = m_PhysicsWorld.CreateShape(chainDef);
	pStaticBody->AddShape(pChain);

	// Constraints
	/*P2D::ConstraintDef constraintDef;
	constraintDef.pBody = pBody;

	constraintDef.constrainRotation = false;
	constraintDef.minAngle = P2D::Math::ToRadians(-30.f);
	constraintDef.maxAngle = P2D::Math::ToRadians(45.f);

	constraintDef.constrainPositionToAxis = true;
	constraintDef.axisPosition = f32v2(5.75f, 0.f);
	constraintDef.axis = f32v2::Up;
	constraintDef.axisTolerance = .5f;
	constraintDef.axisMinValue = -3.6f;

	P2D::Constraint* pConstraint = m_PhysicsWorld.CreateConstraint(constraintDef);*/

	//TODO: Fix issues if enough time is left
	/*P2D::RevoluteJointDef jointDef;
	jointDef.pBody0 = pBody0;
	jointDef.pos0 = f32v2(1.25f, .5f);
	jointDef.pBody1 = pBody1;
	jointDef.pos1 = f32v2(-.5f, -.5f);

	jointDef.limitAngle = true;
	jointDef.minAngle = P2D::Math::ToRadians(-45.f);
	jointDef.maxAngle = P2D::Math::ToRadians(45.f);*/

	/*P2D::DistanceJointDef jointDef;
	jointDef.pBody0 = pBody0;
	jointDef.pos0 = f32v2(1.25f, .5f);
	jointDef.pBody1 = pBody1;
	jointDef.pos1 = f32v2(-.5f, -.5f);

	jointDef.distance = 2.f;*/

	/*P2D::FixedJointDef jointDef;
	jointDef.pBody0 = pBody0;
	jointDef.pos0 = f32v2(1.25f, .5f);
	jointDef.pBody1 = pBody1;
	jointDef.pos1 = f32v2(-.5f, -.5f);*/

	P2D::PrismaticJointDef jointDef;
	jointDef.pBody0 = pBody0;
	jointDef.pos0 = f32v2(1.25f, .5f);
	jointDef.pBody1 = pBody1;
	jointDef.pos1 = f32v2(-.5f, -.5f);

	jointDef.minValue = 2.f;

	m_PhysicsWorld.CreateJoint(jointDef);

	P2D::EventListener& listener = m_PhysicsWorld.GetEventListener();

	listener.SetOnCollisionEnterCallback([](P2D::Contact*)
	{
		std::cout << "Collision Enter\n";
	});
	listener.SetOnCollisionStayCallback([](P2D::Contact*) { std::cout << "Collision Stay\n"; });
	listener.SetOnCollisionLeaveCallback([](P2D::Contact*)
	{
		std::cout << "Collision Leave\n";
	});
	listener.SetOnContactCreateCallback([](P2D::Contact*)
	{
		std::cout << "Contact Created\n";
	});
	listener.SetOnContactDestroyCallback([](P2D::Contact*) { std::cout << "Contact Destroyed\n"; });
}

void TestScene::Update(sf::Time dt)
{
	Scene::Update(dt);
}

void TestScene::UpdateEvent(const sf::Event& evnt)
{
	switch (evnt.type)
	{
	case sf::Event::KeyPressed:
		if (evnt.key.code == sf::Keyboard::Space)
			m_UpdatePhysics = true;

		break;
	default:
		break;
	}
}

void TestScene::Draw(DrawContext& context)
{
	//context.Draw(m_Circle);
	//context.Draw(m_pStaticBody0);
	//context.Draw(m_pStaticBody1);
	//context.Draw(m_pBody);
	context.Draw(m_PhysicsWorld);
	context.SetDefaultView();
}

void TestScene::CleanUp()
{
	//All bodies are removed upon world allocator destruction
	//m_PhysicsWorld.DestroyBody(m_pBody);
}

void TestScene::OnGui(Context& context)
{
	Scene::OnGui(context);
}
