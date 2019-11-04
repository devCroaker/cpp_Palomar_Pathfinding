#pragma once
#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>

struct point{
	float x;
	float y;
};

class Node
{
private:
	std::string name;
	int id;
	point center;
	point box[2];
	std::vector<Node*> links;
	std::vector<float> weights;

	float total;
	Node* path;

public:
	Node(std::string);
	~Node();

	void setName(std::string);
	void setId(int);
	void setTotal(float);
	void setCenter(point);
	void setBox(point, point);
	void setPath(Node*);
	
	std::string getName();
	int getId();
	float getTotal();
	point getCenter();
	point getBoxBottomRight();	
	point getBoxTopLeft();
	Node* getPath();

	bool checkOverlap(Node*);

	void addLink(float w, Node* node);
	Node* getLink(int i);
	int getTotalLinks();

	float getW(int i);
	void setW(int i, float w);
	
};

