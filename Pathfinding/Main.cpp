#include "Graph.h"

int main(int argc, char* argv[]) {

	Graph main;

	main.readNodes();
	main.getLinksAndWeights();

	std::cout << main;
	main.getStartAndDest();

	main.dijkstra();

	return 1;
};