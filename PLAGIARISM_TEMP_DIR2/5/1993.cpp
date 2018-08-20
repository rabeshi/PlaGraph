#include "world.h"

#include <stdexcept>
#include <fstream>
#include <iomanip>


using namespace std;

// Construct the world object.
// Note that this takes in an istream (cin or an fstream), NOT a string as per the pdf
World::World(istream &input)
{
    input>>numRows>>numCols;
    input.ignore();
    char line[numCols];
    while(input.getline(line,256)){
        _world.push_back(line);
    }
    for(int row = 0; row < numRows; ++row){
        auto str = _world[row];
        for(int col = 0; col<numCols; ++col){
            switch (str[col]){
            case 'S':
                start.col = col;
                start.row = row;
                break;
            case 'G':
                goal.col = col;
                goal.row = row;
                break;
            default:break;
            }
        }
    }
    _distance = new int*[numRows];
    for(int row = 0; row < numRows; ++row){
        _distance[row] = new int[numCols];
    }
    _parent = new Cell*[numRows];
    for(int row = 0; row < numRows; ++row){
        _parent[row] = new Cell[numCols];
    }
    _discovered = new bool*[numRows];
    for(int row = 0; row < numRows; ++row){
        _discovered[row] = new bool[numCols];
    }
    for(int row = 0; row<numRows; ++row){
        for(int col = 0; col<numCols; ++col){
            _discovered[row][col] = false;
            _distance[row][col] = -1;
            _parent[row][col] = {-1,-1};
        }
    }
}

World::~World()
{
    /*for(int i = 1; i<numRows; ++i){
        delete [] _discovered[i];
        delete [] _parent[i];
        delete [] _discovered[i];
    }
    delete [] _discovered;
    delete [] _parent;
    delete [] _discovered;*/
}

// 1 Line, return the relevant distance
int &World::distance(int r, int c)
{
    return _distance[r][c];
}

// 1 Line, return the relevant distance
int &World::distance(Cell c)
{
    return  distance(c.row,c.col);
}

// 1 Line, return the relevant parent
Cell &World::parent(int r, int c)
{
    return _parent[r][c];
}

// 1 Line, return the relevant parent
Cell &World::parent(Cell c)
{
    return _parent[c.row][c.col];
}

// 1 Line, return whether the cell is open or a wall.
bool World::isOpen(int r, int c)
{
    if(_world[r][c] == 'x'){
        return false;
    }
    return !_discovered[r][c];
}

// 1 Line, return whether the cell is open or a wall.
bool World::isOpen(Cell c)
{
    return isOpen(c.row,c.col);
}

// 1 Line, return whether the cell has been discovered
bool& World::discovered(int r, int c)
{
    return _discovered[r][c];
}

// 1 Line, return whether the cell has been discovered
bool& World::discovered(Cell c)
{
    return _discovered[c.row][c.col];
}

// Calculate the shortest path as per the algorithm in the pdf
stack<Cell> World::FindPath()
{
    queue<Cell>que;
    stack<Cell>staq;
    distance(start) = 0;
    parent(start) = {-2,-2};
    discovered(start) = true;
    que.push(start);
    while(que.size() && !discovered(goal)){
        const Cell cur = que.front();
        que.pop();
        //UP
        Cell tmp{cur.row -1,cur.col};
        if(isOpen(tmp)){
            distance(tmp) = distance(cur) + 1;
            parent(tmp) = cur;
            discovered(tmp) = true;
            que.push(tmp);
        }
        //DOWN
        tmp.row = cur.row +1;
        tmp.col = cur.col;
        if(isOpen(tmp)){
            distance(tmp) = distance(cur) +1;
            parent(tmp) = cur;
            discovered(tmp) = true;
            que.push(tmp);
        }
        //LEFT
        tmp.row = cur.row;
        tmp.col = cur.col -1;
        if(isOpen(tmp)){
            distance(tmp) = distance(cur) +1;
            discovered(tmp) = true;
            parent(tmp) = cur;
            que.push(tmp);
        }
        //RIGHT
        tmp.row = cur.row;
        tmp.col = cur.col +1;
        if(isOpen(tmp)){
            discovered(tmp) = true;
            distance(tmp) = distance(cur) +1;
            parent(tmp) = cur;
            que.push(tmp);
        }
    }
    if(!discovered(goal)){
        return staq;
    }
    else{
        Cell cur = goal;
        staq.push(cur);
        while(cur != start){
            staq.push(parent(cur));
            cur = parent(cur);
        }
        /*stack<Cell>s;
        while(staq.size()){
            s.push(staq.top());
            staq.pop();
        }
        return s;*/
        return staq;
    }

}

// Allows you to use: cout << curr_cell << endl;
ostream& operator<<(ostream& out, const Cell& c){
    return out << "[" << c.row << "," << c.col << "]";
}

// Allows you to use: cout << w << endl;
ostream& operator<<(ostream &os, World& w)
{
    os << "Source: " << w.start << ", Goal: " << w.goal << endl;
    os << "World [" << w.numRows << "x" << w.numCols << "]" << endl;
    for(const auto& r : w._world)
        os << r << endl;
    os << "Distance" << endl;
    for(int i = 0; i < w.numRows; ++i){
        for(int j = 0; j < w.numCols; ++j){
            os << setw(4) << w.distance(i, j);
        }
        os << endl;
    }
    os << "Parents" << endl;
    for(int i = 0; i < w.numRows; ++i){
        for(int j = 0; j < w.numCols; ++j){
            os << " " << w.parent(i, j);
        }
        os << endl;
    }
    return os;
}
