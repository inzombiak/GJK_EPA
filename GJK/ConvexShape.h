#ifndef CONVEX_SHAPE_H
#define CONVEX_SHAPE_H

#include "IShape.h"

class ConvexShape : public IShape
{
public:

	ConvexShape(const sf::ConvexShape& newShape);
	void Draw(sf::RenderWindow& rw) override;
	bool ContainsPoint(const sf::Vector2f& point) override;

	void SetPosition(const sf::Vector2f& newPos) override;
	void SetFillColor(const sf::Color& newColor) override;

protected:
	sf::ConvexShape m_shape;
};

#endif