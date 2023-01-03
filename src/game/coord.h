#ifndef COORD_H
#define COORD_H


class Position;
class Direction;


class Coord {
public:
	Coord() = default;
	Coord(int x, int y);
	
	Coord& applyOffset(Direction direction, bool is_double_offset);
	
	int getX() const;
	int getY() const;
	bool isValidPosition() const;
	Position getPosition() const;
	bool operator==(const Coord &coord) const;
	
private:
	int m_x;
	int m_y;
};


#endif
