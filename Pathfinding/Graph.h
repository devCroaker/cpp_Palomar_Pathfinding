#pragma once
#include "Node.h"

class Graph
{
private:
	std::vector<Node> nodeList;
	std::vector<Node*> queue;
	std::vector<Node*> visited;
	int destinationID;

public:
	Graph();
	~Graph();

	friend std::ostream& operator<<(std::ostream &s, Graph&g);

	Node getNode(int);
	void addToQueue(Node*);
	void addVisited(Node*);
	bool haveVisited(Node*);

	void readNodes();
	void getLinksAndWeights();
	void getStartAndDest();
	void dijkstra();
};