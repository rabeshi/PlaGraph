#include "world.h"

#include <stdexcept>
#include <fstream>
#include <iomanip>
#include <array>
#include <iostream>

using namespace std;

// Construct the world object.
// Note that this takes in an istream (cin or an fstream), NOT a string as per the pdf
World::World(istream &input)
{      
                string ch;


              string numRC;
                    getline(input,numRC);
                    size_t space=numRC.find(' ');
                    string srow=numRC.substr(0,space);
                    string scol=numRC.substr(space+1,numRC.length());
                    numRows=stoi(srow);
                    numCols=stoi(scol);

                  _distance=new int*[numRows];
                  _parent=new Cell*[numRows];
                  _discovered=new bool*[numRows];

                        for (int j=0; j<numRows;j++)
                        {
                            _distance[j]=new int[numCols];
                            _parent[j]=new Cell[numCols];
                            _discovered[j]=new bool[numCols];
                            getline(input,ch);
                             _world.push_back(ch);

                            for (int i=0; i <numCols; i++)
                            {
                                _distance[j][i]=-1;
                                _parent[j][i]={-1,-1};
                                _discovered[j][i]=false;

                                if(ch[i]=='S')
                                {
                                    //setting start cell up
                                    start.row=j;
                                    start.col=i;
                                }
                                else if (ch[i]=='G')
                                {
                                    //setting goal cell up
                                    goal.row=j;
                                    goal.col=i;
                                }

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
    return(_distance[r][c]);
}

// 1 Line, return the relevant distance
int &World::distance(Cell c)
{
    return(_distance[c.row][c.col]);
}

// 1 Line, return the relevant parent
Cell &World::parent(int r, int c)
{
    return(_parent[r][c]);
}

// 1 Line, return the relevant parent
Cell &World::parent(Cell c)
{
    return(_parent[c.row][c.col]);
}

// 1 Line, return whether the cell is open or a wall.
bool World::isOpen(int r, int c)
{
    if (_world[r][c]=='x')
    {
        return(false);
    }
    else
        return(true);
}

// 1 Line, return whether the cell is open or a wall.
bool World::isOpen(Cell c)
{
    if (_world[c.row][c.col]=='x')
    {
        return(false);
    }
    else
        return(true);
}

// 1 Line, return whether the cell has been discovered
bool& World::discovered(int r, int c)
{
    return(_discovered[r][c]);
}

// 1 Line, return whether the cell has been discovered
bool& World::discovered(Cell c)
{
    return(_discovered[c.row][c.col]);
}

// Calculate the shortest path as per the algorithm in the pdf
stack<Cell> World::FindPath()
{
    queue<Cell>q;
    stack<Cell>s;
    Cell curr,tmp;
    _distance[start.row][start.col]=0;
    _parent[start.row][start.col]={-2,-2};
    discovered(start)=true;
    q.push(start);

    while(q.empty()==false && _discovered[goal.row][goal.col]==false)
    {
        curr=q.front();
        q.pop();

        //checking cell above curr
        tmp={(curr.row)-1,(curr.col)};
             if(isOpen(tmp) && !discovered(tmp))
             {
                 distance(tmp)=distance(curr)+1;
                 parent(tmp)=curr;
                 discovered(tmp)=true;
                 q.push(tmp);
             }

       //checking cell below curr
             tmp={(curr.row)+1,(curr.col)};
                  if(isOpen(tmp) && !discovered(tmp))
                  {
                      distance(tmp)=distance(curr)+1;
                      parent(tmp)=curr;
                      discovered(tmp)=true;
                      q.push(tmp);
                  }

      //checking cell left of curr
                  tmp={(curr.row),(curr.col)-1};
                       if(isOpen(tmp) && !discovered(tmp))
                       {
                           distance(tmp)=distance(curr)+1;
                           parent(tmp)=curr;
                           discovered(tmp)=true;
                           q.push(tmp);
                       }

     //checking cell right of curr
                       tmp={(curr.row),(curr.col)+1};
                            if(isOpen(tmp) && !discovered(tmp))
                            {
                                distance(tmp)=distance(curr)+1;
                                parent(tmp)=curr;
                                discovered(tmp)=true;
                                q.push(tmp);
                            }
    }

   if(q.empty()==true && _discovered[goal.row][goal.col]==false)
    {
       cout<<"No path to goal";
    }
    else
    {
        curr=goal;
        s.push(curr);

        while(curr!=start)
        {
            s.push(parent(curr));
            curr=parent(curr);
        }

    }
    return(s);

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
