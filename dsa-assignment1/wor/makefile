CC = g++
prog: war_of_roses.o game.o board.o piece.o 
	$(CC) war_of_roses.o game.o board.o piece.o -Wall -Werror -o wor
war_of_roses.o: war_of_roses.cpp game.h
	$(CC) -Wall -Werror -c war_of_roses.cpp
game.o: game.cpp game.h board.h piece.h move.h
	$(CC) -Wall -Werror -c game.cpp
board.o: board.cpp board.h piece.h move.h
	$(CC) -Wall -Werror -c board.cpp
piece.o: piece.cpp piece.h board.h move.h
	$(CC) -Wall -Werror -c piece.cpp
