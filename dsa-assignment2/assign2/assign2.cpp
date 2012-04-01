#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <list>

using namespace std;

class Cell 
{
   private:
   char c;

   public:
   
   Cell() : c(0) 
   {
   }
   
   void fillCell(const char &value) 
   {
      c = value;
   }
   
   void print() 
   {
      cout << c;
   }
   
   bool isBlankSpace() 
   {
      return (c == ' ');
   }

   bool isWall() 
   {
      return (c == '#');
   }
   
   bool isStartPoint() 
   {
      return (c == 's');
   }

   bool isFinishPoint() 
   {
      return (c == 'f');
   }

   bool isValid() 
   {
      return (c == ' ' || c == '#' || c == 's' || c == 'f');
   }
};

typedef list<Cell>::const_iterator CellIterator;

class Row 
{
   private:
   list<Cell> cells;

   public:
   
   Row() : cells(0) 
   {
   }
   
   list<Cell>* getCells() 
   {
      return &cells;
   }
   
   void fillRow(const string &s) 
   {
      for (string::const_iterator itr = s.begin(); itr < s.end(); itr++) {
         Cell cell;
         
         cell.fillCell(*itr);
         cells.push_back(cell);
      }
   }

   void print() 
   {
      for (CellIterator itr = cells.begin(); itr != cells.end(); itr++) {
         Cell cell = *itr;
         cell.print();
      }
   }
};

typedef list<Row>::const_iterator RowIterator;

class Maze 
{
   private:
   list<Row> rows;

   public:
   
   Maze() : rows(0) 
   {
   }

   void fillMaze(ifstream &fin) 
   {
      while (!fin.eof()) {
         string line;
         Row row;
         
         getline(fin, line);
         row.fillRow(line);
         rows.push_back(row);
      }
   }
   
   list<Row>* getRows() 
   {
      return &rows;
   }
      
   void print()
   {
      for (RowIterator itr = rows.begin(); itr != rows.end(); itr++) {
         Row row = *itr;
         row.print();
         cout << endl;
      }
   }

   void showMetrics() 
   {
      int deadends = 0;
      int branches = 0;
      
      /* 3 iterator for 3 subsequent vertical rows (top, middle, bottom) 
      run the at the same time */
      
      RowIterator itrTopRow = rows.begin(),
                  itrMiddleRow = rows.begin(), 
                  itrBottomRow = rows.begin();
      
      itrMiddleRow++;
      itrBottomRow++; itrBottomRow++;

      for (; itrBottomRow != rows.end(); 
         itrTopRow++, itrMiddleRow++, itrBottomRow++) 
      {
         Row topRow = *itrTopRow;
         Row middleRow = *itrMiddleRow;
         Row bottomRow = *itrBottomRow;
         
         list<Cell>* topRowList = topRow.getCells();
         list<Cell>* middleRowList = middleRow.getCells();
         list<Cell>* bottomRowList = bottomRow.getCells();
         
         /* 5 iterator run at the same time, a cell and 4 character on its
         left, right, top, and bottom */

         CellIterator itrTopRowList = topRowList->begin();
         CellIterator itrMiddleLeftRowList = middleRowList->begin();
         CellIterator itrMiddleRowList = middleRowList->begin();
         CellIterator itrMiddleRightRowList = middleRowList->begin();
         CellIterator itrBottomRowList = bottomRowList->begin();

         itrTopRowList++;
         itrMiddleRowList++;
         itrMiddleRightRowList++; itrMiddleRightRowList++;
         itrBottomRowList++;

         Cell c = *itrMiddleLeftRowList;
         
         /* Run the iterator until the cell find its first wall */

         while (c.isBlankSpace()) {
            itrTopRowList++;
            itrMiddleLeftRowList++; 
            itrMiddleRowList++; 
            itrMiddleRightRowList++; 
            itrBottomRowList++;
            
            c = *itrMiddleLeftRowList;
         }
         
         /* After the first wall encountered, calculate deadends and branches 
         based on how many wall surrounding the cell */

         for (; itrMiddleRightRowList != middleRowList->end(); 
            itrTopRowList++, 
            itrMiddleLeftRowList++, 
            itrMiddleRowList++, 
            itrMiddleRightRowList++, 
            itrBottomRowList++ ) 
         {
            
            int wallSurround = countWalls(*itrTopRowList, 
                                          *itrMiddleLeftRowList, 
                                          *itrMiddleRightRowList, 
                                          *itrBottomRowList);
                                          
            deadends += countDeadends(*itrMiddleRowList, wallSurround);
            branches += countBranches(*itrMiddleRowList, wallSurround);
         }
      }
      
      cout << "\n";
      cout << "Number of branches in maze is " << branches << "\n";
      cout << "Number of dead-ends in maze is "<< deadends << "\n";
   }

