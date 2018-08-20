#include "world.h"

#include<sstream>
#include <stdexcept>
#include <fstream>
#include <iomanip>

using namespace std;

// Construct the world object.
// Note that this takes in an istream (cin or an fstream), NOT a string as per the pdf
World::World(istream &input)
{
    string line="";
    int counter=0;
    input >> numRows >> numCols; input.ignore();
    while(getline(input,line)){

        _world.push_back(line);

    }
    for(int i=0;i<numRows;i++){
        const char *line = _world[i].c_str();
        for(int j=0;j<numCols;j++){
            if(line[j]=='S'){
                start.row=i;
                start.col=j;

            }else if(line[j]=='G'){
                goal.row=i;
                goal.col=j;
            }
        }
    }
    _distance=new int*[numRows];
    _parent=new Cell*[numRows];
    _discovered=new bool*[numRows];

    for(int i=0;i<numRows;i++){
        _distance[i]=new int[numCols];
        _parent[i]=new Cell[numCols];
        _discovered[i]=new bool[numCols];
    }
    for(int i=0;i<numRows;i++){
        for(int j=0;j<numCols;j++){
            _distance[i][j]=42;
            _parent[i][j]={-1,-1};
            _discovered[i][j]=false;
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
    return distance(c.row, c.col);

}

// 1 Line, return the relevant parent
Cell &World::parent(int r, int c)
{
    return _parent[r][c];
}

// 1 Line, return the relevant parent
Cell &World::parent(Cell c)
{
    return parent(c.row,c.col);
}

// 1 Line, return whether the cell is open or a wall.
bool World::isOpen(int r, int c)
{
    if(_world[r][c]=='x'){
        return false;
    }
    return true;
}

// 1 Line, return whether the cell is open or a wall.
bool World::isOpen(Cell c)
{
    if(_world[c.row][c.col]=='x'){
        return false;
    }
    return true;
}

// 1 Line, return whether the cell has been discovered
bool& World::discovered(int r, int c)
{
    return _discovered[r][c];
}

// 1 Line, return whether the cell has been discovered
bool& World::discovered(Cell c)
{
    return discovered(c.row,c.col);
}

// Calculate the shortest path as per the algorithm in the pdf
stack<Cell> World::FindPath()
{

    stack <Cell> s;
    queue <Cell> q;
    _distance[start.row][start.col]=0;
    discovered(start) = true;
    q.push(start);
    Cell temp, curr;

    while( !q.empty() && !discovered(goal)){
        curr=q.front();
        q.pop();

        cout << curr << endl;
        temp = {curr.row+1,curr.col};
        if((isOpen(temp))&&(!discovered(temp))){
            distance(temp)=distance(curr)+1;
            _parent[temp.row][temp.col]=curr;
            discovered(temp) = true;
            cout << "Enqueue: " << temp << endl;
            q.push(temp);
        }

        temp = {curr.row-1,curr.col};
        if((isOpen(temp))&&(!discovered(temp))){
            distance(temp)=distance(curr)+1;
            _parent[temp.row][temp.col]=curr;
            discovered(temp) = true;
            cout << "Enqueue: " << temp << endl;
            q.push(temp);
        }

        temp = {curr.row,curr.col-1};
        if((isOpen(temp))&&(!discovered(temp))){
            distance(temp)=distance(curr)+1;
            _parent[temp.row][temp.col]=curr;
            discovered(temp) = true;
            cout << "Enqueue: " << temp << endl;
            q.push(temp);
        }

        temp = {curr.row,curr.col+1};
        if((isOpen(temp))&&(!discovered(temp))){
            distance(temp)=distance(curr)+1;
            _parent[temp.row][temp.col]=curr;
            discovered(temp) = true;
            cout << "Enqueue: " << temp << endl;
            q.push(temp);
        }
    }
    if((q.empty())&&(_world[q.front().row][q.front().col])!='G'){
        stack <Cell>fail;
        cout<<"\n"<<"No Path to goal";
        return fail;
    }else{
        curr=goal; s.push(curr);
        while(curr!=start){
            s.push(parent(curr));
            curr=parent(curr);
        }
        return s;
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
