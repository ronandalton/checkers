#include "engine/engine.h"

#include "game/game.h"
#include "game/board.h"
#include "game/move.h"
#include "game/turn.h"
#include "engine/bitboard.h"
#include "engine/convert.h"


Move Engine::findBestMove(const Game &game) {
	// initialize engine internal state
	Bitboard bitboard = convertBoardToBitboard(game.getBoard());
	bool is_whites_turn = (game.getTurn() == Turn::WHITE);

	Move best_move = game.getAvailableMoves().front(); // TODO: replace this with call to negamax function

	return best_move;
}
