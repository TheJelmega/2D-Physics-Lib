#include "DrawContext.h"



DrawContext::DrawContext(sf::RenderWindow& window) : m_Window(window)
{
}


DrawContext::~DrawContext()
{
}

void DrawContext::Draw(const P2D::World& world)
{
	//Draw bodies
	for (const P2D::Body* pBody = world.GetBodyList(); pBody; pBody = pBody->GetNext())
	{
		Draw(pBody);
	}

	//Draw Contact points
	if (m_DrawCollisionPoints)
	{
		for (const P2D::Contact* pContact = world.GetTouchingContactList(); pContact; pContact = pContact->GetNextTouching())
		{
			const P2D::Manifold& manifold = pContact->GetManifold();

			for (u32 i = 0; i < manifold.numPairs; ++i)
			{
				const P2D::ManifoldPair& pair = manifold.pairs[i];
				sf::CircleShape circle;
				circle.setFillColor(sf::Color(63, 0, 0));
				f32 radius = m_ContactSize * m_PhysicsRenderScale;
				circle.setRadius(radius);
				//Point 0
				f32v2 pos = pair.position0 * m_PhysicsRenderScale;
				pos.y = -pos.y;
				pos -= radius;
				circle.setPosition(pos.x, pos.y);
				Draw(circle);

				//Point 1
				circle.setFillColor(sf::Color::Red);
				pos = pair.position1 * m_PhysicsRenderScale;
				pos.y = -pos.y;
				pos -= radius;
				circle.setPosition(pos.x, pos.y);
				Draw(circle);
			}
		}
	}
}

void DrawContext::Draw(const sf::Drawable& drawable) const
{
	m_Window.draw(drawable);
}

void DrawContext::Draw(sf::Vertex* vertices, size_t vertexCount, sf::PrimitiveType type) const
{
	m_Window.draw(vertices, vertexCount, type);
}

void DrawContext::Draw(const P2D::Body* pBody)
{
	if (pBody == nullptr)
		return;

	for (const P2D::Shape* pShape = pBody->GetShapes(); pShape; pShape = pShape->GetNext())
	{
		switch (pShape->GetType())
		{
		case P2D::Shape::Type::Circle:
			Draw(pBody, static_cast<const P2D::CircleShape*>(pShape));
			break;
		case P2D::Shape::Type::Edge:
			Draw(pBody, static_cast<const P2D::EdgeShape*>(pShape));
			break;
		case P2D::Shape::Type::Chain:
			Draw(pBody, static_cast<const P2D::ChainShape*>(pShape));
			break;
		case P2D::Shape::Type::Polygon:
			Draw(pBody, static_cast<const P2D::PolygonShape*>(pShape));
			break;
		default:
			break;
		}
	}

	//Draw forces
	if (m_DrawForces)
	{
		f32v2 pos = pBody->GetTransform().Move(pBody->GetMassData().centerOfMass) * m_PhysicsRenderScale;
		pos.y = -pos.y;
		f32v2 linearForce = pBody->GetLinearVelocity();
		f32 radius = linearForce.Length() * m_PhysicsRenderScale;
		f32 angle = linearForce.Angle();
		sf::VertexArray line(sf::PrimitiveType::Lines, 2);
		line[0].position = sf::Vector2f(pos.x, pos.y);
		line[0].color = sf::Color::Blue;
		line[1].position = sf::Vector2f(pos.x + radius * cos(angle), pos.y - radius * sin(angle));
		line[1].color = sf::Color::Blue;

		m_Window.draw(line);
	}
}

void DrawContext::SetDefaultView() const
{
	m_Window.setView(m_Window.getDefaultView());
}

sf::RenderWindow& DrawContext::GetWindow() const
{
	return m_Window;
}

void DrawContext::Draw(const P2D::Body* pBody, const P2D::CircleShape* pShape) const
{
	sf::CircleShape circle;
	f32 radius = pShape->GetRadius() * m_PhysicsRenderScale;
	circle.setRadius(radius);
	f32v2 pos = (pBody->GetPosition() + pShape->GetRelPosition().Rotated(pBody->GetAngle())) * m_PhysicsRenderScale;
	pos.y = -pos.y;
	pos -= radius;
	circle.setPosition(pos.x, pos.y);

	sf::Color color = pBody->IsAwake() ? m_Color : m_SleepColor;
	circle.setFillColor(color);
	circle.setOutlineColor(sf::Color(63, 63, 63));
	circle.setOutlineThickness(1.f);

	m_Window.draw(circle);

	//Angle line
	pos += radius;
	f32 angle = pBody->GetAngle();
	sf::VertexArray line(sf::PrimitiveType::Lines, 2);
	line[0].position = sf::Vector2f(pos.x, pos.y);
	line[0].color = sf::Color::Red;
	line[1].position = sf::Vector2f(pos.x + radius * cos(angle), pos.y - radius * sin(angle));
	line[1].color = sf::Color::Red;

	m_Window.draw(line);

}

