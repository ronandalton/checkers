#ifndef ENGINE_H
#define ENGINE_H


class Game;
class Move;
struct Bitboard;
class CompactMove;


class Engine {
public:
	Move findBestMove(const Game &game);

private:
	int negamax(const Bitboard &board, bool is_whites_turn, int depth, int alpha, int beta, CompactMove *best_move);

	static int evaluate(const Bitboard &board);

	static constexpr int MAX_DEPTH = 9;
};


#endif
