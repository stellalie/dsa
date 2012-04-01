#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include "bintree.h"

using namespace std;

// class declarations for maze structure ###################################

class Cell
{
private:
    int x;
    char c;
    bool insideMaze;
    bool insidePath;
    bool isVisited;
    
public:
    Cell(): x(0), c(' ') { }
    Cell(int xpos): x(xpos), c(' ') { }
    Cell(int xpos, char c) : x(xpos), c(c) { }
    
    bool operator==(const Cell &other) const;
    bool operator<(const Cell &other) const;
    char toChar() const;
    bool isBlankSpace() const;
    bool isWall() const;
    bool isStartPoint() const;
    bool isFinishPoint() const;
    bool isValid() const;
    void setInsideMaze(bool value);
    void setInsidePath(bool value);
    bool isInsideMaze() const;
    bool isInsidePath() const;
};

// -------------------------------------------------------------------------

bool Cell::operator==(const Cell &other) const
{
    return (x == other.x);
}

bool Cell::operator<(const Cell &other) const
{
    return (x < other.x);
}

char Cell::toChar() const 

{ 
    if (!isStartPoint() && !isFinishPoint() && insidePath) 
    {
        return '.';
    }
    return c; 
}

bool Cell::isBlankSpace() const
{
    return (c == ' ');
}

bool Cell::isWall() const
{
    return (c == '#');
}

bool Cell::isStartPoint() const
{
    return (c == 's');
}

bool Cell::isFinishPoint() const
{
    return (c == 'f');
}

bool Cell::isValid() const
{
    return (c == ' ' || c == '#' || c == 's' || c == 'f') ;
}

void Cell::setInsideMaze(bool value)
{
    insideMaze = value;
}

void Cell::setInsidePath(bool value)
{
    insidePath = value;
}

bool Cell::isInsideMaze() const 
{
    return insideMaze;
}

bool Cell::isInsidePath() const 
{
    return insidePath;
}

// =========================================================================

class Row
{
private:
    int y;
    bintree<Cell> cells;
    
public:
    
    Row() : y(0), cells() { }
    Row(int ypos) : y(ypos), cells() { }
    
    bool operator==(const Row &other) const;
    bool operator<(const Row &other) const;
    
    string toString() const;
    int size() const;
    
    Cell* cellAt(int x) const;
    void loadRow(const string &s);
};

// -------------------------------------------------------------------------

bool Row::operator==(const Row &other) const
{
    return (y == other.y);
}

bool Row::operator<(const Row &other) const
{
    return (y < other.y);
}

string Row::toString() const
{
    string s;
    
    for (int i = 0; i < size(); i++) 
    {
        Cell *cell = cellAt(i);
        s += cell->toChar();
    }
    return s;
}

int Row::size() const
{
    return cells.size();
}

Cell* Row::cellAt(int x) const 
{
    Cell cell(x);
    return cells.find(cell);
}

void Row::loadRow(const string &s)
{
    int xpos = 0;
    
    for (string::const_iterator itr = s.begin(); itr < s.end(); itr++)
    {
        Cell *cell = new Cell(xpos, *itr);
        cells.insert(*cell);
        xpos++;
    }
}

// =========================================================================

class Maze
{
private:
    bintree<Row> rows;
    int cellStartX, cellStartY;
    
public:
    
    Maze() { }
    
    void printMaze() const;
    
    Row* rowAt(int y) const;
    Cell* cellAt(int x, int y) const;
    void loadMaze(char *mazefile);
    bool sumStartPointCheck(int sumStartPoint);
    bool sumFinishPointCheck(int sumFinishPoint);
    bool isValid();
    void calculatePath();
    bool findMazePath(int x, int y);
};

// -------------------------------------------------------------------------

void Maze::printMaze() const
{
    rows.print();
}

Row* Maze::rowAt(int y) const
{
    Row row(y);
    return rows.find(row);
}

Cell* Maze::cellAt(int x, int y) const
{
    Row* row = rowAt(y);
    return row->cellAt(x);
}

void Maze::loadMaze(char *mazefile)
{
    ifstream mazein;
    int y = 0;
    
    mazein.open(mazefile);
    if (!mazein) 
    {
        cout << "Unable to read from " << mazefile << "\n";
        exit(0);
    }
    
    while (!mazein.eof())
    {
        string line;
        
        Row row(y);
        
        getline(mazein, line);
        row.loadRow(line);
        rows.insert(row);
        y++;
    }
    
    rows.rebalance();
    mazein.close();
}

