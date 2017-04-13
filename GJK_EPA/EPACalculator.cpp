#include "EPACalculator.h"


EPACalculator::EPACalculator()
{
}

ContactInfo EPACalculator::GetContactInfo(const std::vector<sf::Vector2f>& shapeA, const std::vector<sf::Vector2f>& shapeB, std::vector<sfmath::SupportPoint>& simplex)
{
 	ContactInfo result;
	sfmath::SupportPoint nextSuppPoint;
	if (simplex.size() == 1)
	{
		const sf::Vector2f searchDirs[] =
		{
			sf::Vector2f(1.0f, 0.0f),
			sf::Vector2f(-1.0f, 0.0f),
			sf::Vector2f(0.0f, 1.0f),
			sf::Vector2f(0.0f, -1.0f)
		};

		for (int i = 0; i < 4; ++i)
		{
			nextSuppPoint = sfmath::GetSupportPoint(shapeA, shapeB, searchDirs[i]);

			if (sfmath::Length2(nextSuppPoint.position - simplex[0].position) >= 0.0001f)
			{
				simplex.push_back(nextSuppPoint);

				break;
			}
		}

	}
	
	if (simplex.size() == 2)
	{
		sf::Vector2f ab = simplex[0].position - simplex[1].position;
		sf::Vector3f searchDir3 = sfmath::Cross3D(sfmath::Cross3D(sf::Vector3f(ab.x, ab.y, 0), -sf::Vector3f(simplex[1].position.x, simplex[1].position.y, 0)), sf::Vector3f(ab.x, ab.y, 0));
		sf::Vector2f searchDir;
		searchDir.x = searchDir3.x;
		searchDir.y = searchDir3.y;
		nextSuppPoint = sfmath::GetSupportPoint(shapeA, shapeB, searchDir);

		if (sfmath::Length2(nextSuppPoint.position - simplex[0].position) < 0.0001f)
		{
			nextSuppPoint = sfmath::GetSupportPoint(shapeA, shapeB, -searchDir);
		}
		simplex.push_back(nextSuppPoint);
	}

	
	Edge e;
	while (true)
	{
		e = FindClosestEdge(simplex);
		nextSuppPoint = sfmath::GetSupportPoint(shapeA, shapeB, e.normal);

		double d = sfmath::Dot(nextSuppPoint.position, e.normal);

		if (d - e.distance < 0.0001f)
		{
			result.normal = e.normal;
			result.depth = d;
			break;
		}
		else
			simplex.insert(simplex.begin() + e.index + 1, nextSuppPoint);
	}

	sf::Vector2f lineVec = e.a - e.b;
	sf::Vector2f projO = sfmath::Dot(lineVec, -e.b)*  lineVec / sfmath::Length2(lineVec);
	float s, t;
	s = sqrt(sfmath::Length2(projO) / sfmath::Length2(lineVec));
	t = 1 - s;
	int nextI = (e.index + 1) % simplex.size();
	result.posA = s * simplex[e.index].originA + t * simplex[nextI].originA;
	result.posB = s * simplex[e.index].originB + t * simplex[nextI].originB;

 	return result;
}

EPACalculator::Edge EPACalculator::FindClosestEdge(const std::vector<sfmath::SupportPoint>& simplex)
{
	Edge result;
	int nextI;
	float minDist = FLT_MAX, currDist;
	sf::Vector2f norm, edge;
	sf::Vector3f norm3d;
	for (int i = 0; i < simplex.size(); ++i)
	{
		
		nextI = (i + 1) % simplex.size();

		edge = simplex[nextI].position - simplex[i].position;
		norm3d = sfmath::Cross3D(sfmath::Cross3D(sf::Vector3f(edge.x, edge.y, 0), sf::Vector3f(simplex[i].position.x, simplex[i].position.y, 0)), sf::Vector3f(edge.x, edge.y, 0));
		norm.x = norm3d.x;
		norm.y = norm3d.y;
		
		norm = sfmath::Normalize(norm);
		currDist = sfmath::Dot(norm, simplex[i].position);
		if (currDist < minDist)
		{ 
 			minDist = currDist;
			result.a = simplex[i].position;
			result.b = simplex[nextI].position;
			result.normal = norm;
			result.distance = currDist;
			result.index = i;
		}
	}

	return result;
}