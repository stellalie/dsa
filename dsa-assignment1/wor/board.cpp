#include <iostream>
#include <time.h>
#include <stdlib.h>

#include "board.h"
#include "piece.h"

using namespace std;

// print functions ========================================

void board::printRow(int y)
{
    // print a row in board showing pieces at each square
    // also print row numbers
    char row = '0' + y;
    
    cout << row << " |";
    for (int x=0; x<MAXDIM; x++)
    {
       cout << squares[y][x]->getPieceChar() << "|";
    }
    cout << "\n";
}

void board::printMiddleLine()
{
    // print a ---+--- etc line
    cout << "  |-";
    for (int x=0; x<MAXDIM-1; x++)
    {
       cout << "--+-";
    }
    cout << "--|\n";
}

void board::printOuterLine()
{
    // print the top and bottom line of board
    cout << "   -";
    for (int x=0; x<MAXDIM-1; x++)
    {
       cout << "----";
    }
    cout << "--\n";
}

void board::printBottomLine()
{
    // bottom part of board with column letters
    cout << "    ";
    for (int x=0; x<MAXDIM; x++)
    {
       char digit = 'a' + x;
       cout << digit << "   ";
    }
    cout << "\n";
}
    
void board::printBoard()
{
    // print entire board
    cout << "       (black)\n";
    printOuterLine();
    for (int y=0; y<MAXDIM; y++)
    {
        printRow(y);
        if (y < MAXDIM-1) printMiddleLine();
    }
    printOuterLine();
    printBottomLine();
    cout << "        WHITE\n";
}

void board::printScore()
{
	//. print the total of each of the whiet and black pieces
    cout << "White score = " << whiteTotal() << " : Black score = " << blackTotal() << "\n";
}

   
// board constructors and destructor =============================================   

board::board()
{
    // set the entire board to NULL
    for (int y=0; y<MAXDIM; y++)
    {
        for (int x=0; x<MAXDIM; x++)
        {
            squares[y][x] = NULL;
        }
    }
    setupBoard();
}

board::board(board *other)
{
	// copy constructor for board. The new board will be a copy of the other board
    clone(other);
}


board::~board()
{
    // set the entire board to NULL
    clear();
}

void board::setupBoard()
{
    // clean up previous board set up
    clear();
    
    // set middle two rows to have no pieces
    for (int y=1; y<MAXDIM-1; y++)
    {
        for (int x=0; x<MAXDIM; x++)
        {
            squares[y][x] = new noPiece();
        }
    }
    
    // setup white and black sides
    squares[3][0] = new knight(WHITE);
    squares[3][1] = new duke(WHITE);
    squares[3][2] = new bishop(WHITE);
    squares[3][3] = new rook(WHITE);
    
    squares[0][0] = new rook(BLACK);
    squares[0][1] = new bishop(BLACK);
    squares[0][2] = new duke(BLACK);
    squares[0][3] = new knight(BLACK);
}

void board::clear()
{
    // delete all former pieces and set board to NULL
    for (int y=0; y<MAXDIM; y++)
    {
        for (int x=0; x<MAXDIM; x++)
        {
            if (squares[y][x] != NULL) delete(squares[y][x]);
            squares[y][x] = NULL;
        }
    }
}

void board::clone(board *other)
{
    // make this board have duplicate layout to other board;
    clear();
   
    piece *tempPiece;
    
    for (int y=0; y<MAXDIM; y++)
    {
        for (int x=0; x<MAXDIM; x++)
        {
            tempPiece = other->getPiece(x, y);
            switch (tempPiece->getPieceType())
            {
                case NOPIECE:
                    squares[y][x] = new noPiece();
                    break;
                case BISHOP:
                    squares[y][x] = new bishop(tempPiece->getPieceColor());
                    break;
                case KNIGHT:
                    squares[y][x] = new knight(tempPiece->getPieceColor());
                    break;
                case ROOK:
                    squares[y][x] = new rook(tempPiece->getPieceColor());
                    break;
                case DUKE:
                    squares[y][x] = new duke(tempPiece->getPieceColor());
                    break;
                case PRINCE:
                    squares[y][x] = new prince(tempPiece->getPieceColor());
                    break;
                default:
                    squares[y][x] = new noPiece();
                    cout << "Clone of board failed\n";
            }
        }
    }
}

// get functions ====================================================

PieceColor board::getPieceColor(int x, int y)
{
    // return color of piece at square x, y
    return squares[y][x]->getPieceColor();
}

