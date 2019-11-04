#include "Node.h"

Node::Node(std::string name) 
{
	this->name = name;
	this->total = 100;
}

Node::~Node()
{
	links.clear();
	weights.clear();
}

void Node::setName(std::string inName) { name = inName; }
void Node::setId(int inId) { id = inId;  }
void Node::setTotal(float inTotal) { total = inTotal;  }
void Node::setCenter(point inPoint) { center = inPoint; }
void Node::setBox(point a, point b) { box[0] = a; box[1] = b; }
void Node::setPath(Node* n) { path = n; }

std::string Node::getName() { return name; }
int Node::getId() { return id; }
float Node::getTotal() { return total; }
point Node::getCenter() { return center; }
point Node::getBoxBottomRight() { return box[0]; }
point Node::getBoxTopLeft() { return box[1]; }
Node* Node::getPath() { return path; }

bool Node::checkOverlap(Node* test)
{

	point l1 = box[1],
		  r1 = box[0],
		  l2 = test->getBoxTopLeft(),
		  r2 = test->getBoxBottomRight();

	if (l1.x > r2.x || l2.x > r1.x)
		return false;

	if (l1.y < r2.y || l2.y < r1.y)
		return false;

	return true;
}

void Node::addLink(float w, Node* node)
{
	weights.push_back(w);
	links.push_back(node);
}

Node* Node::getLink(int i) { return links[i]; }
int Node::getTotalLinks() { return links.size(); }

float Node::getW(int i) { return weights[i]; }
void Node::setW(int i, float w)
{
	weights[i] = w;
}