bool Maze::sumStartPointCheck(int sumStartPoint)
{
    switch (sumStartPoint) 
    {
        case (0):
            cout << "Error - no start found in maze";
            cout << endl;
            return false;
            
        case (1): 
            return true;
            
        default:
            cout << "Error - multiple start found in maze";
            cout << endl;
            return false;
    }
}

bool Maze::sumFinishPointCheck(int sumFinishPoint)
{
    switch (sumFinishPoint) 
    {
        case (0):
            cout << "Error - no finish found in maze";
            return false;
            
        case (1): 
            return true;
            
        default:
            cout << "Error - multiple finish found in maze";
            cout << endl;
            return false;
    }
}

bool Maze::isValid()
{
    int sumStartPoint = 0;
    int sumFinishPoint = 0;
    
    for (int y = 0; y < rows.size(); y++) 
    {
        int x = 0;
        
        Row* row = rowAt(y);
        
        /* Checking errors on cells outside the maze (hasn't met its first
         wall) */
        
        for (int i = 0; i < row->size() && !cellAt(i, y)->isWall(); i++) 
        {
            Cell* cell = cellAt(i, y);
            cell->setInsideMaze(false);       //Set cell as outside the maze
            if (!cell->isValid()) 
            {
                cout << "Invalid character/s"; 
                cout << endl;
                return false;
            }
            if (cell->isStartPoint()) 
            {
                cout << "Error - start declared outside of maze\n";
                return false;
            }
            if (cell->isFinishPoint()) 
            {
                cout << "Error - finish declared outside of maze\n";
                return false;
            }
            x++;      // Keep x-coordinate moving
        }
        
        /* Checking invalid chars on cells inside the maze and counts on start
         and finish points */
        
        for (; x < row->size(); x++) 
        {
            Cell* cell = cellAt(x, y); 
            
            if (!cell->isWall()) 
            {
                cell->setInsideMaze(true);        //Set cell as inside the maze
                cell->setInsidePath(false);       //Set cell as inside the path
            }
            
            if (!cell->isValid()) 
            {
                cout << "Invalid character/s" << endl;
                return false;
            } else if (cell->isStartPoint()) 
            {
                cellStartX = x;       //Store start cell coordinate
                cellStartY = y;
                sumStartPoint++;
            } else if (cell->isFinishPoint()) 
            {
                sumFinishPoint++;
            }
        } 
    }
    
    return (sumStartPointCheck(sumStartPoint) || 
        sumFinishPointCheck(sumFinishPoint))? 1 : 0;
}

void Maze::calculatePath()
{
    if (!findMazePath(cellStartX, cellStartY)) 
    {
    }
}

bool Maze::findMazePath(int x, int y)

{   
    Cell* cell;
    
    Row* row = rowAt(y);
    
    if (x < 0 || y < 0 || x >= row->size() || y >= rows.size()) 
    {
        return false;
    }
    cell = cellAt(x, y);
    if (cell->isFinishPoint()) 
    {
        cell->setInsidePath(true);
        return true;
    } else if (!cell->isInsideMaze() || cell->isInsidePath()) 
    {
        return false;
        
        // Recursive case.
        // Attempt to find a path from each neighbour.
        // Tentatively mark cell as on path.
        
    } else 
    {
        cell->setInsidePath(true);
        if (findMazePath(x - 1, y) 
            || findMazePath(x + 1, y) 
            || findMazePath(x, y - 1)
            || findMazePath(x, y + 1)) 
        {
            return true;
        } else 
        {
            cell->setInsidePath(false);
            return false;
        }
    }
}

// Non class functions #####################################################

int main(int argc, char *argv[])
{
    Maze maze;
    
    if (argc != 2)
    {
        cout << "Must supply 1 argument to this program\n";
        return 0;
    }
    
    maze.loadMaze(argv[1]);
    if (!maze.isValid()) 
    {
        cout << "Unable to load maze " << argv[1] << "\n";
        exit(0);
    }
    maze.calculatePath();
    maze.printMaze();
    
    exit(0);
}
