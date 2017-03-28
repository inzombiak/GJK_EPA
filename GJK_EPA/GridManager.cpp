#include "GridManager.h"

#include <iostream>
#include <unordered_map>

#include "Math.h"

GridManager::GridManager()
{
	m_pointer = sf::CircleShape(CIRCLE_RADIUS);
	m_pointer.setFillColor(sf::Color::Red);

	m_labelFont.loadFromFile("arial.ttf");
}

void GridManager::GenerateGrid(int windowWidth, int windowHeight, unsigned int rows, unsigned int columns)
{
	m_windowHeight = windowHeight;
	m_windowWidth = windowWidth;
	m_rowCount = rows;
	m_columnCount = columns;

	m_xSpacing = (float)(m_windowWidth - LINE_WIDTH * columns) / ((float)m_columnCount + 1);
	m_ySpacing = (float)(m_windowHeight - LINE_WIDTH * rows) / (float)(m_rowCount + 1);

	for (int i = 0; i < m_rowCount; ++i)
	{
		sf::RectangleShape newLine(sf::Vector2f(m_windowWidth , LINE_WIDTH));
		newLine.setPosition(0, (m_ySpacing + LINE_WIDTH) * (i + 1));
		newLine.setFillColor(sf::Color(0, 0, 0, 25));

		if (i == m_rowCount / 2)
		{
			newLine.setFillColor(sf::Color::Blue);
			ORIGIN_OFFSET.y = std::round((m_ySpacing + LINE_WIDTH) * (i + 1));
			//newLine.setSize(sf::Vector2f(m_windowWidth, 2));
		}
		
		m_gridLines.push_back(newLine);
	}
	

	for (int j = 0; j < m_columnCount; ++j)
	{
		sf::RectangleShape newLine(sf::Vector2f(m_windowHeight, LINE_WIDTH));
		newLine.setPosition((m_xSpacing + LINE_WIDTH) * (j + 1), 0);
		newLine.setFillColor(sf::Color(0, 0, 0, 25));

		if (j == m_columnCount / 2)
		{
			newLine.setFillColor(sf::Color::Blue);
			ORIGIN_OFFSET.x = std::round((m_xSpacing + LINE_WIDTH) * (j + 1));
			//newLine.setSize(sf::Vector2f(m_windowHeight, 2));
		}

		newLine.rotate(90.f);
		m_gridLines.push_back(newLine);
	}
}

void GridManager::Draw(sf::RenderWindow& rw)
{
	for (unsigned int i = 0; i < m_gridLines.size(); ++i)
		rw.draw(m_gridLines[i]);

	if (m_drawPointer)
		rw.draw(m_pointer);

	for (unsigned int i = 0; i < m_shapeVertices.size(); ++i)
	{
		rw.draw(m_shapeVertices[i]);
	}

	for (unsigned int i = 0; i < m_convexShapes.size(); ++i)
	{
		m_convexShapes[i].Draw(rw);
	}

	for (unsigned int i = 0; i < m_concaveShapes.size(); ++i)
	{
		m_concaveShapes[i].Draw(rw);
	}
}

void GridManager::AddVertex()
{
	m_shapeVertexPos.push_back(m_pointer.getPosition() + sf::Vector2f(CIRCLE_RADIUS, CIRCLE_RADIUS));

	sf::CircleShape vertex(CIRCLE_RADIUS);
	vertex.setFillColor(sf::Color::Green);
	vertex.setPosition(sf::Vector2f(m_pointer.getPosition()));
	m_shapeVertices.push_back(vertex);

	//Shoelace formula/Concave check
	int currVertex = m_shapeVertexPos.size() - 1;
	if (currVertex > 0)
	{
		m_shoelaceSum += (m_shapeVertexPos[currVertex].x - m_shapeVertexPos[currVertex - 1].x)*(m_shapeVertexPos[currVertex].y + m_shapeVertexPos[currVertex - 1].y);
		if (currVertex > 1)
		{
			if (!m_shapeIsConcave && sfmath::IsReflex(m_shapeVertexPos[currVertex - 1], m_shapeVertexPos[currVertex - 2], m_shapeVertexPos[currVertex]))
			{
				m_shapeIsConcave = true;
			}
		}
	}
		
}

void GridManager::MoveDot(const sf::Vector2f& mousePos)
{
	sf::Vector2i snappedPos = SnapToGrid(mousePos);
	m_pointer.setPosition(snappedPos.x, snappedPos.y);
}

void GridManager::MoveShape(const sf::Vector2f& mousePos)
{
	if (m_currentShape == NULL_SHAPE)
		return;
	if (m_currentShape.second == Concave)
		m_concaveShapes[m_currentShape.first].SetPosition(mousePos);
	else
		m_convexShapes[m_currentShape.first].SetPosition(mousePos);
}

