#include "TestScene.h"
#include <iostream>


TestScene::TestScene()
	: m_pBody(nullptr)
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
	//bodyDef.active = false;
	m_pBody = m_PhysicsWorld.CreateBody(bodyDef);

	P2D::CircleShapeDef circleDef;
	circleDef.radius = 1.f;
	circleDef.relpos = f32v2(2.f, 0.f);
	circleDef.material.density = 10.f;
	P2D::Shape* pCircle = m_PhysicsWorld.CreateShape(circleDef);
	m_pBody->AddShape(pCircle);

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
	m_StaticBody = m_PhysicsWorld.CreateBody(staticBodyDef);

	P2D::CircleShapeDef staticCircleDef;
	staticCircleDef.radius = 1.5f;
	P2D::Shape* staticCircle = m_PhysicsWorld.CreateShape(staticCircleDef);
	m_StaticBody->AddShape(staticCircle);


	P2D::EventListener& listener = m_PhysicsWorld.GetEventListener();

	listener.SetOnCollisionEnterCallback([](P2D::Contact*) { std::cout << "Collision Enter\n"; });
	listener.SetOnCollisionStayCallback([](P2D::Contact*) { std::cout << "Collision Stay\n"; });
	listener.SetOnCollisionLeaveCallback([](P2D::Contact*) { std::cout << "Collision Leave\n"; });
	listener.SetOnContactCreateCallback([](P2D::Contact*) { std::cout << "Contact Created\n"; });
	listener.SetOnContactDestroyCallback([](P2D::Contact*) { std::cout << "Contact Destroyed\n"; });
}

void TestScene::Update(sf::Time dt)
{
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

	default:
		break;
	}
}

void TestScene::Draw(DrawContext& context)
{
	//context.Draw(m_Circle);
	context.Draw(m_StaticBody);
	context.Draw(m_pBody);
	context.SetDefaultView();
	context.Draw(m_FpsCounter);
}

void TestScene::CleanUp()
{
	m_PhysicsWorld.DestroyBody(m_pBody);
}