   int countWalls(const Cell &topRowCell,
                  const Cell &middleLeftRowCell,
                  const Cell &middleRightRowCell,
                  const Cell &bottomRowCell) 
   {
      int wallSurround = 0;
      
      wallSurround += countWall(topRowCell);
      wallSurround += countWall(middleLeftRowCell);
      wallSurround += countWall(middleRightRowCell);
      wallSurround += countWall(bottomRowCell);
      return wallSurround;
   }
   
   int countWall(Cell cell)
   {
      return cell.isWall()? 1 : 0;
   }

   int countDeadends(Cell cell, int wallSurround) 
   {
      return (cell.isBlankSpace() && wallSurround == 3)? 1 : 0;
   }

   int countBranches(Cell cell, int wallSurround) 
   {
      int branch = 0;
      
      if (cell.isBlankSpace() || cell.isFinishPoint()) {
         if (wallSurround == 1) {
            branch = 1;
            } else if (wallSurround == 0) {
            branch = 2;
         }
         } else if (cell.isStartPoint()) {
         if (wallSurround == 2) {
            branch = 1;
            } else if (wallSurround == 1) {
            branch = 2;
            } else if (wallSurround == 0) {
            branch = 3;
         }
      }
      
      return branch;
   }
   
   bool isValid()
   {  
      int sumStartPoint = 0;
      int sumFinishPoint = 0;
      
      for (RowIterator itrRow = rows.begin(); itrRow != rows.end(); itrRow++) {
         Row row = *itrRow;
         list<Cell>* rowList = row.getCells();
         
         CellIterator itrCell = rowList->begin();
         
         Cell cell = *itrCell;
         
         /* Checking errors on cells outside the wall (hasn't met its first
         wall or end of row */

         for (; itrCell != rowList->end() && !cell.isWall(); itrCell++) {
            cell = *itrCell;
            if (!cell.isValid()) {
               cout << "Invalid character/s"; 
               cout << endl;
               return false;
            }
            if (cell.isStartPoint()) {
               cout << "Error - start declared outside of maze";
               cout << endl;
               return false;
            }   
            if (cell.isFinishPoint()) {
               cout << "Error - finish declared outside of maze";
               cout << endl;
               return false;
            }
         }
         
         /* Checking errors on cells inside the wall */
         
         for (; itrCell != rowList->end(); itrCell++) {
            cell = *itrCell;
            if (!cell.isValid()) {
               cout << "Invalid character/s";
               cout << endl; 
               return false;
               } else if (cell.isStartPoint()) {
               sumStartPoint++;
               } else if (cell.isFinishPoint()) {
               sumFinishPoint++;
            }
         }
      }
      
      switch (sumStartPoint) 
      {
         case (0):
            cout << "Error - no start found in maze";
            cout << endl;
            return false;
            
         case (1):
            break;
         
         default:
            cout << "Error - multiple start found in maze";
            cout << endl;
            return false;
      }
      
      switch (sumFinishPoint) 
      {
         case (0):
            cout << "Error - no finish found in maze";
            return false; 
            
         case (1):
            break;
            
         default:
            cout << "Error - multiple finish found in maze";
            cout << endl;
            return false;
      }
      return true;
   }
};

Maze loadMaze(const char *file);

int main(int argc, char *argv[]) 
{
   if (argc != 2) {
      cout << "Must supply 1 argument to this program\n";
      return 0;
   }

   Maze maze = loadMaze(argv[1]);
   
   if (!maze.isValid()) {
      cout << "Unable to load maze " << argv[1] << "\n";
      return 0;
   }
   
   maze.print();
   maze.showMetrics();
   return 0;
}

Maze loadMaze(const char *file) 
{
   Maze maze;
   ifstream fin;

   fin.open(file);

   if (!fin) {
      cout << "Unable to read from " << file << "\n";
      exit(0);
   }

   maze.fillMaze(fin);
   fin.close();
   return maze;
}
