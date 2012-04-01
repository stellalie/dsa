#include <iostream>
#include <string.h>

#include "piece.h"
#include "board.h"
           
piece::piece()
{
    pieceColor = BLANK;
}
        
piece::piece(PieceColor c)
{
    pieceColor = c;
}

piece::~piece()
{
}
        
bool piece::interveningSquaresEmpty(board *theBoard, move theMove)
{
   // assumes we are on a orthoganol (this is horizontal or vertical) line 
   // or 45 degree diagonal line
   // Determine if the squares between from location and to location are empty of pieces
   
   // work out how far the move is. 
   // If were are moving along an orthoganal line then either addx or addy will be zero
   int addx = theMove.xMove();
   int addy = theMove.yMove();
 
   // work out which direction we are moving from location to location
   // we will move in that direcion one square at a time
   if (addx < 0) addx = -1; if (addx > 0) addx = 1;
   if (addy < 0) addy = -1; if (addy > 0) addy = 1;
   
   // start shifting in the direction of toX, toY
   int shiftx = theMove.fromX + addx;
   int shifty = theMove.fromY + addy;
   
   while (!theMove.atToLocation(shiftx, shifty))
   {
        // make sure we haven't moved outside board boundaries
        if (!theBoard->insideBoardBoundaries(shiftx, shifty)) return false;

        // we can only move through a square if it is empty
        if (theBoard->getPieceType(shiftx, shifty) != NOPIECE) return false;

        shiftx += addx;
        shifty += addy;
   }
   return true;
}

// knight functions ===========================================

bool knight::legalMove(board *theBoard,  move theMove)
{  
    // have we moved 2 then 1 space or 1 then 2 spaces?
    return (abs(theMove.xMove() * theMove.yMove()) == 2);
}
		
const char* knight::getPieceChar()
{
    if (pieceColor == WHITE) return " K "; else return "(k)";
}

// bishop functions ===========================================

bool bishop::legalMove(board *theBoard,  move theMove)
{  
    // must be on diagonal
    if (!theMove.onDiagonal()) return false;

    return interveningSquaresEmpty(theBoard, theMove);
}
		
const char* bishop::getPieceChar()
{
    if (pieceColor == WHITE) return " B "; else return "(b)";
}
		
// rook functions ============================================

bool rook::legalMove(board *theBoard,  move theMove)
{
    // must be on same column or row
    if (!theMove.onOrthogonal()) return false;

    return interveningSquaresEmpty(theBoard, theMove);
}

const char* rook::getPieceChar()
{
    if (pieceColor == WHITE) return " R "; else return "(r)";
}

// duke functions ===========================================

bool duke::legalMove(board *theBoard,  move theMove)
{
    return theMove.moveOneSpaceOnly();
}

const char* duke::getPieceChar()
{
    if (pieceColor == WHITE) return " D "; else return "(d)";
}  

bool duke::moveMakesPrince(move theMove)
{
    if (pieceColor == WHITE)
    {
        return (theMove.toY == 0);
    }
    else
    {
        return (theMove.toY == MAXDIM-1);
    }
}

// prince functions =========================================

bool prince::legalMove(board *theBoard,  move theMove)
{
    // move must be on a row, column or diagonal
    if (!theMove.onDiagonal() && !theMove.onOrthogonal()) return false;

    return interveningSquaresEmpty(theBoard, theMove);
}

const char* prince::getPieceChar()
{
    if (pieceColor == WHITE) return " P "; else return "(p)";
}

