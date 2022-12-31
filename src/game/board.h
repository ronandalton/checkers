#ifndef BOARD_H
#define BOARD_H


class Position;
class Coord;
class Move;

#include "game/piece.h"


class Board {
public:
	Piece& pieceAt(Position position);
	const Piece& pieceAt(Position position) const;
	bool operator==(const Board &board) const;

	void doMove(const Move &move);
	void movePiece(Coord from_square, Coord to_square);

private:
	Piece m_squares[32] {};
};


#endif
