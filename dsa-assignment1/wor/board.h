#ifndef _BOARD_H
#define _BOARD_H

#include "constants.h"
#include "piece.h"
#include "move.h"

class board
{
	// the board object manages the board. 
	// Particularly, which pieces are on each square

    private:
		// the array that hold the board square and which pieces are on it
        piece* squares[MAXDIM][MAXDIM];
        
	    void setupBoard();
        void clone(board* other);
        void clear();
        
		// private methods for printing sections of the board
        void printRow(int y);        
        void printMiddleLine();      
        void printOuterLine();      
        void printBottomLine();
        
        piece* getPiece(int x, int y);
        
    
    public:
    
        board();
        board(board *other);
        ~board();
		
        void printBoard();
        void printScore();
		
        bool movePiece(PieceColor playerColor, move theMove);
        bool insideBoardBoundaries(int xx, int yy);
	    bool legalMove(PieceColor playerColor, move theMove);
        
        PieceColor getPieceColor(int x, int y);
        PieceType getPieceType(int x, int y);
    
        int whiteTotal();
        int blackTotal();
        
        int numWhite();
        int numBlack();
        int numPieces(PieceColor playerColor);
        
        int currentScore(PieceColor playerColor);
};

#endif

