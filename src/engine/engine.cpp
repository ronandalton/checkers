#include "engine/engine.h"

#include "game/game.h"
#include "game/board.h"
#include "game/move.h"
#include "game/turn.h"
#include "engine/convert.h"


Move Engine::findBestMove(const Game &game) {
	// initialize engine internal state
	m_bitboard = convertBoardToBitboard(game.getBoard());
	m_is_whites_turn = game.getTurn() == Turn::WHITE ? true : false;
	m_hash = getHash(m_bitboard, m_is_whites_turn, m_bitstring_list);

	Move best_move = game.getAvailableMoves().front(); // replace this with call to negamax function

	return best_move;
}


void Engine::doMove(const move_t &move) {
	// for each piece-square in move:
	// add/remove that piece from the board
	// xor in hash for that piece-square
}


int Engine::negamax(int depth, int alpha, int beta) {

}
