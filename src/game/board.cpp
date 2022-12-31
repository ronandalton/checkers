#include "game/board.h"

#include "game/position.h"
#include "game/coord.h"
#include "game/movegen.h"

#include <algorithm> // for std::equal
#include <iterator> // for std::begin and std::end


Piece& Board::pieceAt(Position position) {
	return m_squares[position];
}


const Piece& Board::pieceAt(Position position) const {
	return m_squares[position];
}


bool Board::operator==(const Board &board) const {
	return std::equal(std::begin(m_squares), std::end(m_squares), std::begin(board.m_squares));
}


void Board::doMove(const Move &move) {
	*this = MoveGen::getBoardAfterMove(*this, move);
}


void Board::movePiece(Coord from_square, Coord to_square) {
	Position start_position(from_square.getPosition());
	Position end_position(to_square.getPosition());

	*this = MoveGen::getBoardAfterSimpleMove(*this, start_position, end_position);
}
