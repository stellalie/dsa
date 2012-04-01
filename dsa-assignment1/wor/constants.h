
#ifndef _CONSTANTS_H
#define _CONSTANTS_H

// maximum size of board side
const int MAXDIM = 4;
// maximum number of moves per side
const int MAXMOVES = 20;
// maximum number of chars player can enter at keyboard
const int ENTRYMAX = 10;

// enumerated colors of pieces. BLANK will be used for noPiece type color
enum PieceColor
{
    BLANK = 0,
    WHITE = 1,
    BLACK = 2
};

// enumerated cosntants for pieces, including NOPIECE for noPiece type
enum PieceType 
{ 
    NOPIECE = 0,
    BISHOP = 3, 
    KNIGHT = 4,
    ROOK = 6,
    DUKE = 7,
    PRINCE = 9
};

#endif
