#include "Scene.h"
#include "Imgui/imgui.h"


Scene::Scene(const char* name)
	: m_Name(name)
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
	sf::Clock clock;
	if (m_UpdatePhysics)
		m_PhysicsWorld.Update(dt.asSeconds());
	else if (m_StepPhysics)
	{
		m_PhysicsWorld.Step(m_PhysicsWorld.GetTimeStep());
		m_PhysicsWorld.ClearBodyForces();

		m_StepPhysics = false;
	}
	m_PhysicsTime = clock.getElapsedTime().asMicroseconds() * 0.001f;
}

void Scene::UpdateEvent(const sf::Event& evnt)
{
	(void)evnt;
}

void Scene::Draw(DrawContext& context)
{
	sf::Clock clock;
	context.Draw(m_PhysicsWorld);
	m_PhysicsDrawTime = clock.getElapsedTime().asMicroseconds() * 0.001f;
}

void Scene::OnGui(Context& context)
{
	ImGui::Indent(10);
	ImGui::Text("physics: %.2f ms, render: %.2f ms", m_PhysicsTime, m_PhysicsDrawTime);
	ImGui::Unindent(10);

	ImGui::Text("Body count: %i", m_PhysicsWorld.GetBodyCount());
	ImGui::Text("Shape count: %i", m_PhysicsWorld.GetShapeCount());
	ImGui::Text("Constraint count: %i", m_PhysicsWorld.GetConstraintCount());
	ImGui::Text("Joint count: %i", m_PhysicsWorld.GetJointCount());
	ImGui::Text("Contact count: %i (%i touching)", m_PhysicsWorld.GetContactCount(), m_PhysicsWorld.GetTouchingContactCount());

	
	if (m_UpdatePhysics)
	{
		if (ImGui::Button(" Pause Physics "))
		{
			m_UpdatePhysics = false;
		}
	}
	else
	{
		if (ImGui::Button("Unpause Physics"))
		{
			m_UpdatePhysics = true;
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Step Physics"))
	{
		m_StepPhysics = true;
	}

	if (ImGui::CollapsingHeader("RenderSettings"))
	{
		ImGui::Indent(10);
		bool draw = context.draw.DrawForces();
		ImGui::Checkbox("Draw forces", &draw);
		context.draw.DrawForces(draw);
		draw = context.draw.DrawCollisionPoints();
		ImGui::Checkbox("Draw collision points", &draw);
		context.draw.DrawCollisionPoints(draw);
		ImGui::Unindent(10);
	}
}

void Scene::CleanUp()
{
}