void GridManager::DropShape()
{
	if (m_currentShape == NULL_SHAPE)
		return;
	if (m_currentShape.second == Concave)
		m_concaveShapes[m_currentShape.first].Selected(false, sf::Vector2f(0, 0));
	else
		m_convexShapes[m_currentShape.first].Selected(false, sf::Vector2f(0, 0));
	m_currentShape = NULL_SHAPE;
}

sf::Vector2i GridManager::SnapToGrid(const sf::Vector2f& pos)
{																									  
	int x = static_cast<int>(std::round(pos.x / (m_xSpacing + LINE_WIDTH)) * (m_xSpacing + LINE_WIDTH));
	int y = static_cast<int>(std::round(pos.y / (m_ySpacing + LINE_WIDTH)) * (m_ySpacing + LINE_WIDTH));

	return sf::Vector2i(x - CIRCLE_RADIUS, y - CIRCLE_RADIUS /2);
}

void GridManager::CreateShape()
{
	if (m_shapeVertexPos.size() <= 2)
		return;
	int vertexCount = m_shapeVertexPos.size();
	m_shoelaceSum += (m_shapeVertexPos[0].x - m_shapeVertexPos[vertexCount - 1].x)*(m_shapeVertexPos[0].y + m_shapeVertexPos[vertexCount - 1].y);
	if (m_shoelaceSum < 0)
		std::reverse(std::begin(m_shapeVertexPos), std::end(m_shapeVertexPos));
	if (!m_shapeIsConcave && (sfmath::IsReflex(m_shapeVertexPos[0], m_shapeVertexPos[vertexCount - 1], m_shapeVertexPos[1]) ||
							  sfmath::IsReflex(m_shapeVertexPos[vertexCount - 1], m_shapeVertexPos[vertexCount - 2], m_shapeVertexPos[0])))
		m_shapeIsConcave = true;

	if (m_shapeIsConcave)
		CreateConcaveShape(m_shapeVertexPos);
	else
		CreateConvexShape(m_shapeVertexPos);

	m_shapeVertices.clear();
	m_shapeVertexPos.clear();
	m_shapeIsConcave = false;
	m_shoelaceSum = 0;
}

void GridManager::ShowPointer(bool val)
{
	m_drawPointer = val;
}

void GridManager::DeleteCurrentShape()
{
	if (m_currentShape == NULL_SHAPE)
		return;

	DeleteShape(m_currentShape);
}

//TODO: Clean up
void GridManager::SelectShape(const sf::Vector2f& mousePos)
{
	if (m_currentShape != NULL_SHAPE)
		if (m_currentShape.second == Concave)
			m_concaveShapes[m_currentShape.first].Selected(false, sf::Vector2f(0, 0));
		else
			m_convexShapes[m_currentShape.first].Selected(false, sf::Vector2f(0, 0));


	if (!GetShapeContainingPoint(mousePos))
		return;
	if (m_currentShape.second == Concave)
	{
		m_concaveShapes[m_currentShape.first].Selected(true, mousePos);
	}
	else
	{
		m_convexShapes[m_currentShape.first].Selected(true, mousePos);
	}
}

void GridManager::MarkShape(const sf::Vector2f& mousePos)
{
	if (!GetShapeContainingPoint(mousePos))
		return;
	IShape* shape = 0;

	if (m_currentShape.second == Concave)
		shape = &m_concaveShapes[m_currentShape.first];
	else
		shape = &m_convexShapes[m_currentShape.first];

	//Deselect shape
	
	IShape* shapeA = 0;
	if (m_shapeA.first != -1)
	{
		if (m_shapeA.second == Concave)
			shapeA = &m_concaveShapes[m_shapeA.first];
		else
			shapeA = &m_convexShapes[m_shapeA.first];
	}
	IShape* shapeB = 0;
	if (m_shapeB.first != -1)
	{
		if (m_shapeB.second == Concave)
			shapeB = &m_concaveShapes[m_shapeB.first];
		else
			shapeB = &m_convexShapes[m_shapeB.first];
	}

	if (shape != 0)
	{
		if (shapeA != NULL && m_currentShape == m_shapeA)
		{
			shape->Marked(false);
			shape->Selected(false, mousePos);
			shape->SetText("");
			if (shapeB != NULL)
			{
				m_shapeA = m_shapeB;

				shapeB->SetFillColor(MARKED_A);
				shapeB->SetText("A");

				m_shapeB = std::make_pair(-1, ShapeType::Concave);
			}
			else
				m_shapeA = std::make_pair(-1, ShapeType::Concave);

		}
		else if (shapeB != NULL &&  m_currentShape == m_shapeB)
		{
			shape->Marked(false);
			shape->Selected(false, mousePos);
			shape->SetText("");

			m_shapeB = std::make_pair(-1, ShapeType::Concave);
		}
		else
		{
			if (shapeA == 0)
			{
				m_shapeA = m_currentShape;
				shape->SetFillColor(MARKED_A);
				shape->SetText("A");
				shape->Marked(true);
			}
			else if (shapeB == 0)
			{
				m_shapeB = m_currentShape;
				shape->SetFillColor(MARKED_B);
				shape->SetText("B");
				shape->Marked(true);
			}
			else
			{
				shapeA->Marked(false);
				shapeA->Selected(false, mousePos);
				shapeA->SetText("");

				m_shapeA = m_shapeB;

				shapeB->Marked(true);
				shapeB->SetFillColor(MARKED_A);
				shapeB->SetText("A");

				m_shapeB = m_currentShape;
				shape->Marked(true);
				shape->SetFillColor(MARKED_B);
				shape->SetText("B");

			}
		}
	}

	m_currentShape = NULL_SHAPE;
}



