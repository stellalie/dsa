#ifndef _MOVE_H
#define _MOVE_H

#include <stdlib.h>
#include <string>

struct move
{
    // struct to store coordinates for moving a piece from a square to a square
    
    int fromX;
    int fromY;
    int toX;
    int toY;
    
    move() 
        : fromX(-1), fromY(-1), toX(-1), toY(-1)
    {
    }
    
    move(int fx, int fy, int tox, int toy) 
        : fromX(fx), fromY(fy), toX(tox), toY(toy)
    {
        
    }
    
    std::string toString()
    {
        std::string s;
        
        if (fromX == 0) { s += "a"; }
        else if (fromX == 1) { s += "b"; }
        else if (fromX == 2) { s += "c"; }
        else if (fromX == 3) { s += "d"; }
        
        if (fromY == 0) { s += "0"; }
        else if (fromY == 1) { s += "1"; }
        else if (fromY == 2) { s += "2"; }
        else if (fromY == 3) { s += "3"; }
        
        s += " ";
        
        if (toX == 0) { s += "a"; }
        else if (toX == 1) { s += "b"; }
        else if (toX == 2) { s += "c"; }
        else if (toX == 3) { s += "d"; }
        
        if (toY == 0) { s += "0"; }
        else if (toY == 1) { s += "1"; }
        else if (toY == 2) { s += "2"; }
        else if (toY == 3) { s += "3"; }
        
        return s;
    }
    
    void setMove(int fx, int fy, int tox, int toy)
    {
        fromX = fx;
        fromY = fy;
        toX = tox;
        toY = toy;
    }
    
    int xMove()
    {
        // return number of x spaces we have moved
        return toX - fromX;
    }

    int yMove()
    {
        // return number of y spaces we have moved
        return toY - fromY;
    }
    
    bool atToLocation(int x, int y)
    {
		// return if we are moving to x, y location
        return (x == toX && y == toY);
    }
    
    bool onOrthogonal()
    {
        // return if move is either along vertical or horzontal line
        return (toX == fromX  || toY == fromY);

    }
    
    bool onDiagonal()
    {
        // return if move is along a 45 degree diagonal line
        return (abs(xMove()) == abs(yMove()));
    }

    bool moveOneSpaceOnly()
    {
        // return if the move is, at most, one space only
        return (abs(xMove()) <= 1 && abs(yMove()) <= 1);
    }
    
};

#endif