PieceType board::getPieceType(int x, int y)
{
    // return type of piece at square x, y
    return squares[y][x]->getPieceType();
}

piece* board::getPiece(int x, int y)
{
    return squares[y][x];
}

// move functions ===================================================

bool board::insideBoardBoundaries(int xx, int yy)
{
    // determine if xx, yy is properly inside the board area
    if (xx < 0 || xx >= MAXDIM) return false;
    if (yy < 0 || yy >= MAXDIM) return false;
    return true;
}
        
bool board::legalMove(PieceColor playerColor, move theMove)
{
    // ckeck that the moveFrom and moveTo squares are within the board
    if (!insideBoardBoundaries(theMove.fromX, theMove.fromY)) return false;
    if (!insideBoardBoundaries(theMove.toX, theMove.toY)) return false;
    
    // confirm the current player has a piece at moveFrom square
    if (getPieceColor(theMove.fromX, theMove.fromY) != playerColor) return false;
    
    // confirm moveTo square does not contain a piece of same color
    // This also eliminates zero moves. That is, moving from x,y to x,y
    if (getPieceColor(theMove.toX, theMove.toY) == playerColor) return false;
    
    // check the particular piece can do the move
    return getPiece(theMove.fromX, theMove.fromY)->legalMove(this, theMove);
}

bool board::movePiece(PieceColor playerColor, move theMove)
{
    // return true if piece is successfully moved
    
    // check for the legalility of the move
    if (!legalMove(playerColor, theMove)) return false;
    
    // store piece at movefrom square
    piece *tempPiece = getPiece(theMove.fromX, theMove.fromY);
    
    // make the movefrom square empty of pieces
    squares[theMove.fromY][theMove.fromX] = new noPiece();
    
    // see if we have a duke being promoted to prince
    if (tempPiece->isDuke() && ((duke*)tempPiece)->moveMakesPrince(theMove))
    {
        PieceColor pieceColor = tempPiece->getPieceColor();
        delete tempPiece;
        tempPiece = new prince(pieceColor);
    }
    
    // delete the piece at moveto square
    delete(squares[theMove.toY][theMove.toX]);
 
    // set the moveto square to the temp piece
    squares[theMove.toY][theMove.toX] = tempPiece;
    
    return true;
}

// scoring functions ===========================================

int board::whiteTotal()
{
    // add up the values of all remaining white pieces and return
    int numWhite = 0;
    
    for (int y=0; y<MAXDIM; y++)
    {
        for (int x=0; x<MAXDIM; x++) 
        {
            if (getPieceColor(x, y) == WHITE) numWhite += getPieceType(x, y);
        }
    }
    return numWhite;
}

int board::blackTotal()
{
    // add up the values of all remaining black pieces and return
    int numBlack = 0;
    
    for (int y=0; y<MAXDIM; y++)
    {
        for (int x=0; x<MAXDIM; x++) 
        {
            if (getPieceColor(x, y) == BLACK) numBlack += getPieceType(x, y);
        }
    }
    return numBlack;
}


int board::currentScore(PieceColor playerColor)
{
    // add up the values of all remaining black pieces and return
	// a +'ve score for the playerColor means they are ahead, other wise they are behind

    int total = 0;
    
    for (int y=0; y<MAXDIM; y++)
    {
        for (int x=0; x<MAXDIM; x++) 
        {
            if (getPieceColor(x, y) != BLANK)
            {
                if (getPieceColor(x, y) == WHITE) total += getPieceType(x, y);
                else total -= getPieceType(x, y);
            }
        }
    }
    
    if (playerColor == WHITE) return total;
    else return -total;
}

int board::numWhite()
{
    // return the number of white pieces remaining
    int num= 0;
    
    for (int y=0; y<MAXDIM; y++)
    {
        for (int x=0; x<MAXDIM; x++) 
        {
            if (getPieceColor(x, y) == WHITE) num++;
        }
    }
    return num;
}

int board::numBlack()
{
    // return the number of black pieces remaining
    int num = 0;
    
    for (int y=0; y<MAXDIM; y++)
    {
        for (int x=0; x<MAXDIM; x++) 
        {
            if (getPieceColor(x, y) == BLACK) num++;
        }
    }
    return num;
}

int board::numPieces(PieceColor playerColor)
{
    if (playerColor == WHITE) return numWhite();
    else if (playerColor == BLACK) return numBlack();
    else return 0;
}
