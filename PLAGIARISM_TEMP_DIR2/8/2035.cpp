#include "world.h"

#include <stdexcept>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

// Construct the world object.
// Note that this takes in an istream (cin or an fstream), NOT a string as per the pdf
World::World(istream &input)
{
	vector<string> ss;
	string world;
	while(getline(input,world))
	{
		ss.push_back(world);
	}
	int a[2];
	stringstream convert(ss[0]);
	for(int i = 0; i < 2; i++)
	{
		convert >> a[i];
	}
	numRows=a[0];
	numCols=a[1];
	for(int i = 1; i <= numRows; i++)
	{
		_world.push_back(ss[i]);
	}

	for(int i = 0; i < numRows; i++)
	{
		for(int j = 0; j < numCols; j++)
		{
			if(_world[i][j] == 'G'){
			goal.row = i;
			goal.col = j;
			}

			if(_world[i][j] == 'S'){
			start.row = i;
			start.col = j;
			}
		}
	}
	const int r = numRows;
	const int c = numCols;

	_distance = new int*[r];
	for (int i = 0; i < r; i++)
	{
		_distance[i] = new int[c];
		for(int j = 0; j < c; j ++)
		{
			_distance[i][j] = -2;
		}
	}

	_parent = new Cell*[r];
	for (int i = 0; i < r; i++)
	{
		_parent[i] = new Cell[c];
		for(int j = 0; j < c; j ++)
		{
			_parent[i][j].row = -7;
			_parent[i][j].col = -7;
		}
	}

	_discovered = new bool*[r];
	for (int i = 0; i < r; i++)
	{
		_discovered[i] = new bool[c];
		for(int j = 0; j < c; j ++)
		{
			_discovered[i][j] = false;
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
	return ((_world[r][c]=='G')||(_world[r][c]==' ')||(_world[r][c]=='S'));
}

// 1 Line, return whether the cell is open or a wall.
bool World::isOpen(Cell c)
{
	return isOpen(c.row, c.col);
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
	queue<Cell> q;
	stack<Cell> s;
	stack<Cell> d;
	_distance[start.row][start.col] = 0;
	_parent[start.row][start.col].row = -2;
	_parent[start.row][start.col].col = -2;
	_discovered[start.row][start.col] = true;

	q.push(start);
	Cell curr;
	Cell temp;

	while((!q.empty())&&(!discovered(goal)))    
	{
		curr = q.front();
		q.pop();
		temp = curr;
		temp.row = temp.row - 1; 

		if((isOpen(temp))&&(!discovered(temp)))
		{
			_distance[temp.row][temp.col] = distance(curr) + 1;
			_parent[temp.row][temp.col] = curr;
			_discovered[temp.row][temp.col] = true;
			q.push(temp);
		}
		temp = curr;
		temp.row = temp.row + 1; 

		if((isOpen(temp))&&(!discovered(temp)))
		{
			_distance[temp.row][temp.col] = distance(curr) + 1;
			_parent[temp.row][temp.col] = curr;
			_discovered[temp.row][temp.col] = true;
			q.push(temp);
		}

		temp = curr;
		temp.col = temp.col - 1; 

		if((isOpen(temp))&&(!discovered(temp)))
		{
			_distance[temp.row][temp.col] = distance(curr) + 1;
			_parent[temp.row][temp.col] = curr;
			_discovered[temp.row][temp.col] = true;
			q.push(temp);
		}

		temp = curr;
		temp.col = temp.col + 1; 

		if((isOpen(temp))&&(!discovered(temp)))
		{
			_distance[temp.row][temp.col] = distance(curr) + 1;
			_parent[temp.row][temp.col] = curr;
			_discovered[temp.row][temp.col] = true;
			q.push(temp);
		}

	}

	if((!q.empty())||(discovered(goal)))
  {
		curr = goal;
		s.push(curr);
		while(curr!=start)
		{
			s.push(parent(curr));
			curr = parent(curr);
		}
	}

	return s;
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
