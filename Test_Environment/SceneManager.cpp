#include "SceneManager.h"



SceneManager::SceneManager() : m_CurrentScene(0)
{
}


SceneManager::~SceneManager()
{
}

void SceneManager::Init(Context& context)
{
	for (Scene* pScene : m_pScenes)
	{
		pScene->Init(context);
	}
}

void SceneManager::Update(sf::Time dt)
{
	if (m_HasScene)
		m_pScenes[m_CurrentScene]->Update(dt);
}

void SceneManager::UpdateEvent(const sf::Event& e)
{
	if (m_HasScene)
		m_pScenes[m_CurrentScene]->UpdateEvent(e);
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
