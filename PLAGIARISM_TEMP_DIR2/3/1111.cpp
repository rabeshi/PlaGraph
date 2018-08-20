#include "world.h"

#include <stdexcept>
#include <fstream>
#include <iomanip>

using namespace std;

World::World(istream &input)
{
    istream& f = input;

    f >> numRows >> numCols; f.ignore();
    for(int i = 0; i < numRows; ++i){
        string curr;
        getline(f, curr);
        _world.push_back(curr);
        auto col = curr.find('S');
        if(col != curr.npos){
            start = {i, (int)col};
        }
        col = curr.find('G');
        if(col != curr.npos){
            goal = {i, (int)col};
        }

    }

    _distance = new int*[numRows];
    _parent   = new Cell*[numRows];
    _discovered = new bool*[numRows];
    for(int r = 0; r < numRows; ++r){
        _distance[r] = new int[numCols];
        _parent[r] = new Cell[numCols];
        _discovered[r] = new bool[numCols];
    }
    for(int r = 0; r < numRows; ++r){
        for(int c = 0; c < numCols; ++c){
            distance(r,c) = 999;
            parent(r,c) = {-1,-1};
            discovered(r,c) = false;
        }
    }

}

World::~World()
{

}

int &World::distance(int r, int c)
{
    return _distance[r][c];
}

int &World::distance(Cell c)
{
    return distance(c.row, c.col);
}

Cell &World::parent(int r, int c)
{
    return _parent[r][c];
}

Cell &World::parent(Cell c)
{
    return parent(c.row, c.col);
}

bool World::isOpen(int r, int c)
{
    return _world[r][c] != 'x';
}

bool World::isOpen(Cell c)
{
    return isOpen(c.row, c.col);
}

bool& World::discovered(int r, int c)
{
    return _discovered[r][c];
}

bool& World::discovered(Cell c)
{
    return discovered(c.row, c.col);
}

stack<Cell> World::FindPath()
{
    stack<Cell> s;
    queue<Cell> q;

    distance(start) = 0;
    parent(start) = {-2,-2};
    q.push(start);
    discovered(start) = true;

    while(!q.empty() && !discovered(goal)){
        Cell curr = q.front(); q.pop();
        Cell tmp;

        tmp = curr; --tmp.row;
        if(isOpen(tmp) && !discovered(tmp)){
            distance(tmp) = distance(curr) + 1;
            parent(tmp) = curr;
            discovered(tmp) = true;
            q.push(tmp);
        }

        tmp = curr; ++tmp.row;
        if(isOpen(tmp) && !discovered(tmp)){
            distance(tmp) = distance(curr) + 1;
            parent(tmp) = curr;
            discovered(tmp) = true;
            q.push(tmp);
        }

        tmp = curr; --tmp.col;
        if(isOpen(tmp) && !discovered(tmp)){
            distance(tmp) = distance(curr) + 1;
            parent(tmp) = curr;
            discovered(tmp) = true;
            q.push(tmp);
        }

        tmp = curr; ++tmp.col;
        if(isOpen(tmp) && !discovered(tmp)){
            distance(tmp) = distance(curr) + 1;
            parent(tmp) = curr;
            discovered(tmp) = true;
            q.push(tmp);
        }
    }

    if(discovered(goal)){
        Cell curr = goal; s.push(curr);
        while(curr != start){
            curr = parent(curr);
            s.push(curr);
        }
    }
    return s;
}

ostream& operator<<(ostream& out, const Cell& c){
    return out << "[" << c.row << "," << c.col << "]";
}

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
