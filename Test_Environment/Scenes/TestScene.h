#pragma once
#include "../Scene.h"

class TestScene : public Scene
{
public:
	TestScene();
	~TestScene();

	void Init(Context& context) override;
	void Update(sf::Time dt) override;
	void UpdateEvent(const sf::Event& evnt) override;
	void Draw(DrawContext& context) override;
	void CleanUp() override;

	void OnGui(Context& context) override;
private:

	/*sf::CircleShape m_Circle;

	P2D::Body* m_pBody;
	P2D::Body *m_pStaticBody0, *m_pStaticBody1;*/
	
};

