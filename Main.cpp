#include <iostream>
#include <string>
#include <tuple>
#include <stack>



#include "API.h"
using namespace std;


//CHANGEABLE PROPERTIES
int maze_width=API::mazeWidth();
int maze_height=API::mazeHeight();
int orient=0; //starting orient
//CHANGEABLE PROPERTIES

//troubleshooting aid (functions to print text to simulator console)
void log(const string& text) {
    cerr << text << endl;
}

void log(int var) {
    cerr << var << endl;
}

void log(char dir) {
    cerr << dir << endl;
}


//initialize variables
int x;
int y;
int xprev;
int yprev;


//array that stores wall information
int cells[16][16] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

//array that stores flood (distance) information; 0's indicate the goal location
int flood[16][16]={
    {14,13,12,11,10,9,8,7,7,8,9,10,11,12,13,14},
        {13,12,11,10,9,8,7,6,6,7,8,9,10,11,12,13},
        {12,11,10,9,8,7,6,5,5,6,7,8,9,10,11,12},
        {11,10,9,8,7,6,5,4,4,5,6,7,8,9,10,11},
        {10,9,8,7,6,5,4,3,3,4,5,6,7,8,9,10},
        {9,8,7,6,5,4,3,2,2,3,4,5,6,7,8,9},
        {8,7,6,5,4,3,2,1,1,2,3,4,5,6,7,8},
        {7,6,5,4,3,2,1,0,0,1,2,3,4,5,6,7},
        {7,6,5,4,3,2,1,0,0,1,2,3,4,5,6,7},
        {8,7,6,5,4,3,2,1,1,2,3,4,5,6,7,8},
        {9,8,7,6,5,4,3,2,2,3,4,5,6,7,8,9},
        {10,9,8,7,6,5,4,3,3,4,5,6,7,8,9,10},
        {11,10,9,8,7,6,5,4,4,5,6,7,8,9,10,11},
        {12,11,10,9,8,7,6,5,5,6,7,8,9,10,11,12},
        {13,12,11,10,9,8,7,6,6,7,8,9,10,11,12,13},
        {14,13,12,11,10,9,8,7,7,8,9,10,11,12,13,14}
};

//another troubleshooting aid, shows numbers in simulator grid
void showFlood() {
    for (int x = 0; x < 16; ++x) {
        for (int y = 0; y < 16; ++y) {
            API::setText(x, y, to_string(flood[y][x]));
        }
    }
}

//function that updates the wall information of 'cell' array
void updateWalls(int x, int y, int orient, bool L, bool R, bool F) {
    if (L && R && F) {
        if (orient == 0) {
            cells[y][x] = 13;
        } else if (orient == 1) {
            cells[y][x] = 12;
        } else if (orient == 2) {
            cells[y][x] = 11;
        } else if (orient == 3) {
            cells[y][x] = 14;
        }
    } else if (L && R && !F) {
        if (orient == 0 || orient == 2) {
            cells[y][x] = 9;
        } else if (orient == 1 || orient == 3) {
            cells[y][x] = 10;
        }
    } else if (L && F && !R) {
        if (orient == 0) {
            cells[y][x] = 8;
        } else if (orient == 1) {
            cells[y][x] = 7;
        } else if (orient == 2) {
            cells[y][x] = 6;
        } else if (orient == 3) {
            cells[y][x] = 5;
        }
    } else if (R && F && !L) {
        if (orient == 0) {
            cells[y][x] = 7;
        } else if (orient == 1) {
            cells[y][x] = 6;
        } else if (orient == 2) {
            cells[y][x] = 5;
        } else if (orient == 3) {
            cells[y][x] = 8;
        }
    } else if (F) {
        if (orient == 0) {
            cells[y][x] = 2;
        } else if (orient == 1) {
            cells[y][x] = 3;
        } else if (orient == 2) {
            cells[y][x] = 4;
        } else if (orient == 3) {
            cells[y][x] = 1;
        }
    } else if (L) {
        if (orient == 0) {
            cells[y][x] = 1;
        } else if (orient == 1) {
            cells[y][x] = 2;
        } else if (orient == 2) {
            cells[y][x] = 3;
        } else if (orient == 3) {
            cells[y][x] = 4;
        }
    } else if (R) {
        if (orient == 0) {
            cells[y][x] = 3;
        } else if (orient == 1) {
            cells[y][x] = 4;
        } else if (orient == 2) {
            cells[y][x] = 1;
        } else if (orient == 3) {
            cells[y][x] = 2;
        }
    } else if (!L && !R && !F) {
        cells[y][x]=15;
    }
}

