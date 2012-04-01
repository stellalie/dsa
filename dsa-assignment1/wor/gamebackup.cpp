
#include <string.h>
#include <iostream>

#include "game.h"

using namespace std;

game::game()
{
    currentPlayer = WHITE;
    numMoves = 0;
    entryString = new char[ENTRYMAX];
    
    srand(time(NULL));
}
        
bool game::splitUpMove(char *entry, move &theMove)
{
	// the user has entered a move - eg a1 b3
	// split this up and update the move object

    if (entry == NULL) return false;
    
    if (strlen(entry) != 5) return false;
    if (entry[2] != ' ') return false;
    
    theMove.fromX = entry[0] - 'a';
    theMove.fromY = entry[1] - '0';
    theMove.toX = entry[3] - 'a';
    theMove.toY = entry[4] - '0';
    
    return true;
}

char* game::getMove()
{
	// ask the user to enter a move
    if (currentPlayer == WHITE)
        cout << "\n" << numMoves << ". Next move, white to play : ";
    else
        cout << "\n" << numMoves << ". Next move, black to play : ";
      
    cin.getline(entryString, ENTRYMAX);
    
    return entryString;
}
    
void game::printMenu()
{
    cout << "\nINSTRUCTIONS\n";
    cout << "m     : print this menu\n";
    cout << "q     : quit\n";
    cout << "a2 b3 : move piece from a2 to b3\n";
    cout << "\n";
}

void game::printBoard()
{
    gameBoard.printBoard();
}

void game::printWinner()
{
	// print the results of the game
    int totWhite = gameBoard.whiteTotal();
    int totBlack = gameBoard.blackTotal();
    
    if (totWhite > totBlack)
    {
        cout << "\nAfter " << numMoves << " moves white has won with a score of " 
            << totWhite << " to " << totBlack << "\n";
    }
    else if (totBlack > totWhite)
    {
        cout << "\nAfter " << numMoves << " moves black has won with a score of " 
            << totBlack << " to " << totWhite << "\n";
    }
    else
    {
        cout << "\nAfter " << numMoves << " moves it's a draw with a score of " 
            << totWhite << " each\n";
    }
}

void game::nextMove()
{
	// ask the user to enter a move, validate the move and, if valid, run it
    bool validMove = true;
    move theMove;
    if (isComputer(currentPlayer)) {
        theMove = computerMove();
        if (currentPlayer == WHITE)
            cout << "\n" << numMoves << ". White moves : " << theMove.toString() << "\n\n";
        else
            cout << "\n" << numMoves << ". Black moves : " << theMove.toString() << "\n\n";
        
    } else {
        char* entry = getMove();
        
        if (entry[0] == 'q' && entry[1] == '\0') 
        {
            // the player has entered the quit commmand
            exit(0);
        }
        else if (entry[0] == 'm' && entry[1] == '\0')
        {
            // the player has entered the print menu command
            printMenu();
            return;
        }
        else 
        {
            // the player has possibly entered a move instruction
            validMove = splitUpMove(entry, theMove);
        }
    }
    
    if (validMove)
    {
        // a properly formated move instruction has been entered
        // now try and run it
        validMove = gameBoard.movePiece(currentPlayer, theMove);
    }
    
    if (validMove)
    {
        // a properly formated, legal move was entered
        gameBoard.printBoard();
        gameBoard.printScore();
        
        // swap the current player and update moves
        currentPlayer = piece::otherColor(currentPlayer);
        numMoves++;
    }
    else
    {
		cout << "THE MOVE : " << theMove.toString() << " ";
        cout << "invalid move entered\n";
        exit(0);
    }
}

bool game::ended()
{
    static int numMovesOneEach = 2;
    // game ends if MAXMOVES made or one side has no pieces left.
    // Also ends if each side has only one piece left. It is 
    // impossible to force taking the other piece in these circumstances
    
    int remain = gameBoard.numWhite() * gameBoard.numBlack();
    if (remain == 1) numMovesOneEach--;
    
    return (numMoves >= MAXMOVES || remain == 0 || numMovesOneEach < 0);
}

void game::setComputer(PieceColor computerPlayer) {
    if (computerPlayer == WHITE) {
        computerIsWhite = true;
    } else {
        computerIsWhite = false;
    }

}

bool game::isComputer(PieceColor p) {
    if (p == WHITE && (computerIsWhite)) {
        return true;
    } else if (p == WHITE && (!computerIsWhite)) {
        return false;
    } else if (p == BLACK && (computerIsWhite)) {
        return false;
    } else if (p == BLACK && (!computerIsWhite)) {
        return true;
    }
    cout << "Error has occured";
    return false;
}

