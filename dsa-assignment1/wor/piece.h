#ifndef _PIECE_H
#define _PIECE_H

#include "constants.h"
#include "move.h"

// forward declaration
class board;

/*********************************************************\
   WARNING
    This code makes use of polymorphism and inheritance.
    Do NOT use such concepts in assignments 2 and 3.
   WARNING
\*********************************************************/
   
// these objects manage the base piece class and the derived pieces of 
// knight, boshop, rook, duke and prince

class piece
{
	// you cannot create a piece. Instead, you create derived objects such as rooks, etc
	// which are pieces.

    protected:
        PieceColor pieceColor;
        bool interveningSquaresEmpty(board *theBoard, move theMove);
    
        
    public:
        piece();
        piece(PieceColor c);
        virtual ~piece();
        
        // pure virtual functions
        virtual bool legalMove(board *theBoard, move theMove) = 0;
        virtual PieceType getPieceType() = 0;      
        virtual const char* getPieceChar() = 0;
    
        inline PieceColor getPieceColor() { return pieceColor; }
        
        static PieceColor otherColor(PieceColor aColor)
        {
            if (aColor == BLACK) return WHITE;
            else if (aColor == WHITE) return BLACK;
            else return aColor;
        }
        
        bool isKnight() { return getPieceType() == KNIGHT; } 
        bool isBishop() { return getPieceType() == BISHOP; } 
        bool isRook() { return getPieceType() == ROOK; } 
        bool isDuke() { return getPieceType() == DUKE; } 
        bool isPrince() { return getPieceType() == PRINCE; } 
        bool isNoPiece() { return getPieceType() == NOPIECE; }
};

class knight : public piece //------------------------------
{
	public :
		knight(PieceColor c) : piece (c)
        {
        }
        
        bool legalMove(board *theBoard, move theMove);
		
		inline PieceType getPieceType()
		{
		   return KNIGHT;
		}
		
		const char* getPieceChar();
};

class bishop : public piece //------------------------------
{
	public:
		bishop(PieceColor c) : piece (c)
        {
        }
        
        bool legalMove(board *theBoard, move theMove);
		
		const char* getPieceChar();
		
		inline PieceType getPieceType()
		{
		   return BISHOP;
		}
};

class rook : public piece //------------------------------
{
	public:
		rook(PieceColor c) : piece (c)
        {
        }
        
        bool legalMove(board *theBoard, move theMove);
		
		const char* getPieceChar();
		
		inline PieceType getPieceType()
		{
		   return ROOK;
		}
};

class duke : public piece //------------------------------
{
	public:
        duke(PieceColor c) : piece (c)
        {
        }
        
		bool legalMove(board *theBoard, move theMove);
		
		const char* getPieceChar();
		
		inline PieceType getPieceType()
		{
	   		return DUKE;
		}
        
        bool moveMakesPrince(move theMove);
};

class prince : public piece //------------------------------
{
	public:
        prince(PieceColor c) : piece (c)
        {
        }
        
		bool legalMove(board *theBoard, move theMove);
		
		const char* getPieceChar();
		
		inline PieceType getPieceType()
		{
	   		return PRINCE;
		}
};

class noPiece : public piece //------------------------------
{
	// The piece you are having when you are not having a piece. 
	// This is a pattern object that simplifies some of our code

	public:
		noPiece() : piece(BLANK)
		{
		}
		
		inline bool legalMove(board *theBoard, move theMove)
		{
		   return false;
		}
		
		inline const char* getPieceChar()
		{
		   return "   ";
		}   
		
		inline PieceType getPieceType()
		{
		   return NOPIECE;
		} 
};

#endif

