// AStarSearchProject.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <algorithm>
#include <time.h>
#include <stdlib.h>
#include <cmath>
#include <vector>
#include <iterator> 

using namespace std;

struct Node {
	int x = 0;
	int y = 0;
	int f = 10000000;
	int g = 0;
	int h = 0;
	bool start = false;
	bool goal = false;
	bool blocked = false;
	Node *parent;

};


class Search {
private:
	Node start;
	Node goal;
	Node **grid;
	vector<Node> closedList;
public:
	Search(Node s, Node g, Node **m) {
		start = s;
		goal = g;
		grid = m;
	}

	Search(Node **m) {
		grid = m;
	}

	void setStart(Node s) {
		start = s;
	}

	Node getStart() {
		return start;
	}

	void setGoal(Node g) {
		goal = g;
	}

	Node getGoal() {
		return goal;
	}

	void findPath(int length) {
		int tempCounter = 0;
		Node current;
		for (int i = 0; i < length; i++) {
			for (int j = 0; j < length; j++) {
				grid[i][j].h = (abs(i - goal.x) + (abs(j - goal.y)));
			}
		}
		bool found = false;
		current = start;
		cout << "GRID:\n[";
		for (int i = 0; i < length; i++) {
			for (int j = 0; j < length; j++) {
				cout << " {";
				cout << grid[i][j].x;
				cout << ", " << grid[i][j].y;
				cout << "} ";
			}
		}
		cout << " ]";
		cout << "]" << endl;
		//iterates at each step of path
		while (!found) {
			if (current.x == goal.x && current.y == goal.y) {
				cout << "GOT HERE" << endl;
				found = true;
			}
			else {
				vector<Node> openList;
				// Loop through all possible +/- 1 (surrounding node) combinations, adjust for edge cases if necessary
				for (int k = -1; k <= 1; k++) {
					for (int l = -1; l <= 1; l++) {
						int i, j;
						if (!(k == 0 && l == 0)){
								if (current.y == 0){
									if (current.x == 0){
										i = abs(k);
										j = abs(l);
									}else if(current.x == length){
										i = (-1)*abs(k);
										j = abs(l);
									}else{
										i = k;
										j = abs(l);
									}
								}else if (current.y == length){
									if (current.x == 0){
										i = abs(k);
										j = (-1)*abs(l);
									}else if(current.x == length){
										i = (-1)*abs(k);
										j = (-1)*abs(l);
									}else{
										i = k;
										j = (-1)*abs(l);
									}
								}else if (current.x == 0){
									i = abs(k);
									j = l;
								}else if (current.x == length){
									i = (-1)*abs(k);
									j = l;
								}else{
									i = k;
									j = l;
								}
								// JUST FOR TESTING
								/*
								if (tempCounter < 10){
									cout << "\n======================\nCurrent Closed list at iteration:\n\tX: " << k << "\tY: " << l << endl;
									cout << "[";
									for (vector<Node>::iterator i = closedList.begin(); i != closedList.end(); i++){
										cout << " {" << (*i).x << "," << (*i).y << "} ";
									}
									cout << "]" << endl;
									cout << "(Current Node: [" << current.x << "," << current.y << "]" << endl;
								}
								*/
								
								//for (int i = 0; i < 8; i++){
									//cout << "test " << i << endl;
									Node gridAtXiYj = grid[current.x + i][current.y + j];
									bool inClosedList = false;
									for (vector<Node>::iterator it = closedList.begin(); it != closedList.end(); it++){
										if ((*it).x == (current.x + i) && (*it).y == (current.y + j)){
											inClosedList = true;
										}
									}
									// Check if gridAtXiYj
									if	(!gridAtXiYj.blocked && !inClosedList){
										openList.push_back(grid[current.x + i][current.y + j]);
										//cout << "JUST PUSHED A BIG ONE" << endl;
										grid[current.x + i][current.y + j].parent = &current;
										if (i != 0 && j != 0) {
											grid[current.x + i][current.y + j].g = 14 + grid[current.x][current.y].g;
										}
										else {
											grid[current.x + i][current.y + j].g = 10 + grid[current.x][current.y].g;
										}
										
									}
									grid[current.x + i][current.y + j].f = grid[current.x + i][current.y + j].g + grid[current.x + i][current.y + j].h;	
								//}
								// JUST FOR TESTING
								/*
								if (tempCounter < 10){
									cout << "\nCurrent Open list at iteration:\n\tX: " << k << "\tY: " << l << endl;
									cout << "[";
									for (vector<Node>::iterator i = openList.begin(); i != openList.end(); i++){
										cout << " {" << (*i).x << "," << (*i).y << "} ";
									}
									cout << "]\n======================\n" << endl;
								}
								*/
								
						}
					}
				}
				Node bestChoice = openList.at(0);
				cout << "Current Node (BC):\n\tX: " << current.x << "\n\t" << current.y << endl;
				//cout << openList.size();
				
				
				for (int li = 0; li < openList.size(); li++) {
				//cout << "Comparison: \nBCf: " << bestChoice.f << "\nBCx: " << bestChoice.x << "\nBCy: " << bestChoice.y << endl;
					//			cout << "OLf: " << openList[li].f << "\nOLx: " << openList[li].x << "\nOLy: " << openList[li].y << endl;
					if (openList[li].f < bestChoice.f) {
						cout << "LI was smaller!" << endl;
						bestChoice = openList[li];
					}
				}
				
				current = bestChoice;
				cout << "Current Node (AC):\n\tX: " << current.x << "\n\t" << current.y << endl;
			}
			closedList.push_back(current);
			tempCounter++;
		}
			cout << "test" << endl;
		for (vector<Node>::iterator it = closedList.begin(); it != closedList.end(); it++){
					cout << "\nNodex:\t" << (*it).x << "\nNodeY:\t" << (*it).y << endl; 
		}
	}
};

int main()
{
	cout << "Enter the length of one side of a square map: " << endl;
	int length;
	cin >> length;
	Node **grid = new Node*[length];
	for (int i = 0; i < length; i++) {
		grid[i] = new Node[length];
	}

	for (int i = 0; i < length; i++) {
		for (int j = 0; j < length; j++) {
			grid[i][j].x = i;
			grid[i][j].y = j;
		}
	}

	int startX, startY, goalX, goalY;

	cout << "Enter a starting Node x and then y position: " << endl;
	cin >> startX >> startY;

	cout << "Enter a goal Node x and then Y position: " << endl;
	cin >> goalX >> goalY;


	grid[startX][startY].start = true;
	grid[goalX][goalY].goal = true;

	Search route(grid[startX][startY], grid[goalX][goalY], grid);
	route.findPath(length);

	return 0;
}
