#include "SceneManager.h"
#include "Imgui/imgui.h"


SceneManager::SceneManager() : m_CurrentScene(0)
{
}


SceneManager::~SceneManager()
{
}

void SceneManager::Init(Context& context)
{
	//ImGui::Ini

	for (Scene* pScene : m_pScenes)
	{
		pScene->Init(context);
	}
}

void SceneManager::Update(sf::Time dt)
{
	m_TimePassed += dt.asSeconds();
	++m_Frames;
	if (m_TimePassed >= 1.f)
	{
		m_Fps = m_Frames;
		m_Frames = 0;
		m_TimePassed -= 1.f;
	}

	if (m_HasScene)
		m_pScenes[m_CurrentScene]->Update(dt);
}

void SceneManager::UpdateEvent(const sf::Event& e)
{
	if (m_HasScene)
		m_pScenes[m_CurrentScene]->UpdateEvent(e);
}

void SceneManager::OnGui(Context& context)
{
	ImGui::Begin("Physics Library");
	if (ImGui::Button("<"))
	{
		--m_CurrentScene;
		if (m_CurrentScene < 0)
			m_CurrentScene = m_pScenes.size() - 1;
	}
	ImGui::SameLine();
	ImGui::Text("Scene: %s", m_pScenes[m_CurrentScene]->GetName());
	ImGui::SameLine();
	if (ImGui::Button(">"))
	{
		++m_CurrentScene;
		if (m_CurrentScene >= int(m_pScenes.size()))
			m_CurrentScene = 0;
	}
	ImGui::Text("Fps: %i", m_Fps);
		
	if (m_HasScene)
		m_pScenes[m_CurrentScene]->OnGui(context);

	ImGui::End();
}

void SceneManager::Draw(DrawContext& context)
{
	if (m_HasScene)
		m_pScenes[m_CurrentScene]->Draw(context);
}

void SceneManager::Cleanup()
{
	for (Scene* pScene : m_pScenes)
	{
		pScene->CleanUp();
		delete pScene;
	}
}

void SceneManager::AddScene(Scene* pScene)
{
	if (!pScene)
		return;

	m_HasScene = true;
	m_pScenes.push_back(pScene);
}

void SceneManager::NextScene()
{
	if (!m_HasScene)
		return;

	++m_CurrentScene;
	if (m_CurrentScene == int(m_pScenes.size()))
		m_CurrentScene = 0;
}

void SceneManager::PrevScene()
{
	if (!m_HasScene)
		return;

	--m_CurrentScene;
	
	if (m_CurrentScene < 0)
		m_CurrentScene = m_pScenes.size() - 1;
}