void DrawContext::Draw(const P2D::Body* pBody, const P2D::EdgeShape* pShape) const
{
	const f32v2& relPos = pShape->GetRelPosition();
	const P2D::Transform& transform = pBody->GetTransform();
	//f32v2 pos = (pBody->GetPosition() + pShape->GetRelPosition().Rotated(pBody->GetAngle())) * m_PhysicsRenderScale;
	f32v2 v0 = relPos + pShape->GetV0();
	v0 = transform.Move(v0) * m_PhysicsRenderScale;
	f32v2 v1 = relPos + pShape->GetV1();
	v1 = transform.Move(v1) * m_PhysicsRenderScale;

	sf::VertexArray line(sf::PrimitiveType::Lines, 2);
	line[0].position = sf::Vector2f(v0.x, -v0.y);
	line[0].color = sf::Color(127, 127, 192);
	line[1].position = sf::Vector2f(v1.x, -v1.y);
	line[1].color = sf::Color(127, 127, 192);

	m_Window.draw(line);

	f32v2 pos = v0 + v1;
	pos *= .5f;
	f32v2 normalPos = pShape->GetNormal().Rotated(transform.rotation);
	normalPos *= m_NormalLength * m_PhysicsRenderScale;
	line[0].position = sf::Vector2f(pos.x, -pos.y);
	line[0].color = sf::Color(191, 191, 191);
	line[1].position = sf::Vector2f(pos.x + normalPos.x, -pos.y - normalPos.y);
	line[1].color = sf::Color(191, 191, 191);

	m_Window.draw(line);
}

void DrawContext::Draw(const P2D::Body* pBody, const P2D::ChainShape* pShape) const
{
	u32 numPoints = pShape->GetNumPoints();
	const f32v2* points = pShape->GetPoints();
	const f32v2& relPos = pShape->GetRelPosition();
	const P2D::Transform& transform = pBody->GetTransform();
	
	if (numPoints > 2)
	{
		f32v2 v0 = relPos + points[0];
		v0 = transform.Move(v0) * m_PhysicsRenderScale;
		//f32v1

		for (u32 i = 1; i < numPoints; ++i)
		{
			f32v2 v1 = relPos + points[i];
			v1 = transform.Move(v1) * m_PhysicsRenderScale;

			sf::VertexArray line(sf::PrimitiveType::Lines, 2);
			line[0].position = sf::Vector2f(v0.x, -v0.y);
			line[0].color = sf::Color(127, 127, 192);
			line[1].position = sf::Vector2f(v1.x, -v1.y);
			line[1].color = sf::Color(127, 127, 192);

			m_Window.draw(line);

			f32v2 pos = v0 + v1;
			pos *= .5f;
			f32v2 normal = (v1 - v0).Cross(1.f).Normalized();
			f32v2 normalPos = normal * m_NormalLength * m_PhysicsRenderScale;
			line[0].position = sf::Vector2f(pos.x, -pos.y);
			line[0].color = sf::Color(191, 191, 191);
			line[1].position = sf::Vector2f(pos.x + normalPos.x, -pos.y - normalPos.y);
			line[1].color = sf::Color(191, 191, 191);
			m_Window.draw(line);

			v0 = v1;
		}
	}
}

void DrawContext::Draw(const P2D::Body* pBody, const P2D::PolygonShape* pShape) const
{
	u32 numPoints = pShape->GetNumPoints();
	const f32v2* points = pShape->GetPoints();
	const f32v2& relPos = pShape->GetRelPosition();
	const P2D::Transform& transform = pBody->GetTransform();
	sf::Color color = pBody->IsAwake() ? m_Color : m_SleepColor;

	if (numPoints > 2)
	{
		f32v2 v0 = relPos + points[0];
		v0 = transform.Move(v0) * m_PhysicsRenderScale;
		//f32v1

		for (u32 i = 0; i < numPoints; ++i)
		{
			u32 i1 = i + 1;
			if (i1 == numPoints)
				i1 = 0;

			f32v2 v1 = relPos + points[i1];
			v1 = transform.Move(v1) * m_PhysicsRenderScale;

			sf::VertexArray line(sf::PrimitiveType::Lines, 2);
			line[0].position = sf::Vector2f(v0.x, -v0.y);
			line[0].color = color;
			line[1].position = sf::Vector2f(v1.x, -v1.y);
			line[1].color = color;

			m_Window.draw(line);

			v0 = v1;
		}
	}
}
