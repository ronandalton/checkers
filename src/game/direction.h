#ifndef DIRECTION_H
#define DIRECTION_H


class Coord;


class Direction {
public:
	enum Type {
		UP_LEFT,
		UP_RIGHT,
		DOWN_RIGHT,
		DOWN_LEFT,
	};
	
	static const Direction ALL_DIRECTIONS[4];
	
	Direction() = default;
	Direction(Type type);
	
	bool operator==(const Direction &direction) const;
	Direction getReverse() const; // not used
	bool isUpwards() const;
	bool isDownwards() const;
	Coord getOffset() const;
	
private:
	Type m_type {};
};


#endif
