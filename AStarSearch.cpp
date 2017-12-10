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
#include <sstream>

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Window.H>

using namespace std;


// Define color constants - These values will not change and need to be accessed from multiple sources
Fl_Color fl_PATH = fl_rgb_color(255,0,0);
Fl_Color fl_START = fl_rgb_color(0,255,255);
Fl_Color fl_GOAL = fl_rgb_color(0,255,0);
Fl_Color fl_BLOCKED = fl_rgb_color(100,100,100);
Fl_Int_Input *istartX;
Fl_Int_Input *istartY;
Fl_Int_Input *igoalX;
Fl_Int_Input *igoalY;



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

vector<Node> blockedNodes;

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


class GridDraw{
	private:
		Node **grid;		
		vector<Node> closedList;
		int startX, startY, goalX, goalY, length;
	public: 
		GridDraw(){
				length = 25;
			grid = new Node*[length];
			for (int i = 0; i < length; i++){
				grid[i] = new Node[length];
			}
			for (int i = 0; i < length; i++) {
				for (int j = 0; j < length; j++) {
					grid[i][j].x = i;
					grid[i][j].y = j;
				}
			}
		}	
		void setBlockedNodes(int x, int y){
			grid[x][y].blocked = true;
		}
		void clearBlockedNodes(){
			for (int i = 0; i < length; i++){
				for (int j = 0; j < length; j++){
					grid[i][j].blocked = false;
				}
			}
		}
		void drawPath(){
				Search route(grid[startX][startY], grid[goalX][goalY], grid);
				route.findPath(length);		
				closedList = route.getClosedList();
		}
		void setStartNode(int sX, int sY){
			//To be replaced with user entered values
			startX = sX;
			startY = sY;
		}
		void setEndNode(int gX, int gY)	{
			//To be replaced with user entered values
			goalX = gX;
			goalY = gY;
		}
		vector<Node> getClosedList(){
			return closedList;
		}
		Node getFinalGrid(int i, int j){
			return grid[i][j];
		}
	
};



class CustomBox : public Fl_Box{
	public:
		bool isBlocked = false;
		int gridXPos, gridYPos;
		CustomBox(int x, int y, int w, int h) : Fl_Box(FL_BORDER_BOX,x,y,w,h,""){
		}
		int handle(int e){
			if (e == FL_PUSH){
				isBlocked = true;
				Node tempNode;
				tempNode.x = gridXPos;
				tempNode.y = gridYPos;
				blockedNodes.push_back(tempNode);
				color(fl_BLOCKED);
				cout << "Clicked at [" << gridXPos << "," << gridYPos << "]" << endl;
				redraw();
			}
		}
		void setGridPos(int x, int y){
			gridXPos = x;
			gridYPos = y;
		}
};	



