#include "game/piece.h"

#include "game/turn.h"
#include "game/direction.h"

#include <cassert>


Piece::Piece(Type type) :
	m_type {type}
{
}


Piece& Piece::crown() {
	if (m_type == BLACK_MAN) {
		m_type = BLACK_KING;
	} else if (m_type == WHITE_MAN) {
		m_type = WHITE_KING;
	}
	
	return *this;
}


Piece& Piece::remove() {
	m_type = NONE;
	
	return *this;
}


bool Piece::operator==(const Piece &piece) const {
	return m_type == piece.m_type;
}


bool Piece::exists() const {
	return m_type != NONE;
}


bool Piece::belongsTo(Turn turn) const {
	if (turn == Turn::BLACK) {
		return m_type == BLACK_MAN || m_type == BLACK_KING;
	} else {
		return m_type == WHITE_MAN || m_type == WHITE_KING;
	}
}


// returns the turn that this piece belongs to
// the piece should exist
Turn Piece::getTurn() const {
	assert(exists());
	return belongsTo(Turn::BLACK) ? Turn::BLACK : Turn::WHITE;
}


// returns the opposite of getTurn()
Turn Piece::getOppositeTurn() const {
	return getTurn() == Turn::BLACK ? Turn::WHITE : Turn::BLACK;
}


bool Piece::isCrowned() const {
	return m_type == BLACK_KING || m_type == WHITE_KING;
}


// returns true if the piece can move in the direction specified
// ie. pieces that aren't kings can only move one way (up or down)
bool Piece::canMoveInDirection(Direction direction) const {
	if (m_type == BLACK_KING || m_type == WHITE_KING) {
		return true;
	} else if (m_type == BLACK_MAN) {
		return direction.isDownwards();
	} else if (m_type == WHITE_MAN) {
		return direction.isUpwards();
	} else {
		return false;
	}
}
