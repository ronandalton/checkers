#ifndef ENGINE_H
#define ENGINE_H


#include "engine/bitboard.h"
#include "engine/hash.h"
#include "engine/hash_table.h"
#include "engine/bitstring_list.h"
#include "engine/move_type.h"

class Game;
class Move;


class Engine {
public:
	Move findBestMove(const Game &game);

private:
	void doMove(const move_t &move);
	int negamax(int depth, int alpha, int beta);

	Bitboard m_bitboard;
	bool m_is_whites_turn;
	hash_t m_hash;
	HashTable m_hash_table;
	BitstringList m_bitstring_list;
};


#endif
