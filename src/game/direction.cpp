#include "game/direction.h"

#include "game/coord.h"


const Direction Direction::ALL_DIRECTIONS[4] = {
	UP_LEFT,
	UP_RIGHT,
	DOWN_RIGHT,
	DOWN_LEFT,
};


Direction::Direction(Type type) :
	m_type {type}
{
}


bool Direction::operator==(const Direction &direction) const {
	return m_type == direction.m_type;
}


Direction Direction::getReverse() const {
	switch (m_type) {
		case UP_LEFT:
			return DOWN_RIGHT;
		case UP_RIGHT:
			return DOWN_LEFT;
		case DOWN_LEFT:
			return UP_RIGHT;
		default: // DOWN_RIGHT
			return UP_LEFT;
	}
}


bool Direction::isUpwards() const {
	return m_type == UP_LEFT || m_type == UP_RIGHT;
}


bool Direction::isDownwards() const {
	return m_type == DOWN_LEFT || m_type == DOWN_RIGHT;
}


// gets the coord that can be added to another coord to offset it in this direction by one square
Coord Direction::getOffset() const {
	switch (m_type) {
		case UP_LEFT:
			return {-1, -1};
		case UP_RIGHT:
			return {1, -1};
		case DOWN_LEFT:
			return {-1, 1};
		default: // DOWN_RIGHT
			return {1, 1};
	}
}
