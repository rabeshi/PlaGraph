#include "world.h"

#include <stdexcept>
#include <fstream>
#include <iomanip>
#include <stack>
#include <queue>

using namespace std;

// Construct the world object.
// Note that this takes in an istream (cin or an fstream), NOT a string as per the pdf
World::World(istream &input)
{
    input >>numRows >> numCols;

    _distance = new int*[numRows];
    _parent = new Cell*[numRows];
    _discovered = new bool*[numRows];

    for(int count = 0 ; count< numRows; count++)
    {
        _distance[count]= new int[numCols];
        _parent[count]= new Cell[numCols];
        _discovered[count]= new bool[numCols];
    }

    string temp = "";
    getline(input, temp);
        for(int countr = 0; countr<numRows; countr++)
        {
            getline(input, temp);
            _world.push_back(temp);
            for(int countc = 0; countc < numCols ; countc++)
            {
                if(temp[countc] == 'S')
                {
                    start.row = countr;
                    start.col = countc;
                }
                else if(temp[countc] == 'G')
                {
                    goal.row = countr;
                    goal.col = countc;
                }

                if(temp[countc] == 'x')
                {
                    _parent[countr][countc].row = -3;
                    _parent[countr][countc].col = -3;
                }
                else
                {
                    _parent[countr][countc].row = -1;
                    _parent[countr][countc].col = -1;
                }

                _distance[countr][countc]= numRows*numCols;
                _discovered[countr][countc]= false;
            }
        }
}

World::~World()
{
    // Not tested
}

// 1 Line, return the relevant distance
int &World::distance(int r, int c)
{
    return _distance[r][c];
}

// 1 Line, return the relevant distance
int &World::distance(Cell c)
{
    return _distance[c.row][c.col];
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
    return (-1 == _parent[r][c].row);
}

// 1 Line, return whether the cell is open or a wall.
bool World::isOpen(Cell c)
{
    return (-1 == _parent[c.row][c.col].row);
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
    queue<Cell>* q = new queue<Cell>;
    stack<Cell>* s = new stack<Cell>;

    _distance[start.row][start.col]=0;
    _parent[start.row][start.col].row = -2;
    _parent[start.row][start.col].col = -2;

    bool found = false;
    Cell curr;
    Cell temp;

    q->push(start);
    while(q->size()!= 0 && !found)
    {
        curr = q->front();
        q->pop();

        temp = curr;
        temp.row--;
        if(isOpen(temp) && !discovered(temp))
        {
            distance(temp) = distance(curr)+1;
            parent(temp) = curr;
            discovered(temp) = true;
            q->push(temp);
            if(temp == goal)
            {
                found = true;
            }
        }

        temp = curr;
        temp.row++;
        if(isOpen(temp) && !discovered(temp))
        {
            distance(temp) = distance(curr)+1;
            parent(temp) = curr;
            discovered(temp) = true;
            q->push(temp);
            if(temp == goal)
            {
                found = true;
            }
        }

        temp = curr;
        temp.col--;
        if(isOpen(temp) && !discovered(temp))
        {
            distance(temp) = distance(curr)+1;
            parent(temp) = curr;
            discovered(temp) = true;
            q->push(temp);
            if(temp == goal)
            {
                found = true;
            }
        }

        temp = curr;
        temp.col++;
        if(isOpen(temp) && !discovered(temp))
        {
            distance(temp) = distance(curr)+1;
            parent(temp) = curr;
            discovered(temp) = true;
            q->push(temp);
            if(temp == goal)
            {
                found = true;
            }
        }
    }

    if(q->empty() && !found)
    {
        return *s;
    }
    else
    {
        curr = goal;
        s->push(curr);
        while(curr != start)
        {
            s->push(parent(curr));
            curr = parent(curr);
        }
        return *s;
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
