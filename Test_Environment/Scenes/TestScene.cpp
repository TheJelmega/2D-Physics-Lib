#include "TestScene.h"



TestScene::TestScene()
	: m_pBody(nullptr)
{
}


TestScene::~TestScene()
{
}

void TestScene::Init(Context& context)
{
	m_Font.loadFromFile("arial.ttf");
	m_FpsCounter.setFont(m_Font);
	m_FpsCounter.setPosition(0, 0);

	//m_Circle.setRadius(20);
	//m_Circle.setPosition(640, 360);

	m_PhysicsWorld.SetTimeStep(1.f / 60.f);
	m_PhysicsWorld.SetGravity(f32v2::Zero);

	P2D::BodyDef bodyDef;
	//bodyDef.position.y = 40.f;
	//bodyDef.active = false;
	m_pBody = m_PhysicsWorld.CreateBody(bodyDef);

	P2D::CircleShapeDef circleDef;
	circleDef.radius = 1.f;
	P2D::Shape* pCircle = m_PhysicsWorld.CreateShape(circleDef);

	m_pBody->AddShape(pCircle);

	//m_pBody->ApplyImpulse(f32v2(20.f, 0.f));
	m_pBody->ApplyAngularImpulse(20.f);

	//context.draw.GetCamera().SetZoom(8.f);
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
	(void)evnt;
}

void TestScene::Draw(DrawContext& context)
{
	//context.Draw(m_Circle);
	context.Draw(m_pBody);
	context.SetDefaultView();
	context.Draw(m_FpsCounter);
}

void TestScene::CleanUp()
{
	m_PhysicsWorld.DestroyBody(m_pBody);
}
