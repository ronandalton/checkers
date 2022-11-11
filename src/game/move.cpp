#include "game/move.h"

#include <cassert>


Move::Move(const std::vector<Position> &positions, const std::vector<Position> &jumped_piece_positions) :
	m_positions {positions},
	m_jumped_piece_positions {jumped_piece_positions}
{
}


Move::Move(Position position) :
	m_positions {position}
{
}


void Move::addPosition(Position position) {
	m_positions.push_back(position);
}


void Move::addJumpedPiecePosition(Position position) {
	m_jumped_piece_positions.push_back(position);
}


bool Move::operator==(const Move &move) const {
	return m_positions == move.m_positions && m_jumped_piece_positions == move.m_jumped_piece_positions;
}


int Move::getLength() const {
	return m_positions.size();
}


Position Move::getPosition(int index) const {
	assert(index >= 0 && index < static_cast<int>(m_positions.size()));
	
	return m_positions[index];
}


const std::vector<Position>& Move::getPositions() const {
	return m_positions;
}


const std::vector<Position>& Move::getJumpedPiecePositions() const {
	return m_jumped_piece_positions;
}


Position Move::getStartPosition() const {
	assert(m_positions.empty() == false);
	
	return m_positions.front();
}


Position Move::getEndPosition() const {
	assert(m_positions.empty() == false);
	
	return m_positions.back();
}


bool Move::isJump() const {
	return m_jumped_piece_positions.empty() == false;
}


bool Move::exists() const {
	return m_positions.empty() == false;
}
