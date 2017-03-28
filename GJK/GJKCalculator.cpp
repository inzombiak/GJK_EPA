#include "GJKCalculator.h"
#include "Math.h"

GJKCalculator::GJKCalculator()
{
}


void GJKCalculator::CalculateGJKFull(const std::vector<sf::Vector2f> shapeA, const std::vector<sf::Vector2f> shapeB)
{
	sf::Vector2f simplex[3];
	int simplexCount = 0;



}



sf::Vector2f GetSupportPoint(const std::vector<sf::Vector2f> vertices, const sf::Vector2f& dir)
{
	float max = -FLT_MAX;
	sf::Vector2f result;
	float dot;

	for (int i = 0; i < vertices.size(); ++i)
	{
		dot = sfmath::Dot(dir, vertices[i]);
		if (dot > max)
		{
			max = dot;
			result = vertices[i];
		}
	}

	return result;
}