void GridManager::CalculateGJK()
{
	if (m_shapeA == NULL_SHAPE || m_shapeB == NULL_SHAPE)
		return;
	IShape *a, *b;

	if (m_shapeA.second == Concave)
		a = &m_concaveShapes[m_shapeA.first];
	else
		a = &m_convexShapes[m_shapeA.first];

	if (m_shapeB.second == Concave)
		b = &m_concaveShapes[m_shapeB.first];
	else
		b = &m_convexShapes[m_shapeB.first];



	m_gjkCalc.CalculateGJKFull(a->GetVerticies(), b->GetVerticies());
}

bool GridManager::IsEar(const Node& n, const std::vector<Node>& nodes, const std::vector<sf::Vector2f>& vertices)
{
	bool isEar = true;

	for (unsigned int j = 0; j < nodes.size(); ++j)
	{
		if (nodes[j].index != n.index &&  nodes[j].index != n.next && nodes[j].index != n.prev)
			if (sfmath::PointInTriangle(vertices[n.index], vertices[n.prev], vertices[n.next], vertices[nodes[j].index]))
			{
				isEar = false;
				break;
			}
	}

	return isEar;
}

//See https://www.geometrictools.com/Documentation/TriangulationByEarClipping.pdf for ear clipping
void GridManager::CreateConcaveShape(const std::vector<sf::Vector2f>& vertices)
{
	//Used ear clipping method to decompose polygon

	int pointCount = vertices.size();
	std::vector<sf::ConvexShape> shapes;
	std::vector<Node> nodes;
	nodes.resize(pointCount);
	std::vector<int> reflexVertices; //for ear clipping
	std::vector<int> reflexVerticesForShape; //to pass to shape for M-sum
	reflexVertices.resize(pointCount, -1);
	std::vector<int> convexVertices;
	convexVertices.resize(pointCount, -1);
	std::unordered_map<int, int> ears;


	for (int i = 0; i < pointCount; ++i)
	{
		nodes[i].value = vertices[i];
		nodes[i].index = i;
		nodes[i].prev = sfmath::Mod((i - 1), pointCount);
		nodes[i].next = sfmath::Mod((i + 1), pointCount);
	}
	/*
	Just in case passing nodes vector starts shwoing bugs, gonna save this.
	std::vector<int> temp;
	temp.reserve(reflexVertices.size() + convexVertices.size());
	temp.insert(temp.end(), reflexVertices.begin(), reflexVertices.end());
	temp.insert(temp.end(), convexVertices.begin(), convexVertices.end());
	*/
	{
		for (int i = 0; i < pointCount; ++i)
		{

			printf("%i \n", i);
			if (sfmath::IsReflex(nodes[i].value, nodes[nodes[i].prev].value, nodes[nodes[i].next].value))
			{
				reflexVertices[i] = i;
				reflexVerticesForShape.push_back(i);
				nodes[i].isReflex = true;
				printf(" %i is reflex \n", i);
			}
			else
			{
				convexVertices[i] = i;
			}

			if (!nodes[i].isReflex)
				if (IsEar(nodes[i], nodes, vertices))
					ears[i] = i;
		}
	}


	sf::ConvexShape cs;
	int steps = 0;
	while (!ears.empty())
	{
		int ear = ears.begin()->second;
		ears.erase(ear);

		Node currentNode = nodes[ear];
		int next = currentNode.next;
		int prev = currentNode.prev;

		if ((next == prev) || (next == ear) || (prev == ear))
			break;

		cs = sf::ConvexShape(3);
		cs.setPoint(0, currentNode.value);
		cs.setPoint(1, nodes[currentNode.prev].value);
		cs.setPoint(2, nodes[currentNode.next].value);
		cs.setOutlineThickness(0);
		shapes.push_back(cs);

		nodes[prev].next = next;
		nodes[next].prev = prev;

		if (nodes[prev].isReflex)
		{
			if (!(nodes[prev].isReflex = sfmath::IsReflex(nodes[prev].value, nodes[nodes[prev].prev].value, nodes[nodes[prev].next].value)))
			{
				reflexVertices[prev] = -1;
				convexVertices[prev] = prev;
			}
		}

		if (nodes[next].isReflex)
		{
			if (!(nodes[next].isReflex = sfmath::IsReflex(nodes[next].value, nodes[nodes[next].prev].value, nodes[nodes[next].next].value)))
			{
				reflexVertices[next] = -1;
				convexVertices[next] = next;
			}
		}

		auto it = ears.begin();
		while (it != ears.end())
		{
			if (!IsEar(nodes[it->second], nodes, vertices))
				it = ears.erase(it);
			else
				it++;
		}

		if (!nodes[prev].isReflex)
			if (ears.find(prev) == ears.end() && IsEar(nodes[prev], nodes, vertices)) //; not sure why this was here
				ears[prev] = prev;

		if (!nodes[next].isReflex)
			if (ears.find(next) == ears.end() && IsEar(nodes[next], nodes, vertices)) //; not sure why this was here
				ears[next] = next;

		steps++;
	}

	ConcaveShape concaveShape(shapes, vertices, reflexVerticesForShape);
	concaveShape.SetFont(m_labelFont);
	if (!m_unusedConcaveShapes.empty())
	{
		int index = m_unusedConcaveShapes.front();
		m_concaveShapes[index] = concaveShape;
		m_unusedConcaveShapes.pop();
	}
	else
		m_concaveShapes.push_back(concaveShape);
}

