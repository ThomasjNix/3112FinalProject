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
		void setBlockedNodes(){
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

class GuiWindow : public Fl_Window{
	public: 
		GuiWindow(int w, int h, const char* title):Fl_Window(w,h,title){
			begin();

			Fl_Box *box = new Fl_Box(750,-200,250,1300,"1.Enter Start and Goal X and Y Values\n2.Click Start\n3.Path will be drawn!");
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
			startBtn -> callback(btn_cb, this);	
			Fl_Box **box_grid = new Fl_Box*[25];

			for (int i = 0; i < 25; i++){
				for (int j = 0; j < 25; j++){
		
					Fl_Box *box = new Fl_Box(30*i, 30*j, 30, 30);
					box -> box(FL_BORDER_BOX);
				}
			}
			
			end();
			show();
		}
		static void btn_cb(Fl_Widget *widget, void*w){
			GuiWindow *gw = (GuiWindow*)w;
				gw->make_current();
				
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
				newGrid.setBlockedNodes();
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
					Fl_Box *box = new Fl_Box(30*i, 30*j, 30, 30);
					box -> box(FL_BORDER_BOX);

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
						if (grid[i][j].blocked){
							box -> color(fl_BLOCKED);
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