//function that returns true/false based on whether the mouse can move from one cell to an adjacent cell
bool isAccessible(int x, int y, int x1, int y1) {
    // Returns true if the mouse can move from (x, y) to (x1, y1) (two adjacent cells)
    if (cells[y][x]==15){
        return true;
        
    } else if (x == x1) {
        if (y > y1) {
            // Moving down
            if (cells[y][x] == 4 || cells[y][x] == 5 || cells[y][x] == 6 || 
                cells[y][x] == 10 || cells[y][x] == 11 || cells[y][x] == 12 || 
                cells[y][x] == 14) {
                return false;
            } else {
                return true;
            }
        } else {
            // Moving up
            if (cells[y][x] == 2 || cells[y][x] == 7 || cells[y][x] == 8 || 
                cells[y][x] == 10 || cells[y][x] == 12 || cells[y][x] == 13 || 
                cells[y][x] == 14) {
                return false;
            } else {
                return true;
            }
        }
    } else if (y == y1) {
        if (x > x1) {
            // Moving left
            if (cells[y][x] == 1 || cells[y][x] == 5 || cells[y][x] == 8 || 
                cells[y][x] == 9 || cells[y][x] == 11 || cells[y][x] == 13 || 
                cells[y][x] == 14) {
                return false;
            } else {
                return true;
            }
        } else {
            // Moving right
            if (cells[y][x] == 3 || cells[y][x] == 6 || cells[y][x] == 7 || 
                cells[y][x] == 9 || cells[y][x] == 11 || cells[y][x] == 12 || 
                cells[y][x] == 13) {
                return false;
            } else {
                return true;
            }
        }
    }

    // If neither x nor y coordinates are the same, the cells are not adjacent
    return false;
}

//function that returns the coordinates of the four surrounding adjacent cells
tuple<int, int, int, int, int, int, int, int> getSurrounds(int x, int y) {
    // Returns the coordinates of the four surrounding squares
    int x0 = x;
    int y0 = y + 1;  // North
    int x1 = x + 1;
    int y1 = y;      // East
    int x2 = x;
    int y2 = y - 1;  // South
    int x3 = x - 1;
    int y3 = y;      // West

    if (x1 >= 16) x1 = -1;
    if (y0 >= 16) y0 = -1;

    return make_tuple(x0, y0, x1, y1, x2, y2, x3, y3);
}

//function that is part of the main floodfill mechanism (the 'checking' part)
bool isConsistent(int x, int y) {
    // Returns true if the value of the current square is one greater than the minimum value in an accessible neighbor
    int x0;
    int y0;
    int x1;
    int y1;
    int x2;
    int y2;
    int x3;
    int y3;
    
    tie(x0, y0, x1, y1, x2, y2, x3, y3) = getSurrounds(x, y);
    int val = flood[y][x];
    int minVals[4] = {-1, -1, -1, -1};

    if (x0 >= 0 && y0 >= 0 && isAccessible(x, y, x0, y0)) {
        minVals[0] = flood[y0][x0];
    }
    if (x1 >= 0 && y1 >= 0 && isAccessible(x, y, x1, y1)) {
        minVals[1] = flood[y1][x1];
    }
    if (x2 >= 0 && y2 >= 0 && isAccessible(x, y, x2, y2)) {
        minVals[2] = flood[y2][x2];
    }
    if (x3 >= 0 && y3 >= 0 && isAccessible(x, y, x3, y3)) {
        minVals[3] = flood[y3][x3];
    }

    int minCount = 0;
    for (int i = 0; i < 4; ++i) {
        if (minVals[i] == -1) {
            continue;
        } else if (minVals[i] == val + 1) {
            continue;
        } else if (minVals[i] == val - 1) {
            ++minCount;
        }
    }

    return (minCount > 0);
}

