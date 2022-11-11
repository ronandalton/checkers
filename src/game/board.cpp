#include "game/board.h"

#include "game/position.h"

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