class GuiWindow : public Fl_Window{
	public: 
		GuiWindow(int w, int h, const char* title):Fl_Window(w,h,title){
			begin();

			Fl_Box *box = new Fl_Box(750,-200,250,1300,"1.Enter Start and Goal X and Y Values\n2.Click to place obstacles\n3.Click start to draw path\n4.Path will be drawn!\n\n\n\nClear will erase blocked nodes \nand redrawthe path with\n given coordinates");
			box->box(FL_UP_BOX);
			box->labelsize(12);
	
			Fl_Text_Display *lSX = new Fl_Text_Display(850,50,50,25, "Start X (0-24)");
			Fl_Text_Display *lSY = new Fl_Text_Display(850,100,50,25, "Start Y (0-24)");
			Fl_Text_Display *lGX = new Fl_Text_Display(850,150,50,25, "Goal X (0-24)");
			Fl_Text_Display *lGY = new Fl_Text_Display(850,200,50,25, "Start Y (0-24)");

			istartX = new Fl_Int_Input(850, 50,50,25);
			istartY = new Fl_Int_Input(850,100,50,25);
			igoalX = new Fl_Int_Input(850,150,50,25);
			igoalY = new Fl_Int_Input(850,200,50,25);
			
			
			Fl_Button *startBtn = new Fl_Button(850,250,50,25, "Start");
			startBtn -> callback(start_btn_cb, this);	
			
			Fl_Button *clearBtn = new Fl_Button(850,300,50,25, "Clear");
			clearBtn -> callback(clear_btn_cb, this);	
			for (int i = 0; i < 25; i++){
				for (int j = 0; j < 25; j++){
					CustomBox *box = new CustomBox(30*i, 30*j, 30, 30);
					box->setGridPos(i,j);
				}
			}
			
			end();
			show();
		}
		static void start_btn_cb(Fl_Widget *widget, void*w){

			int sX, sY, gX, gY;
			stringstream s1(istartX -> value());
			s1 >> sX;
			stringstream s2(istartY -> value());
			s2 >> sY;
			stringstream s3(igoalX -> value());
			s3 >> gX;
			stringstream s4(igoalY -> value());
			s4 >> gY;

			// Create instance of the grid 
				GridDraw newGrid;
				
				for (int i = 0; i < blockedNodes.size(); i++){
					newGrid.setBlockedNodes(blockedNodes.at(i).x, blockedNodes.at(i).y);
				}

				newGrid.setStartNode(sX,sY);
				newGrid.setEndNode(gX,gY);
				newGrid.drawPath();
				vector<Node> closedList = newGrid.getClosedList();
				Node **grid = new Node*[25];
				for (int i = 0; i < 25; i++){
					grid[i] = new Node[25];
					for (int j = 0; j < 25; j++){
						grid [i][j] = newGrid.getFinalGrid(i,j);
					}
				}

				for (int i = 0; i < 25; i++){
					for (int j = 0; j < 25; j++){
						CustomBox *box = new CustomBox(30*i, 30*j, 30, 30);
						box->setGridPos(i,j);
						for (int p = 0; p < closedList.size(); p++){
							if (i == closedList[p].x && j == closedList[p].y){
								box -> color(fl_PATH);
							}
							if (i == sX && j == sY){
								box -> color(fl_START);
							}
							if (i == gX && j == gY){
								box -> color(fl_GOAL);
							}
							for (int z = 0; z < blockedNodes.size(); z++){
								if (blockedNodes.at(z).x == i && blockedNodes.at(z).y == j){
									box -> color(fl_BLOCKED);
								}
							}
						}
						widget->parent()->add(box);
				}
			}
			((Fl_Window*)widget)->parent()->redraw();
		}
		
		static void clear_btn_cb(Fl_Widget *widget, void*w){
			blockedNodes.clear();
			int sX, sY, gX, gY;
			stringstream s1(istartX -> value());
			s1 >> sX;
			stringstream s2(istartY -> value());
			s2 >> sY;
			stringstream s3(igoalX -> value());
			s3 >> gX;
			stringstream s4(igoalY -> value());
			s4 >> gY;

			// Create instance of the grid 
				GridDraw newGrid;
				
				newGrid.clearBlockedNodes();
				newGrid.setStartNode(sX,sY);
				newGrid.setEndNode(gX,gY);
				newGrid.drawPath();
				vector<Node> closedList = newGrid.getClosedList();
				Node **grid = new Node*[25];
				for (int i = 0; i < 25; i++){
					grid[i] = new Node[25];
					for (int j = 0; j < 25; j++){
						grid [i][j] = newGrid.getFinalGrid(i,j);
					}
				}

				for (int i = 0; i < 25; i++){
					for (int j = 0; j < 25; j++){
						CustomBox *box = new CustomBox(30*i, 30*j, 30, 30);
						box->setGridPos(i,j);
						for (int p = 0; p < closedList.size(); p++){
							if (i == closedList[p].x && j == closedList[p].y){
								box -> color(fl_PATH);
							}
							if (i == sX && j == sY){
								box -> color(fl_START);
							}
							if (i == gX && j == gY){
								box -> color(fl_GOAL);
							}
							for (int z = 0; z < blockedNodes.size(); z++){
								if (blockedNodes.at(z).x == i && blockedNodes.at(z).y == j){
									box -> color(fl_BLOCKED);
								}
							}
						}
						widget->parent()->add(box);
				}
			}
			((Fl_Window*)widget)->parent()->redraw();
			}
			
};


int main(int argc, char **argv){
	GuiWindow gw(1000,800,"AStarSearch");
	return Fl::run();
}
