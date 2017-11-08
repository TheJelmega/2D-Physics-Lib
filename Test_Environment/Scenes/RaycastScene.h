#pragma once
#include "../Scene.h"


class RaycastScene : public Scene
{
public:
	RaycastScene();
	~RaycastScene();

	void Init(Context& context) override;
	void Update(sf::Time dt) override;
	void UpdateEvent(const sf::Event& evnt) override;
	void Draw(DrawContext& context) override;
	void OnGui(Context& context) override;
	void CleanUp() override;

private:

	P2D::Shape::Type m_HitType;

	P2D::RaycastInput m_Input;
	P2D::RaycastOutput m_Output;
	float m_Angle;
};

