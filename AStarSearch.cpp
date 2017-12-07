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

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Window.H>




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
	vector<Node> getClosedList(){
		return closedList;
	}
	void findPath(int length) {
		int tempCounter = 0;
		Node current;
		current = start;
		cout << "\n\n====================\nCURRENT NODE: [" << current.x << "," << current.y << "]" << endl;
		cout << "GOAL NODE: [" << goal.x << "," << goal.y << "]\n====================\n" << endl;
		for (int i = 0; i < length; i++) {
			for (int j = 0; j < length; j++) {
				grid[i][j].h = (abs(i - goal.x) + (abs(j - goal.y)));
			}
		}
		bool found = false;

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
									
									// Default closed list contains bool to false
									bool inClosedList = false;
									
									// Check if potential next node already exists in closedList
									for (vector<Node>::iterator it = closedList.begin(); it != closedList.end(); it++){
										if ((*it).x == grid[current.x + i][current.y + j].x && (*it).y == grid[current.x + i][current.y + j].y){
											inClosedList = true;
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
									}
									
									if (grid[current.x + i][current.y + j].blocked){
	 									cout << "Node [" << grid[current.x + i][current.y + j].x << "," << grid[current.x + i][current.y + j].y << "] is blocked!" << endl;
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
					closedList.push_back(current);
					current = bestChoice;
				}
			}			
		}
		cout << "End node: [" << current.x << "," << current.y << "]" << endl;
	}
};

int main(int argc, char **argv)
{
	// Set up grid for boxes
	int length = 25;
	Node **grid = new Node*[length];
	for (int i = 0; i < length; i++){
		grid[i] = new Node[length];
	}
	for (int i = 0; i < length; i++) {
		for (int j = 0; j < length; j++) {
			grid[i][j].x = i;
			grid[i][j].y = j;
		}
	}

	// Read user values
	int startX, startY, goalX, goalY;
	cout << "Enter a start node X value (0-" << length-1 << "): ";
	cin >> startX;
	cout << "\nEnter a start node Y value (0-" << length-1 << "): ";
	cin >> startY;
	cout << "\nEnter a goal node X value (0-" << length-1 << "): ";
	cin >> goalX;
	cout << "\nEnter a goal node Y value (0-" << length-1 << "): ";
	cin >> goalY;


	// Set values and start search
	grid[startX][startY].start = true;
	grid[goalX][goalY].goal = true;
	
	
// TEMPORARY: Manually setting blocked nodes for testing
	grid[3][3].blocked = true;
		grid[3][4].blocked = true;
			grid[3][5].blocked = true;
				grid[3][6].blocked = true;
	grid[4][3].blocked = true;
		grid[4][4].blocked = true;
			grid[4][5].blocked = true;
				grid[4][6].blocked = true;
	grid[5][3].blocked = true;
		grid[5][4].blocked = true;
			grid[5][5].blocked = true;
				grid[5][6].blocked = true;
	grid[6][3].blocked = true;
		grid[6][4].blocked = true;
			grid[6][5].blocked = true;
				grid[6][6].blocked = true;
	grid[10][15].blocked = true;
		grid[10][16].blocked = true;
			grid[10][17].blocked = true;
				grid[10][18].blocked = true;
	grid[11][15].blocked = true;
		grid[11][16].blocked = true;
			grid[11][17].blocked = true;
				grid[11][18].blocked = true;
	grid[12][15].blocked = true;
		grid[12][16].blocked = true;
			grid[12][17].blocked = true;
				grid[12][18].blocked = true;
	grid[13][15].blocked = true;
		grid[13][16].blocked = true;
			grid[13][17].blocked = true;
				grid[13][18].blocked = true;
	grid[14][15].blocked = true;
		grid[14][16].blocked = true;
			grid[14][17].blocked = true;
				grid[14][18].blocked = true;
					grid[19][19].blocked = true;
										grid[19][20].blocked = true;
															grid[19][21].blocked = true;
					grid[20][19].blocked = true;
										
															grid[20][21].blocked = true;
					grid[21][19].blocked = true;
										grid[21][20].blocked = true;
															grid[21][21].blocked = true;
				
				
	Search route(grid[startX][startY], grid[goalX][goalY], grid);
	
	route.findPath(length);
	
	// Get path
	vector<Node> closedList = route.getClosedList();
	
	// Define color constants
	Fl_Color fl_BG_COLOR = fl_rgb_color(242,242,245);
	Fl_Color fl_PATH = fl_rgb_color(255,0,0);
	Fl_Color fl_START = fl_rgb_color(0,255,255);
	Fl_Color fl_GOAL = fl_rgb_color(0,255,0);
	Fl_Color fl_BLOCKED = fl_rgb_color(100,100,100);
		
	// Create window and apply properties
	Fl_Window *window = new Fl_Window(1000,800);
	window -> color(fl_BG_COLOR);
	
	//Create box for instructions and set values
	Fl_Box *box = new Fl_Box(750,-300,250,1300,"1.Click to set a start point\n2.Right click to set a goal point\n3.Click again to set obstacles\n4.Press space to start the search.\n\nCyan is the start\nGreen is the goal\nRed is the path\nGray indicates an obstacle");
	box->box(FL_UP_BOX);
	box->labelsize(12);
	
	
	for (int i = 0; i < length; i++){
		for (int j = 0; j < length; j++){
			Fl_Box *box = new Fl_Box(30*i, 30*j, 30, 30);
			box -> box(FL_BORDER_BOX);
			
			for (int p = 0; p < closedList.size(); p++){
				if (i == closedList[p].x && j == closedList[p].y){
					box -> color(fl_PATH);
				}
				if (i == startX && j == startY){
					box -> color(fl_START);
				}
				if (i == goalX && j == goalY){
					box -> color(fl_GOAL);
				}
				if (grid[i][j].blocked){
					box -> color(fl_BLOCKED);
				}
			}

		}
	}
	
	window->end();
	window->show(argc, argv);
	Fl::run();
	return 0;


}
