#ifndef MOVEGEN_H
#define MOVEGEN_H


class Board;
enum class Turn;
class Move;
class Position;
class Direction;

#include <vector>


class MoveGen {
public:
	MoveGen() = delete;
	
	static std::vector<Move> generateMoves(const Board &board, Turn turn);
	static Board getBoardAfterMove(Board board, const Move &move);
	
private:
	static bool moveIsLegal(const Board &board, Position position, Direction direction, bool jumping);
	static Move getMove(Position position, Direction direction, bool jumping);
	static bool moveCrownsPiece(const Board &board, const Move &move);
	static std::vector<Move> findNormalMoves(const Board &board, Position position);
	static std::vector<Move> findJumpingMoves(const Board &board, const Move &partial_move);
};


#endif
