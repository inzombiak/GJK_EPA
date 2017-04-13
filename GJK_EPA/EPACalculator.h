#ifndef EPA_CALCULATOR_H
#define EPA_CALCULATOR_H

#include "Math.h"

struct ContactInfo
{
	sf::Vector2f posA = sf::Vector2f(0, 0);
	sf::Vector2f posB = sf::Vector2f(0, 0);
	sf::Vector2f normal = sf::Vector2f(0, 0);
	int depth = 0;

	void Reset()
	{
		posA = sf::Vector2f(0, 0);
		posB = sf::Vector2f(0, 0);
		normal = sf::Vector2f(0, 0);
		depth = 0;
	}
};

class EPACalculator
{
public:
	EPACalculator();
	ContactInfo GetContactInfo(const std::vector<sf::Vector2f>& shapeA, const std::vector<sf::Vector2f>& shapeB, std::vector<sfmath::SupportPoint>& simplex);

private:
	struct Edge
	{
		sf::Vector2f a;
		sf::Vector2f b;
		sf::Vector2f normal;
		int index;
		float distance;
	};

	Edge FindClosestEdge(const std::vector<sfmath::SupportPoint>& simplex);

};

#endif