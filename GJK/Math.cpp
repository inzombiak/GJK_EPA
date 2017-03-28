#include "Math.h"

float sfmath::Dot(const sf::Vector2f& v1, const sf::Vector2f& v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}
float sfmath::Length2(const sf::Vector2f& v)
{
	return (v.x * v.x + v.y  * v.y);
}
float sfmath::Length(const sf::Vector2f& v)
{
	return sqrt(Length2(v));
}

float sfmath::Cross(const sf::Vector2f& v1, const sf::Vector2f& v2)
{
	float result = (v1.x * v2.y) - (v1.y * v2.x);

	result = roundf(result * 100) / 100;

	return result;
}

sf::Vector3f sfmath::Cross3D(const sf::Vector3f& v1, const sf::Vector3f& v2)
{
	sf::Vector3f result;

	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;

	return result;
}

bool sfmath::SameSideOfLine(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& p1, const sf::Vector2f& p2)
{
	float cp1 = Cross(b - a, p1 - a);
	float cp2 = Cross(b - a, p2 - a);

	return ((cp1 < 0) != (cp2 < 0));

}

bool sfmath::PointInTriangle(const sf::Vector2f& p0, const sf::Vector2f& p1, const sf::Vector2f& p2, const sf::Vector2f& p)
{
	float s = p0.y*p2.x - p0.x*p2.y + (p2.y - p0.y)*p.x + (p0.x - p2.x)*p.y;
	float t = p0.x*p1.y - p0.y*p1.x + (p0.y - p1.y)*p.x + (p1.x - p0.x)*p.y;

	if ((s < 0) != (t < 0))
		return false;

	float A = -p1.y * p2.x + p0.y * (p2.x - p1.x) + p0.x * (p1.y - p2.y) + p1.x * p2.y;
	if (A < 0.0)
	{
		s = -s;
		t = -t;
		A = -A;
	}
	return s > 0 && t > 0 && (s + t) <= A;
}

bool sfmath::LineLineIntersect(const sf::Vector2f vA, const sf::Vector2f vB, const sf::Vector2f uA, const sf::Vector2f uB, sf::Vector2f& intersectPoint)
{
	sf::Vector2f vDir = (vB - vA);
	sf::Vector2f uDir = (uB - uA);

	//Cross product of the directions
	float dirCross = Cross(vDir, uDir);
	sf::Vector2f originDiff = uA - vA;

	if (dirCross == 0 && Cross(originDiff, vDir) == 0)
	{
		float vDot = Dot(vDir, vDir);
		float t0 = Dot(originDiff, vDir) / vDot;
		float t1 = t0 + (Dot(uDir, vDir) / vDot);
					
		if (t0 >= 0 && t0 <= 1)
		{
			t0 = roundf(t0 * 100) / 100;
			intersectPoint = vA + t0 * vDir;
			return true;
		}
		else if (t1 >= 0 && t1 <= 1)
		{
			t1 = roundf(t1 * 100) / 100;
			intersectPoint = vA + t1 * vDir;
			return true;
		}
			return false;

	}
	else
	{
		float t = (Cross(originDiff, uDir)) / dirCross;
		float u = (Cross(originDiff, vDir)) / dirCross;
		if (t >= 0 && t <= 1 && u >= 0 && u <= 1)
		{
			t = roundf(t * 100) / 100;
			intersectPoint = vA + (float)t * vDir;
			return true;
		}
	}
	
	return false;
}

bool sfmath::RayLineIntersect(const Ray& ray, const sf::Vector2f a, const sf::Vector2f b)
{
	//Read this http://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect/565282#565282
	//The direction of the line, normalized
	sf::Vector2f lineDir = b - a;

	//Cross product of the directions
	float dirCross = Cross(ray.direction, lineDir);
	if (dirCross == 0)
		return false;

	sf::Vector2f originDiff = a - ray.origin;
	float t = (Cross(originDiff, lineDir)) / dirCross;
	float u = (Cross(originDiff, ray.direction)) / dirCross;

	//Since t is for ray it just needs to be >= 0 and u needs to between 0 and 1
	return ((t >= 0) & (u >= 0) & (u <= 1));
}

bool sfmath::IsReflex(const sf::Vector2f& p, const sf::Vector2f& prev, const sf::Vector2f& next, bool counterClockwise)
{
	sf::Vector2f vec1, vec2;
	float angle;
	vec1 = prev - p;
	vec2 = next - p;

	angle = Angle(vec1, vec2);
	printf("Angle: %lf \n", angle);
	if (angle > 0)
		return false;

	return true;
}

std::vector<sf::Vector2f> sfmath::InvertShape(const std::vector<sf::Vector2f>& vertices, sf::Vector2f origin)
{
	std::vector<sf::Vector2f> result;
	result.reserve(vertices.size());
	for (unsigned int i = 0; i < vertices.size(); ++i)
	{
		result.push_back(2.f*origin - vertices[i]);
	}
	return result;
}

int sfmath::Mod(int i, int base)
{
	return ((i % base) + base) % base;

}

float sfmath::Angle(const sf::Vector2f& v1, const sf::Vector2f& v2)
{
	return atan2(sfmath::Cross(v1, v2), sfmath::Dot(v1, v2));
}