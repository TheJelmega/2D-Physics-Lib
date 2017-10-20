#include "TestScene.h"
#include <iostream>
#include "Physics/Shapes/EdgeShape.h"


TestScene::TestScene()
{
}


TestScene::~TestScene()
{
}

void TestScene::Init(Context& context)
{
	(void)context;

	m_Font.loadFromFile("arial.ttf");
	m_FpsCounter.setFont(m_Font);
	m_FpsCounter.setPosition(0, 0);

	//m_Circle.setRadius(20);
	//m_Circle.setPosition(640, 360);

	m_PhysicsWorld.SetTimeStep(1.f / 60.f);
	//m_PhysicsWorld.SetGravity(f32v2::Zero);

	P2D::BodyDef bodyDef;
	bodyDef.position.y = 5.f;
	bodyDef.position.x = 5.f;
	//bodyDef.active = false;
	P2D::Body* pBody = m_PhysicsWorld.CreateBody(bodyDef);

	P2D::CircleShapeDef circleDef;
	circleDef.radius = 1.f;
	circleDef.material.density = 10.f;
	circleDef.material.restitution = 10.f;
	circleDef.material.staticFriction = 0.2f;
	circleDef.material.dynamicFriction = .2f;
	P2D::Shape* pCircle = m_PhysicsWorld.CreateShape(circleDef);
	pBody->AddShape(pCircle);

	//m_pBody->ApplyAngularImpulse(-100.f);

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
	m_pStaticBody0->AddShape(staticCircle);


	//staticBodyDef.position = f32v2(2.f, 0.f);
	staticBodyDef.position= f32v2(4.f, -7.8f);
	m_pStaticBody1 = m_PhysicsWorld.CreateBody(staticBodyDef);

	//staticCircleDef.material.restitution = 1.25f;
	staticCircle = m_PhysicsWorld.CreateShape(staticCircleDef);
	m_pStaticBody1->AddShape(staticCircle);*/

	P2D::EdgeShapeDef edgeDef;
	edgeDef.v0 = f32v2(-5.f, 0.f);
	edgeDef.v1 = f32v2(5.f, 0.f);
	edgeDef.material.staticFriction = 0.2f;
	edgeDef.material.dynamicFriction = 1.f;
	edgeDef.material.restitution = 1.f;

	//P2D::EdgeShape* pEdge = m_PhysicsWorld.CreateShape(edgeDef);
	//pStaticBody->AddShape(pEdge);

	P2D::ChainShapeDef chainDef;
	f32v2 chainPoints[] = {f32v2(-5.f, .5f), f32v2(0.f, 0.f), f32v2(5.f, .5f)};
	chainDef.numPoints = 3;
	chainDef.points = chainPoints;
	chainDef.material.staticFriction = 0.2f;
	chainDef.material.dynamicFriction = 1.f;
	P2D::ChainShape* pChain = m_PhysicsWorld.CreateShape(chainDef);
	pStaticBody->AddShape(pChain);

	/*staticBodyDef.angle = P2D::Math::ToRadians(-5.f);
	//staticBodyDef.position.x = -1.f;
	pStaticBody = m_PhysicsWorld.CreateBody(staticBodyDef);

	edgeDef.v0 = f32v2(-5.f, 0.f);
	edgeDef.v1 = f32v2(5.f, 0.f);

	pEdge = m_PhysicsWorld.CreateShape(edgeDef);
	pStaticBody->AddShape(pEdge);*/


	P2D::EventListener& listener = m_PhysicsWorld.GetEventListener();

	listener.SetOnCollisionEnterCallback([](P2D::Contact*)
	{
		std::cout << "Collision Enter\n";
	});
	listener.SetOnCollisionStayCallback([](P2D::Contact*) { std::cout << "Collision Stay\n"; });
	listener.SetOnCollisionLeaveCallback([](P2D::Contact*) { std::cout << "Collision Leave\n"; });
	listener.SetOnContactCreateCallback([](P2D::Contact*)
	{
		std::cout << "Contact Created\n";
	});
	listener.SetOnContactDestroyCallback([](P2D::Contact*) { std::cout << "Contact Destroyed\n"; });
}

void TestScene::Update(sf::Time dt)
{
	if (m_UpdatePhysics)
		Scene::Update(dt);

	m_TimePassed += dt.asSeconds();
	++m_Frames;
	if (m_TimePassed >= 1.f)
	{
		m_FpsCounter.setString("Fps:" + std::to_string(m_Frames));
		m_Frames = 0;
		m_TimePassed -= 1.f;
	}
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
	context.Draw(m_FpsCounter);
}

void TestScene::CleanUp()
{
	//All bodies are removed upon world allocator destruction
	//m_PhysicsWorld.DestroyBody(m_pBody);
}