void GridManager::CreateConvexShape(const std::vector<sf::Vector2f>& vertices)
{
	sf::ConvexShape shape;
	shape.setPointCount(vertices.size());
	bool isConcave = false;
	int pointCount = vertices.size();
	for (int i = 0; i < pointCount; ++i)
	{
		if (sfmath::IsReflex(vertices[i], vertices[sfmath::Mod((i - 1), pointCount)], vertices[sfmath::Mod((i + 1), pointCount)]))
		{
			isConcave = true;
			break;
		}
		else
		{
			shape.setPoint(i, vertices[i]);
		}

	}

	shape.setFillColor(sf::Color(125, 125, 0, 50));
	ConvexShape cs(shape);
	cs.SetFont(m_labelFont);
	if (!m_unusedConvexShapes.empty())
	{
		int index = m_unusedConvexShapes.front();
		m_convexShapes[index] = cs;
		m_unusedConvexShapes.pop();
	}
	else
		m_convexShapes.push_back(cs);
}

//TODO: BAD FUNCTION
bool GridManager::GetShapeContainingPoint(const sf::Vector2f& point)
{
	bool found = false;

	for (unsigned int i = 0; i < m_concaveShapes.size(); ++i)
	{
		if (m_concaveShapes[i].IsInUse() && m_concaveShapes[i].ContainsPoint(point))
		{
			m_currentShape.first = i;
			m_currentShape.second = Concave;
			found = true;
			break;

		}
	}

	if (!found)
	{
		for (unsigned int i = 0; i < m_convexShapes.size(); ++i)
		{
			if (m_convexShapes[i].IsInUse() && m_convexShapes[i].ContainsPoint(point))
			{
				m_currentShape.first = i;
				m_currentShape.second = Convex;
				found = true;
				break;

			}
		}
	}

	if (!found)
		m_currentShape = NULL_SHAPE;

	return found;
}

void GridManager::DeleteShape(std::pair<int, ShapeType> shapeDeets)
{
	if (shapeDeets.second == Concave)
	{
		m_concaveShapes[shapeDeets.first].SetInUse(false);
		m_unusedConcaveShapes.push(shapeDeets.first);
	}
	else if (shapeDeets.second == Convex)
	{
		m_convexShapes[shapeDeets.first].SetInUse(false);
		m_unusedConvexShapes.push(shapeDeets.first);
	}
}

const sf::Color GridManager::MARKED_A = sf::Color(255, 0, 0, 255);
const sf::Color GridManager::MARKED_B = sf::Color(0, 255, 0, 255);