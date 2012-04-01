#ifndef _PIECE_H
#define _PIECE_H

#include "bintree.h"
#include "constant.h"
#include "move.h"

class ply
{
    int score;
    move theMove;
    bintree<ply> plies;
    
    ply() {}
    
};

#endif

