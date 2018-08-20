#include "world.h"

#include <stdexcept>
#include <fstream>
#include <iomanip>

using namespace std;

// Construct the world object.
// Note that this takes in an istream (cin or an fstream), NOT a string as per the pdf
World::World(istream &input)
{
  input>>numRows>>numCols;input.ignore();
  for(int k=0;k<numRows;++k)
  {
   string line;
   getline(input,line);
   _world.push_back(line);
   for (int j=0;j<numCols;j++)
     {
     // Cell Maze=parent(j,k);
      if( line[j]=='S')
       {
         start.row=k;
         start.col=j;
       }
      if( line[j]=='G')
       {
         goal.row=k;
         goal.col=j;
       }
     }

  }

  _distance=new int*[numRows];
  _parent=new Cell*[numRows];
  _discovered= new bool*[numRows];

  for (int j=0;j<numRows;++j)
  {
   _distance[j]=new int[numCols];
    _parent[j]=new Cell[numCols];
    _discovered[j]= new bool[numCols];
  }

 for (int r=0;r<numRows;++r)
     for (int c=0;c<numCols;++c)
     {
       _distance[r][c]=10000;
       _parent[r][c]={-1,-1};
       _discovered[r][c]=false;
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
  return distance(c.row,c.col);
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
  return (_world[r][c]!='x');
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
 return discovered(c.row,c.col);
}

// Calculate the shortest path as per the algorithm in the pdf
stack<Cell> World::FindPath()
{
    queue<Cell> q;
    stack<Cell> s;

   // _distance[numRows][numCols]=1000;
    distance(start)=0;
    parent(start)={-2,-2};
    discovered(start)=true;
    q.push({start.row,start.col});
    Cell curr,tmp;

    while ((q.size()!=0)&&(discovered(goal)!=true))
    {
       curr = q.front();
       q.pop();
       tmp = {curr.row-1,curr.col};
       if ((isOpen(tmp))&&(discovered(tmp)!=true))
       {
         distance(tmp)=distance(curr)+1;
         parent(tmp)=curr;
         discovered(tmp)=true;
         q.push(tmp);
       }

       tmp={curr.row+1,curr.col};
       if ((isOpen(tmp))&&(discovered(tmp)!=true))
       {
         distance(tmp)=distance(curr)+1;
         parent(tmp)=curr;
         discovered(tmp)=true;
         q.push(tmp);
       }

       tmp={curr.row,curr.col-1};
       if ((isOpen(tmp))&&(discovered(tmp)!=true))
       {
         distance(tmp)=distance(curr)+1;
         parent(tmp)=curr;
         discovered(tmp)=true;
         q.push(tmp);
       }

       tmp = {curr.row,curr.col+1};
       if ((isOpen(tmp))&&(discovered(tmp)!=true))
       {
         distance(tmp)=distance(curr)+1;
         parent(tmp)=curr;
         discovered(tmp)=true;
         q.push(tmp);
       }
    }

    if ((q.size()==0)&&(discovered(goal)!=true))
    {


    }
    else
     {
      curr = goal;
     s.push(curr);
     while (curr!=start)
     {
       s.push(parent(curr));
       curr = parent(curr);
    }
   }
    // Popping items off of S will give the path in the correct order
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
//Robert Basson
