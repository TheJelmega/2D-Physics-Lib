#include "StressTestScene.h"
#include "../Imgui/imgui-SFML.h"
#include "../Imgui/imgui.h"


StressTestScene::StressTestScene()
	: Scene("Stress Test")
{
}

StressTestScene::~StressTestScene()
{
}

void StressTestScene::Init(Context& context)
{
	(void)context;

	P2D::BodyDef bodyDef;
	bodyDef.type = P2D::BodyType::Static;

	P2D::Body* pBody = m_PhysicsWorld.CreateBody(bodyDef);

	/*P2D::ChainShapeDef chainDef;
	f32v2 points[] = { f32v2(-25.f, 10.f), f32v2(-20.f, -10.f), f32v2(20.f, -10.f), f32v2(25.f, 10.f) };
	chainDef.points = points;
	chainDef.numPoints = 4;

	P2D::ChainShape* pShape = m_PhysicsWorld.CreateShape(chainDef);
	pBody->AddShape(pShape);*/

	P2D::EdgeShapeDef edgeDef;
	edgeDef.v0 = f32v2(-25.f, 10.f);
	edgeDef.v1 = f32v2(-20.f, -10.f);
	P2D::EdgeShape* pShape = m_PhysicsWorld.CreateShape(edgeDef);
	pBody->AddShape(pShape);

	edgeDef.v0 = f32v2(-20.f, -10.f);;
	edgeDef.v1 = f32v2(20.f, -10.f);
	pShape = m_PhysicsWorld.CreateShape(edgeDef);
	pBody->AddShape(pShape);

	edgeDef.v0 = f32v2(20.f, -10.f);
	edgeDef.v1 = f32v2(25.f, 10.f);
	pShape = m_PhysicsWorld.CreateShape(edgeDef);
	pBody->AddShape(pShape);
}

void StressTestScene::Update(sf::Time dt)
{
	Scene::Update(dt);
}

void StressTestScene::UpdateEvent(const sf::Event& evnt)
{
	(void)evnt;
}

void StressTestScene::Draw(DrawContext& context)
{
	Scene::Draw(context);
}

void StressTestScene::OnGui(Context& context)
{
	Scene::OnGui(context);

	ImGui::DragInt("Amount of bodies to spawn", &m_AmountToSpawn, 0.1f, 1, 5);
	constexpr float spacing = 1.2f;

	if (ImGui::Button("Spawn Sphere"))
	{
		int halfAmount = m_AmountToSpawn / 2;
		float startPos;
		if (m_AmountToSpawn & 1)
			startPos = -halfAmount * spacing;
		else
			startPos = -(halfAmount - .5f) * spacing;
		for (int i = 0; i < m_AmountToSpawn; ++i)
		{
			P2D::BodyDef bodyDef;
			bodyDef.position.y = 10.f;
			bodyDef.position.x = startPos + i * spacing + ((rand() % 3) - 1) * .01f;
			P2D::Body* pBody = m_PhysicsWorld.CreateBody(bodyDef);
			P2D::CircleShapeDef circleDef;
			circleDef.radius = 1.f;
			P2D::CircleShape* pCircle = m_PhysicsWorld.CreateShape(circleDef);
			pBody->AddShape(pCircle);
		}
	}

	ImGui::DragInt("Polygon Sides", &m_PolySides, 0.1f, 3, 8);

	if (ImGui::Button("Spawn Polygon"))
	{
		int halfAmount = m_AmountToSpawn / 2;
		float startPos;
		if (m_AmountToSpawn & 1)
			startPos = -halfAmount * spacing;
		else
			startPos = -(halfAmount - .5f) * spacing;
		for (int i = 0; i < m_AmountToSpawn; ++i)
		{
			P2D::BodyDef bodyDef;
			bodyDef.position.y = 10.f;
			bodyDef.position.x = startPos + i * spacing + ((rand() % 3) - 1) * .01f;
			P2D::Body* pBody = m_PhysicsWorld.CreateBody(bodyDef);
			P2D::PolygonShapeDef shapeDef;
			P2D::PolygonShape* pShape = m_PhysicsWorld.CreateShape(shapeDef);
			pShape->SetAsRegularPolygon(m_PolySides, 1.f);
			pBody->AddShape(pShape);
		}
	}
}

void StressTestScene::CleanUp()
{
}
