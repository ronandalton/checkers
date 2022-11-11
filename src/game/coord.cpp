#include "game/coord.h"

#include "game/position.h"
#include "game/direction.h"

#include <cassert>


Coord::Coord(int x, int y) :
	m_x {x},
	m_y {y}
{
}


// apply a shift in a direction and make it a double shift if specified
Coord& Coord::applyOffset(Direction direction, bool is_double_offset) {
	Coord offset = direction.getOffset();
	
	if (is_double_offset) {
		offset.m_x *= 2;
		offset.m_y *= 2;
	}
	
	m_x += offset.m_x;
	m_y += offset.m_y;
	
	return *this;
}


int Coord::getX() const {
	return m_x;
}


int Coord::getY() const {
	return m_y;
}


// returns true if coord is a valid position (on a playable square)
bool Coord::isValid() const {
	if (m_x >= 0 && m_x < 8 && m_y >= 0 && m_y < 8) {
		if ((m_x + m_y) % 2 == 1) {
			return true;
		}
	}
	
	return false;
}


// converts the coord to a position
// the coord should be a valid position
Position Coord::getPosition() const {
	assert(isValid());
	
	return {m_y * 4 + (m_x + m_y % 2 - 1) / 2};
}


bool Coord::operator==(const Coord &coord) const {
	return m_x == coord.m_x && m_y == coord.m_y;
}
