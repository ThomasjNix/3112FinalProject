/*
	 AStarSearchProject.cpp : Defines the entry point for the console application.
	 Authors: Thomas Nix, David Pushard, Elenimarys Henriquez
	 ITCS 3112, 
	 Professor Kalpathi Subramanian
*/


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

		cout << "GOAL NODE: [" << goal.x << "," << goal.y << "]" << endl;
		for (int i = 0; i < length; i++) {
			for (int j = 0; j < length; j++) {
				grid[i][j].h = (abs(i - goal.x) + (abs(j - goal.y)));
			}
		}
		bool found = false;
		current = start;
		cout << "CURRENT NODE: [" << current.x << "," << current.y << "]" << endl;
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
				cout << "Search Completed" << endl;
				found = true;
			}
			else {
				cout << "Current node: [" << current.x << "," << current.y << "]" << endl;
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
									}else if(current.x == length-1){
										i = (-1)*abs(k);
										j = abs(l);
									}else{
										i = k;
										j = abs(l);
									}
								}else if (current.y == length-1){
									if (current.x == 0){
										i = abs(k);
										j = (-1)*abs(l);
									}else if(current.x == length-1){
										i = (-1)*abs(k);
										j = (-1)*abs(l);
									}else{
										i = k;
										j = (-1)*abs(l);
									}
								}else if (current.x == 0){
									i = abs(k);
									j = l;
								}else if (current.x == length-1){
									i = (-1)*abs(k);
									j = l;
								}else{
									i = k;
									j = l;
								}								

									bool inClosedList = false;
									
									// Check if potential next node already exists in closedList
									for (vector<Node>::iterator it = closedList.begin(); it != closedList.end(); it++){
										if ((*it).x == grid[current.x + i][current.y + j].x && (*it).y == grid[current.x + i][current.y + j].y){
											inClosedList = true;
											cout << "Node [" << (*it).x << "," << (*it).y << "] is in the closed loop!" << endl;
										}
									}
									
									/*
									 If grid[current.x + i][current.y + j] is NOT blocked and NOT in closed list, push to open list
									 and set the node it represents in the grid to have a parent of the 
									 current node, then set g and f values.
									*/
									if	(!grid[current.x + i][current.y + j].blocked && !inClosedList){
																			
										grid[current.x + i][current.y + j].parent = &current;
										if (abs(i)==1 && abs(j)==1) {
											grid[current.x + i][current.y + j].g = 10 + grid[current.x][current.y].g;
										}
										else {
											grid[current.x + i][current.y + j].g = 10 + grid[current.x][current.y].g;
										}
										grid[current.x + i][current.y + j].f = grid[current.x + i][current.y + j].g + grid[current.x + i][current.y + j].h;	
										openList.push_back(grid[current.x + i][current.y + j]);

									}else{
	 									cout << "Node [" << grid[current.x + i][current.y + j].x << "," << grid[current.x + i][current.y + j].y << "] is either blocked or in the closed loop." << endl;
									}

						}
					}
				}
				if (openList.empty()){
								found = true;
				}
				if (!found){
					Node bestChoice = openList.at(0);
					for (int li = 0; li < openList.size(); li++) {
						if (openList[li].f < bestChoice.f) {
							bestChoice = openList[li];
						}
					}
					current = bestChoice;
				}
			}			
		}
		cout << "Current node: [" << current.x << "," << current.y << "]" << endl;
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