//function that is part of the main floodfill mechanism (the 'updating/correcting' part)
void makeConsistent(int x, int y) {
    int x0;
    int y0;
    int x1;
    int y1;
    int x2;
    int y2;
    int x3;
    int y3;

    tie (x0, y0, x1, y1, x2, y2, x3, y3) = getSurrounds(x, y);
    int val = flood[y][x];
    int minVals[4] = {-1, -1, -1, -1};

    if (x0 >= 0 && y0 >= 0 && isAccessible(x, y, x0, y0)) {
        minVals[0] = flood[y0][x0];
    }
    if (x1 >= 0 && y1 >= 0 && isAccessible(x, y, x1, y1)) {
        minVals[1] = flood[y1][x1];
    }
    if (x2 >= 0 && y2 >= 0 && isAccessible(x, y, x2, y2)) {
        minVals[2] = flood[y2][x2];
    }
    if (x3 >= 0 && y3 >= 0 && isAccessible(x, y, x3, y3)) {
        minVals[3] = flood[y3][x3];
    }

    for (int i = 0; i < 4; ++i) {
        if (minVals[i] == -1) {
            minVals[i] = 1000;
        }
    }

    int minVal = *min_element(minVals, minVals + 4);
    flood[y][x] = minVal + 1;
}

//Main floodfill mechanism (the 'stacking and executing' part) that makes use of the previous 2 functions 
void floodFill(int x, int y, int xprev, int yprev) {
    
    if (!isConsistent(x, y)&&flood[y][x] != 0) {
        flood[y][x] = flood[yprev][xprev] + 1;
    }
    
    stack<int> stack;
    stack.push(x);
    stack.push(y);
    
    int x0;
    int y0;
    int x1;
    int y1;
    int x2;
    int y2;
    int x3;
    int y3;

    tie (x0, y0, x1, y1, x2, y2, x3, y3) = getSurrounds(x, y);
    if (x0 >= 0 && y0 >= 0 && isAccessible(x, y, x0, y0)) {
        stack.push(x0);
        stack.push(y0);
    }
    if (x1 >= 0 && y1 >= 0 && isAccessible(x, y, x1, y1)) {
        stack.push(x1);
        stack.push(y1);
    }
    if (x2 >= 0 && y2 >= 0 && isAccessible(x, y, x2, y2)) {
        stack.push(x2);
        stack.push(y2);
    }
    if (x3 >= 0 && y3 >= 0 && isAccessible(x, y, x3, y3)) {
        stack.push(x3);
        stack.push(y3);
    }

    while (!stack.empty()) {
        int yrun = stack.top(); stack.pop();
        int xrun = stack.top(); stack.pop();

        if (isConsistent(xrun, yrun) || flood[yrun][xrun] == 0) {
            continue;
        } else {
            makeConsistent(xrun, yrun);
            stack.push(xrun);
            stack.push(yrun);

            tie(x0, y0, x1, y1, x2, y2, x3, y3) = getSurrounds(xrun, yrun);
            if (x0 >= 0 && y0 >= 0 && isAccessible(xrun, yrun, x0, y0)) {
                stack.push(x0);
                stack.push(y0);
            }
            if (x1 >= 0 && y1 >= 0 && isAccessible(xrun, yrun, x1, y1)) {
                stack.push(x1);
                stack.push(y1);
            }
            if (x2 >= 0 && y2 >= 0 && isAccessible(xrun, yrun, x2, y2)) {
                stack.push(x2);
                stack.push(y2);
            }
            if (x3 >= 0 && y3 >= 0 && isAccessible(xrun, yrun, x3, y3)) {
                stack.push(x3);
                stack.push(y3);
            }
        }
    }

    
}

