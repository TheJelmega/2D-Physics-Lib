#pragma once
#include "../Scene.h"

class RestitutionScene : public Scene
{
public:
	RestitutionScene();
	~RestitutionScene();
	void Init(Context& context) override;
	void Update(sf::Time dt) override;
	void UpdateEvent(const sf::Event& evnt) override;
	void Draw(DrawContext& context) override;
	void OnGui(Context& context) override;
	void CleanUp() override;
};

