#ifndef MOVE_H
#define MOVE_H


#include "game/position.h"

#include <vector>


class Move {
public:
	Move(const std::vector<Position> &positions = {}, const std::vector<Position> &jumped_piece_positions = {});
	Move(Position position);

	void addPosition(Position position);
	void addJumpedPiecePosition(Position position);
	
	bool operator==(const Move &move) const;
	int getLength() const;
	Position getPosition(int index) const;
	const std::vector<Position>& getPositions() const; // not used
	const std::vector<Position>& getJumpedPiecePositions() const;
	Position getStartPosition() const;
	Position getEndPosition() const;
	bool isJump() const;
	bool exists() const;

private:
	std::vector<Position> m_positions;
	std::vector<Position> m_jumped_piece_positions;
};


#endif
