#include "Graph.h"

Graph::Graph()
{
}

Graph::~Graph()
{
	nodeList.clear();
	queue.clear();
	visited.clear();
}

Node Graph::getNode(int i)
{
	return nodeList[i];
}

void Graph::addVisited(Node* n) { visited.push_back(n); }
bool Graph::haveVisited(Node* n)
{
	if (visited.size() < 1) {
		return false;
	}
	else {
		for (size_t i = 0; i < visited.size(); i++) {
			if (visited[i] == n) {
				return true;
			}
		}
	}
	return false;
}

void Graph::addToQueue(Node* n)
{
	if (queue.size() < 1) {
		queue.push_back(n);
	}
	else {

		//check if in queue and remove
		for (size_t idx = 0; idx < queue.size(); idx++) {
			if (queue[idx] == n) {
				queue.erase(queue.begin() + idx);
			}
		}

		// add/re-add at new position
		size_t i = 0;
		while (i < queue.size()) {

			if (queue[i]->getTotal() > n->getTotal()) {
				break;
			}
			i++;
		}

		queue.emplace(queue.begin() + i, n);
	}
}

std::ostream& operator<<(std::ostream &s, Graph&g) {


	std::ios_base::fmtflags f(std::cout.flags());

	s << " ";
	for (int i = 0; i < 84; i++) {
		s << "-";
	}
	s << std::endl;

	s << "| " << std::left << std::setw(2) << "#" << " | " << std::setw(9) << "ID#" << " | " << std::setw(65) << "Building Name" << " |" << std::endl;

	s << "|";
	for (int i = 0; i < 84; i++) {
		s << "-";
	}
	s << "|" << std::endl;

	for (size_t i = 0; i < g.nodeList.size(); i++) {
		s << "| " << std::left << std::setw(2) << i+1 << " | " << std::setw(9) << g.nodeList[i].getTotalLinks() << " | " << std::setw(65) << g.nodeList[i].getName() << " |" << std::endl;
	}

	s << " ";
	for (int i = 0; i < 84; i++) {
		s << "-";
	}
	s << std::endl;

	std::cout.flags(f);

	return s;
}

void Graph::readNodes()
{
	std::ifstream shapeFile("PalomarCollege.txt");
	std::string line;

	while (std::getline(shapeFile, line)) {

		for (int i = 0; i < 3; i++) {
			std::getline(shapeFile, line);
		}

		Node newNode(line);
		std::getline(shapeFile, line);
		newNode.setId(std::stoi(line));
		std::getline(shapeFile, line);

		int pointNum = 0;
		float boxIncrease = (float)0.0001; // Add a bit to get everything connected to at lease one other building
		if (newNode.getId() == 353730753) {
			boxIncrease = (float)0.0005; // Baseball field is to far away from everything
		}
		std::string num = "";
		point pointA, pointB, pointC;
		for (size_t j = 0; j < line.length(); j++) {
			char cur = line.at(j);

			if (cur == ' ') {
				switch (pointNum) {
				case 0:
					pointA.x = std::stof(num) + boxIncrease;
					break;
				case 1:
					pointA.y = std::stof(num) - boxIncrease;
					break;
				case 2:
					pointB.x = std::stof(num) - boxIncrease;
					break;
				default:
					break;
				}
				num = "";
				pointNum++;
			} else if (cur != '-') {
				num = num + cur;
			}
		}
		pointB.y = std::stof(num) + boxIncrease;
		newNode.setBox(pointA, pointB);
		pointC.x = (pointA.x + pointB.x) / 2;
		pointC.y = (pointA.y + pointB.y) / 2;
		newNode.setCenter(pointC);

		std::getline(shapeFile, line);

		int skipLines = std::stoi(line) -1;
		for (int i = 0; i < skipLines; i++) {
			std::getline(shapeFile, line);
		}

		nodeList.push_back(newNode);

	}

}

void Graph::getLinksAndWeights()
{
	for (size_t i = 0; i < nodeList.size(); i++) {
		Node *current = &nodeList[i];

		for (size_t j = 0; j < nodeList.size(); j++) {

			if (i != j) {
				Node testNode = nodeList[j];

				if (current->checkOverlap(&testNode)) {

					float weight;
					weight = std::sqrt(pow((testNode.getCenter().x - current->getCenter().x), (float)2.0) + pow((testNode.getCenter().y - current->getCenter().y), (float)2.0));

					current->addLink(weight, &nodeList[j]);

				}

			}

		}

	}
}

void Graph::getStartAndDest()
{

	int start, end;

	std::cout << std::endl << "Please enter your starting location #:" << std::endl;
	std::cin >> start;
	start--;
	std::cout << std::endl << "Please enter your destination location #:" << std::endl;
	std::cin >> end;
	end--;

	nodeList[start].setTotal(0);
	addToQueue(&nodeList[start]);
	destinationID = nodeList[end].getId();

}

void Graph::dijkstra()
{
	Node *current, *search;
	float pathTotal, searchTotal;
	do {

		current = queue[0];
		pathTotal = current->getTotal();

		for (int i = 0; i < current->getTotalLinks(); i++) {

			search = current->getLink(i);
			if (!haveVisited(search)) {

				searchTotal = pathTotal + current->getW(i);

				if (searchTotal < search->getTotal()) {

					search->setTotal(searchTotal);
					search->setPath(queue[0]);

				}
				addToQueue(current->getLink(i));
			}

		}

		addVisited(queue[0]);
		queue.erase(queue.begin());

	} while (queue.size() > 0 && current->getId() != destinationID);

	if (current->getId() == destinationID) {
		std::vector<Node*> finalPath;
		finalPath.emplace(finalPath.begin(), current);
		while (current->getPath() != NULL) {
			finalPath.emplace(finalPath.begin(), current->getPath());
			current = current->getPath();
		}

		if (finalPath.size() > 1) {
			std::cout << std::endl << "Your Ideal path is:" << std::endl << "Start at " << finalPath[0]->getName() << std::endl;
			for (size_t i = 1; i < finalPath.size(); i++) {
				std::cout << "Proceed to " << finalPath[i]->getName() << std::endl;
			}
			std::cout << "You have reached your desintation of " << finalPath[finalPath.size() - 1]->getName();
		}
		else {
			std::cout << std::endl << "You started at your destination.";
		}
		finalPath.clear();

	}
	else {
		std::cout << std::endl << "Could not reach destination from starting location" << std::endl;
	}

}