//function that returns where the mouse should go based on the flood values of the surrounding cells
char toMove(int x, int y, int orient,int xprev, int yprev) {
    int x0;
    int y0;
    int x1;
    int y1;
    int x2;
    int y2;
    int x3;
    int y3;

    int xfront;
    int yfront;
    int xright;
    int yright;
    int xleft;
    int yleft;

    tie (x0, y0, x1, y1, x2, y2, x3, y3) = getSurrounds(x, y);
    int val = flood[y][x];

    

    if (orient==0){
        xfront=x0;
        yfront=y0;
        xright=x1;
        yright=y1;
        xleft=x3;
        yleft=y3;
    }

    else if (orient==1){
        xfront=x1;
        yfront=y1;
        xright=x2;
        yright=y2;
        xleft=x0;
        yleft=y0;
    }

    else if (orient==2){
        xfront=x2;
        yfront=y2;
        xright=x3;
        yright=y3;
        xleft=x1;
        yleft=y1;
    }

    else if (orient==3){
        xfront=x3;
        yfront=y3;
        xright=x0;
        yright=y0;
        xleft=x2;
        yleft=y2;
    }

    //PRIORITY = FORWARD > RIGHT > LEFT > BACK

    if (xfront >= 0 && yfront >= 0 && isAccessible(x, y, xfront, yfront)&& flood[yfront][xfront] == val - 1) {
        return 'F';
    } else if (xright >= 0 && yright >= 0 && isAccessible(x, y, xright, yright) && flood[yright][xright] == val - 1) { 
        return 'R';

    } else if (xleft >= 0 && yleft >= 0  && isAccessible(x, y, xleft, yleft) && flood[yleft][xleft] == val - 1) {
        return 'L';

    } else {
        return 'B'; //turn back if mouse runs into a dead end
        
    }
}

//function that updates the coordinates of the mouse
void updateCoordinates(){
    if(orient == 0){y += 1;}
    else if(orient == 1){x += 1;}
    else if(orient == 2){y -= 1;}
    else if(orient == 3){x -= 1;}
}

//function that updates the orientation of the mouse
void updateOrient(char turning){
    if (turning == 'L'){
        orient -= 1;
        if (orient == -1){orient = 3;}
    }
    else if(turning == 'R'){
        orient += 1;
        if (orient == 4){orient=0;}
    }
    else if(turning== 'B'){
        if (orient==0){orient=2;}
        else if (orient==1){orient=3;}
        else if (orient==2){orient=0;}
        else if (orient==3){orient=1;}
    }
}

//yet another troubleshooting aid; shows the discovered walls in the simulator UI
void put_walls(int orient,bool w_left,bool w_right,bool w_front){
    if (orient==0){
            if(w_front){
                API::setWall(x, y, 'n');

            }
            if(w_left){
                API::setWall(x, y, 'w');

            }
            if(w_right){
                API::setWall(x, y, 'e');

    }}

    else if (orient==1){
        if(w_front){
            API::setWall(x, y, 'e');    
        }
        if(w_left){
            API::setWall(x, y, 'n');    
        }
        if(w_right){
            API::setWall(x, y, 's');    
    }}

    else if (orient==2){
        if(w_front){
            API::setWall(x, y, 's');    
        }
        if(w_left){
            API::setWall(x, y, 'e');    
        }
        if(w_right){
            API::setWall(x, y, 'w');    
    }}

    else if (orient==3){
        if(w_front){
            API::setWall(x, y, 'w');    
        }
        if(w_left){
            API::setWall(x, y, 's');    
        }
        if(w_right){
            API::setWall(x, y, 'n');    
    }}
}

int main(int argc, char* argv[]) {
    
    log("Running...");
    showFlood();
    API::setColor(0, 0, 'G');
    API::setText(0, 0, "starting");
    
    while (flood[y][x]!=0){

        int w_left = API::wallLeft();
        int w_right = API::wallRight();
        int w_front = API::wallFront();

        
        //OVERALL MECHANISM: update walls > perform floodfill > move > update orient > update coordinates > repeat until goal is reached
        

        updateWalls(x, y, orient, w_left, w_right, w_front);

        //put walls (for debug purposes)
        put_walls(orient,w_left,w_right,w_front);

        floodFill(x,y,xprev,yprev);

        showFlood();

        //decide where to move
        char direction=toMove(x,y,orient,xprev,yprev);

        
        xprev=x;
        yprev=y;
        if(direction == 'F'){
            API::moveForward();
        }
        else if(direction == 'R'){
            API::turnRight();
            updateOrient('R');
            API::moveForward();
        }
        else if(direction == 'L'){
            API::turnLeft();
            updateOrient('L');
            API::moveForward();
        }
        else if(direction == 'B'){
            API::turnRight();
            updateOrient('R');
            API::turnRight();
            updateOrient('R');
            API::moveForward();
        }
        updateCoordinates();
        if (flood[y][x]==0){
            log("goal reached :DDD");
        }
        

    }
        

}

//congrats! you've reached the end of the code! now, what you can do is think of ways to improve the code by adding additional mechanisms or functions
//OR even better, try to implement your own maze-solving algorithm! good luck! :D
