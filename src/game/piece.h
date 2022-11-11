#ifndef PIECE_H
#define PIECE_H


enum class Turn;
class Direction;


class Piece {
public:
	enum Type {
		NONE,
		BLACK_MAN,
		WHITE_MAN,
		BLACK_KING,
		WHITE_KING,
	};

	Piece() = default;
	Piece(Type type);
	
	Piece& crown();
	Piece& remove();
	
	bool operator==(const Piece &piece) const;
	bool exists() const;
	bool belongsTo(Turn turn) const;
	Turn getTurn() const;
	Turn getOppositeTurn() const;
	bool isCrowned() const;
	bool canMoveInDirection(Direction direction) const;

private:
	Type m_type {};
};


#endif
