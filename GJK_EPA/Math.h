#ifndef MATH_H
#define MATH_H

#include "SFML\Graphics.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

namespace sfmath
{
	struct Ray
	{
		sf::Vector2f origin;
		sf::Vector2f direction;
	};

	float Dot(const sf::Vector2f& v1, const sf::Vector2f& v2);
	float Dot(const sf::Vector3f& v1, const sf::Vector3f& v2);

	float Length(const sf::Vector2f& v);
	float Length2(const sf::Vector2f& v);
	float Cross(const sf::Vector2f& v1, const sf::Vector2f& v2);
	float Angle(const sf::Vector2f& v1, const sf::Vector2f& v2);

	bool LineLineIntersect(const sf::Vector2f vA, const sf::Vector2f vB, const sf::Vector2f uA, const sf::Vector2f uB, sf::Vector2f& intersectPoint);
	bool RayLineIntersect(const Ray& ray, const sf::Vector2f a, const sf::Vector2f b);

	sf::Vector3f Cross3D(const sf::Vector3f& v1, const sf::Vector3f& v2);

	bool SameSideOfLine(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& p1, const sf::Vector2f& p2);
	bool PointInTriangle(const sf::Vector2f& p0, const sf::Vector2f& p1, const sf::Vector2f& p2, const sf::Vector2f& p);
	bool SameDirection(const sf::Vector2f& a, const sf::Vector2f& b);
	bool SameDirection(const sf::Vector3f& a, const sf::Vector3f& b);

	bool IsReflex(const sf::Vector2f& p, const sf::Vector2f& prev, const sf::Vector2f& next, bool counterClockwise = true);
	std::vector<sf::Vector2f> InvertShape(const std::vector<sf::Vector2f>& vertices, sf::Vector2f origin = sf::Vector2f(0.f, 0.f));
	int Mod(int i, int base);
	struct Vector2fComperator
	{
		bool operator()(sf::Vector2f const& a, sf::Vector2f const& b)
		{
			return (a.x < b.x) || (a.x == b.x && a.y < b.y);
		}
	};
	sf::Vector2f Normalize(const sf::Vector2f& vec);
}

#endif