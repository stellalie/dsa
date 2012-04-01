
#include <iostream>
#include <string>

#include "constants.h"
#include "game.h"

using namespace std;

// code =====================================================

int main(int argc, char *argv[])
{
    game theGame;
    PieceColor computerPlayer;
    
    if (argc != 2) {
        cout << "Must supply 1 argument to this program\n";
        return 0;
    }
    
    string s;
    s = argv[1];
    
    if (s == "-w") {
        computerPlayer = WHITE;        
    } else if (s == "-b") {
        computerPlayer = BLACK;
    } else {
        cout << "Invalid argument to this program\n";
        return 0;
    }
    
    theGame.setComputer(computerPlayer);
    theGame.printMenu();
    theGame.printBoard();
    
    while (!theGame.ended())
    {
	   theGame.nextMove();
    }
    theGame.printWinner();
    
    return 0;
}
