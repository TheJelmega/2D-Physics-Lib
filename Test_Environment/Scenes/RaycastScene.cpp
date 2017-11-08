#include "RaycastScene.h"
#include "../Imgui/imgui-SFML.h"
#include "../Imgui/imgui.h"


RaycastScene::RaycastScene()
	: Scene("Raycast scene")
	, m_HitType(P2D::Shape::Type::None)
	, m_Angle(0)
{
}


RaycastScene::~RaycastScene()
{
}

void RaycastScene::Init(Context& context)
{
	(void)context;
	m_Input.length = 5.f;


	P2D::BodyDef bodyDef;
	bodyDef.type = P2D::BodyType::Static;
	bodyDef.position = f32v2(0.f, 5.f);
	P2D::Body* pBody = m_PhysicsWorld.CreateBody(bodyDef);

	P2D::CircleShapeDef circleDef;
	circleDef.radius = 1.f;
	P2D::CircleShape* pCricle = m_PhysicsWorld.CreateShape(circleDef);
	pBody->AddShape(pCricle);

	bodyDef.position = f32v2(5.f, 0.f);
	pBody = m_PhysicsWorld.CreateBody(bodyDef);

	P2D::PolygonShapeDef polyDef;
	P2D::PolygonShape* pPoly = m_PhysicsWorld.CreateShape(polyDef);
	pPoly->SetAsRegularPolygon(5, 1.f);
	pBody->AddShape(pPoly);

	bodyDef.position = f32v2(0.f, -4.5f);
	pBody = m_PhysicsWorld.CreateBody(bodyDef);

	P2D::EdgeShapeDef edgeDef;
	edgeDef.v0 = f32v2(-2.5f, 0.f);
	edgeDef.v1 = f32v2(2.5f, 0.f);
	P2D::EdgeShape* pEdge = m_PhysicsWorld.CreateShape(edgeDef);
	pBody->AddShape(pEdge);

	bodyDef.position = f32v2(-4.f, 0.f);
	pBody = m_PhysicsWorld.CreateBody(bodyDef);

	P2D::ChainShapeDef chainDef;
	f32v2 arr[] = { f32v2(0.f, 2.5f), f32v2(-.5f, 0.f), f32v2(0.f, -2.5f) };
	chainDef.points = arr;
	chainDef.numPoints = 3;
	P2D::ChainShape* pChain = m_PhysicsWorld.CreateShape(chainDef);
	pBody->AddShape(pChain);
}

void RaycastScene::Update(sf::Time dt)
{
	Scene::Update(dt);

	m_Input.direction = f32v2::AxisX.Rotated(m_Angle);

	m_PhysicsWorld.Raycast(m_Input, m_Output);
	if (m_Output.hit)
		m_HitType = m_Output.pShape->GetType();
	else
		m_HitType = P2D::Shape::Type::None;
}

void RaycastScene::UpdateEvent(const sf::Event& evnt)
{
	(void)evnt;
}

void RaycastScene::Draw(DrawContext& context)
{
	Scene::Draw(context);

	float len;
	if (m_Output.hit)
		len = m_Input.length * m_Output.fraction;
	else
		len = m_Input.length;
	f32v2 end = m_Input.position + m_Input.direction * len;
	end *= DrawContext::GetPhysicsRenderScale();
	end.y = -end.y;
	f32v2 begin = m_Input.position * DrawContext::GetPhysicsRenderScale();
	begin.y = -begin.y;

	sf::Color color(102, 51, 0);
	sf::VertexArray arr(sf::PrimitiveType::Lines, 2);
	arr[0].position = sf::Vector2f(begin.x, begin.y);
	arr[0].color = color;
	arr[1].position = sf::Vector2f(end.x, end.y);
	arr[1].color = color;
	context.Draw(arr);

	sf::CircleShape circle;
	float radius = 0.02f * DrawContext::GetPhysicsRenderScale();
	circle.setRadius(radius);
	circle.setFillColor(color);

	radius *= .5f;
	circle.setPosition(begin.x - radius, begin.y - radius);
	context.Draw(circle);
	circle.setPosition(end.x - radius, end.y - radius);
	context.Draw(circle);

	//normal
	if (m_Output.hit)
	{
		begin = end;
		end += f32v2(1.f, -1.f) * m_Output.normal * 0.25f * DrawContext::GetPhysicsRenderScale();
		sf::Color color2(204, 102, 0);
		arr[0].position = sf::Vector2f(begin.x, begin.y);
		arr[0].color = color2;
		arr[1].position = sf::Vector2f(end.x, end.y);
		arr[1].color = color2;
		context.Draw(arr);
	}
}

void RaycastScene::OnGui(Context& context)
{
	Scene::OnGui(context);

	float angle = m_Angle * 180.f / P2D::Math::Pi<float>;
	ImGui::DragFloat("Ray angle", &angle, 0.5f, -360.f, 360.f);
	m_Angle = P2D::Math::ToRadians(angle);


	switch (m_HitType)
	{
	case P2D::Shape::Type::None:
		ImGui::Text("Hit: None");
		break;
	case P2D::Shape::Type::Circle:
		ImGui::Text("Hit: Circle");
		break;
	case P2D::Shape::Type::Polygon:
		ImGui::Text("Hit: Polygon");
		break;
	case P2D::Shape::Type::Edge:
		ImGui::Text("Hit: Edge");
		break;
	case P2D::Shape::Type::Chain:
		ImGui::Text("Hit: Chain");
		break;
	default: ;
	}
}

void RaycastScene::CleanUp()
{
}
