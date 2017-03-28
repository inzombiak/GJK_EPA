#ifndef CONCAVE_SHAPE_H
#define CONCAVE_SHAPE_H

#include "IShape.h"

class ConcaveShape : public IShape
{
public:

	ConcaveShape(const std::vector<sf::ConvexShape>& shapes, const std::vector<sf::Vector2f>& vertices, const std::vector<int>& reflexVertexIndices);
	~ConcaveShape();

	bool ContainsPoint(const sf::Vector2f& point) override;
	void SetPosition(const sf::Vector2f& newPos) override;
	void SetFillColor(const sf::Color& newColor);
	void Draw(sf::RenderWindow& rw) override;

	std::vector<int> GetReflexIndices() const;

private:;
	std::vector<sf::ConvexShape> m_shapes;
	std::vector<int> m_reflexVertices;
};

#endif
