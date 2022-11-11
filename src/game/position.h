#ifndef POSITION_H
#define POSITION_H


class Coord;
class Direction;
enum class Turn;


class Position {
public:
	static const Position ALL_POSITIONS[32];
	static bool isValidIndex(int index);
	
	Position() = default;
	Position(int index);
	
	int getX() const;
	int getY() const;
	Coord getCoord() const;
	operator int() const;
	bool isCrowningSquare(Turn turn) const;
	bool offsetIsValid(Direction direction, bool is_double_offset) const;
	Position getOffset(Direction direction, bool is_double_offset) const;
	
private:
	int m_index {};
};


#endif
