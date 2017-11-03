#pragma once
#include "../Scene.h"


class StressTestScene : public Scene
{
public:
	StressTestScene();
	~StressTestScene();
	void Init(Context& context) override;
	void Update(sf::Time dt) override;
	void UpdateEvent(const sf::Event& evnt) override;
	void Draw(DrawContext& context) override;
	void OnGui(Context& context) override;
	void CleanUp() override;

private:

	int m_PolySides = 3;
	int m_AmountToSpawn = 1;
};

