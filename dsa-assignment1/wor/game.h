#ifndef _GAME_H
#define _GAME_H

// forward declarations
class board;
class piece;

#include <time.h>
#include <list>

#include "constants.h"
#include "move.h"
#include "piece.h"
#include "board.h"
#include "ply.h"

using namespace std;

class game
{
	// the game object manages the game of wor.
	// the moves, who wins/loses, etc

	private:
        board gameBoard;
        bool computerIsWhite;
        PieceColor currentPlayer;
        int numMoves;
        char* entryString;
        
        bool splitUpMove(char *entry, move &theMove);
        char* getMove();
    
        move theBestMove;
			
	public:
        game();
        ~game() {}
            
        void printBoard();
        
        void printWinner();
        
        void printMenu();
        
        void nextMove();
		
        bool ended();
    
        void setComputer(PieceColor computerPlayer);
    
        bool isComputer(PieceColor p);
    
        list<move> allPossibleMoves(board *theBoard);
    
        move computerMove();
    
        int moveScore(board *theBoard, move theMove);
    
        move minMaxMove(board *theBoard, int depth);
		int minMove(board *theBoard, int depth);
		int maxMove(board *theBoard, int depth);
    
    
};

#endif
