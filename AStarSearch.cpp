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
	int f = 0;
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
	vector<Node> openList;
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
		Node current;

		for (int i = 0; i < length; i++) {
			for (int j = 0; j < length; j++) {
				grid[i][j].h = (abs(goal.x - i) + (abs(goal.y - j)));
			}
		}
		bool found = false;
		current = start;


		while (!found) {
			if (current.x == goal.x && current.y == goal.y) {
				cout << "GOT HERE" << endl;
				found = true;
			}
			else {

				closedList.push_back(current);

				for (int k = -1; k <= 1; k++) {
					for (int l = -1; l <= 1; l++) {
						if (k != 0 && l != 0){
							int i, j;
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
						
								//cout << "i:\t" << i << "\nj:\t" << j << endl;
								//cout << "=====currentX:\t" << current.x << "\n=====currentY:\t" << current.y << endl;
								for (int i = 0; i < 8; i++){
									//cout << "test " << i << endl;
									Node gridAtXiYj = grid[current.x + i][current.y + j];
							
									// Check if gridAtXiYj
									if	(!gridAtXiYj.blocked){
										openList.push_back(grid[current.x + i][current.y + j]);
										grid[current.x + i][current.y + j].parent = &current;
										if ((i == 1 || i == -1) && (j == 1 || j == -1)) {
											grid[current.x + i][current.y + j].g = 14 + grid[current.x][current.y].g;
										}
										else {
											grid[current.x + i][current.y + j].g = 10 + grid[current.x][current.y].g;
										}

										grid[current.x + i][current.y + j].f = grid[current.x + i][current.y + j].g + grid[current.x][current.y = j].h;	
									}
								}
						}
						
					}
				}

				Node bestChoice = openList[0];

				for (int li = 0; li < (sizeof(openList)/sizeof(openList[0])); li++) {
					if (openList[li].f < bestChoice.f) {
						bestChoice = openList[li];
					}
				}
				
				current = bestChoice;
				
			}
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
