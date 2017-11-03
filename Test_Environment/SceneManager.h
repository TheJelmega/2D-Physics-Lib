#pragma once
#include <vector>
#include "Scene.h"

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void Init(Context& context);
	void Update(sf::Time dt);
	void UpdateEvent(const sf::Event& e);
	void OnGui(Context& context);
	void Draw(DrawContext& context);
	void Cleanup();

	void AddScene(Scene* pScene);
	void NextScene();
	void PrevScene();

private:
	int m_CurrentScene;
	std::vector<Scene*> m_pScenes;
	bool m_HasScene = false;

	float m_TimePassed = 0.f;
	int m_Frames = 0;
	int m_Fps = 0;
};

