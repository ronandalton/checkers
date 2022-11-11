#include "game/position.h"

#include "game/coord.h"
#include "game/direction.h"
#include "game/turn.h"

#include <cassert>


const Position Position::ALL_POSITIONS[32] = {
	 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
	10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
	20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
	30, 31,
};


bool Position::isValidIndex(int index) {
	return index >= 0 && index < 32;
}


Position::Position(int index) :
	m_index {index}
{
	assert(isValidIndex(index));
}


int Position::getX() const {
	return getCoord().getX();
}


int Position::getY() const {
	return getCoord().getY();
}


Coord Position::getCoord() const {
	int y = m_index / 4;
	int x = m_index % 4 * 2 + 1 - y % 2;
	
	return {x, y};
}


Position::operator int() const {
	return m_index;
}


// returns true if the position is on the crowning row for turn
bool Position::isCrowningSquare(Turn turn) const {
	if (turn == Turn::BLACK) {
		return getY() == 7;
	} else {
		return getY() == 0;
	}
}


// returns true if applying an offset (double if specified) in direction would be valid
bool Position::offsetIsValid(Direction direction, bool is_double_offset) const {
	return getCoord().applyOffset(direction, is_double_offset).isValid();
}


// gets the position after applying an offset (double if specified) in direction
// it is assumed that the offset is valid
Position Position::getOffset(Direction direction, bool is_double_offset) const {
	return getCoord().applyOffset(direction, is_double_offset).getPosition();
}
