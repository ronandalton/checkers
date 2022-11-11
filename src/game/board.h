#ifndef BOARD_H
#define BOARD_H


class Position;

#include "game/piece.h"


class Board {
public:
	Piece& pieceAt(Position position);
	const Piece& pieceAt(Position position) const;
	bool operator==(const Board &board) const;

private:
	Piece m_squares[32] {};
};


#endif