list<move> game::allPossibleMoves() {
    list<move> allPossibleMoves;
    for (int fromX = 0; fromX < MAXDIM; fromX++) {
        for (int fromY = 0; fromY < MAXDIM; fromY++) {
            
            if (gameBoard.getPieceType(fromX, fromY) != NOPIECE) {
                for (int toX = 0; toX < MAXDIM; toX++) {
                    for (int toY = 0; toY < MAXDIM; toY++) {
                        move theMove(fromX, fromY, toX, toY);
                        if (gameBoard.legalMove(currentPlayer, theMove)) {
                            allPossibleMoves.push_back(theMove);
                        }
                    }
                    
                }
                
            } // end if piece is not blank
        }
    }
    cout << endl;
    return allPossibleMoves;
}

list<move> game::allPossibleMoves(board *theBoard) {
    list<move> allPossibleMoves;
    for (int fromX = 0; fromX < MAXDIM; fromX++) {
        for (int fromY = 0; fromY < MAXDIM; fromY++) {
            
            if (theBoard->getPieceType(fromX, fromY) != NOPIECE) {
                for (int toX = 0; toX < MAXDIM; toX++) {
                    for (int toY = 0; toY < MAXDIM; toY++) {
                        move theMove(fromX, fromY, toX, toY);
                        if (theBoard->legalMove(currentPlayer, theMove)) {
                            allPossibleMoves.push_back(theMove);
                        }
                        
                    }
                }
                
            } // end if piece is not blank
            
        }
    }
    return allPossibleMoves;
}

move game::computerMove()
{
    board *newBoard = new board(gameBoard);
    move bestMove = minMaxMove(newBoard, 2);
    
	/* while (!gameBoard.legalMove(currentPlayer, theMove)) {
		theMove.fromX = rand() % 5;
		theMove.fromY = rand() % 5;
		theMove.toX = rand() % 5;
		theMove.toY = rand() % 5;
	} */
    
    return bestMove;
}

move game::minMaxMove(board *theBoard, int depth)
{
    if (currentPlayer == WHITE) {
        cout << "\nMAX WHITE MOVES\n\n";
    } else {
        cout << "\nMAX BLACK MOVES\n\n";
    }

	move bestMove = maxMove(theBoard, depth);
    return bestMove;
}

move game::maxMove(board *theBoard, int depth)
{	
    list<move> moves = allPossibleMoves(theBoard);
    move bestMove;
	
	depth--;
	if (depth == 0) {
		return bestMove;
	}
	
    list<move>::const_iterator itr = moves.begin();
	bestMove = *itr;
    for (; itr != moves.end(); itr++) {
        move theMove = *itr;
        
        cout << theMove.toString() 
        << " score: " 
        << moveScore(theBoard, theMove)
        << " legal: "
        << theBoard->legalMove(currentPlayer, theMove)
        << "\n";
		
		//board *newBoard = new board(theBoard);
		//newBoard->movePiece(currentPlayer, theMove);
		//theMove = minMove(newBoard, depth);
        
        if (currentPlayer == WHITE) {
            if (moveScore(theBoard, bestMove) < moveScore(theBoard, theMove)) {
                bestMove = theMove;
				cout << "MAKE MAX BEST MOVE: " << theMove.toString() << "\n";
            }
        } else {
            if (moveScore(theBoard, bestMove) > moveScore(theBoard, theMove)) {
                bestMove = theMove;
            }
        }
		
        //bestMove = maxMove(theBoard, depth);
    }
	
	cout << "\n";
	
    return bestMove;
}


move game::minMove(board *theBoard, int depth)
{
	theBoard->printBoard();
    list<move> moves = allPossibleMoves(theBoard);
    move bestMove;
	
    list<move>::const_iterator itr = moves.begin();
	bestMove = *itr;
    for (; itr != moves.end(); itr++) {
        move theMove = *itr;
        
        cout << theMove.toString() 
        << " score: " 
        << moveScore(theBoard, theMove)
        << " legal: "
        << theBoard->legalMove(currentPlayer, theMove)
        << "\n";
        
        if (currentPlayer == WHITE) {
            if (moveScore(theBoard, bestMove) > moveScore(theBoard, theMove)) {
                bestMove = theMove;
				cout << "MAKE MIN BEST MOVE: " << theMove.toString() << "\n";
            }
        } else {
            if (moveScore(theBoard, bestMove) < moveScore(theBoard, theMove)) {
                bestMove = theMove;
            }
        }
	}
	
	cout << "\n";
	
    return bestMove;
}


int game::moveScore(board *theBoard, move theMove)
{
    int score;
	
    board *newBoard = new board(theBoard);
    if (newBoard->movePiece(currentPlayer, theMove)) {
        score = newBoard->whiteTotal() - newBoard->blackTotal();
    } else {
		return 0;
	}

    return score;
}



