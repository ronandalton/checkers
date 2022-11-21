#include "engine/engine.h"

#include "game/game.h"
#include "game/board.h"
#include "game/move.h"
#include "game/turn.h"
#include "game/piece.h"
#include "game/position.h"
#include "engine/bitboard.h"


static Bitboard convertBoardToBitboard(const Board &board) {
	Bitboard bitboard;

	bitboard.black_pieces = 0;
	bitboard.white_pieces = 0;
	bitboard.king_pieces = 0;

	for (int position = 0; position < 32; position++) {
		Piece piece = board.pieceAt(position);

		if (piece.exists()) {
			u32 mask = 1u << position;

			if (piece.belongsTo(Turn::BLACK)) {
				bitboard.black_pieces |= mask;
			} else {
				bitboard.white_pieces |= mask;
			}

			if (piece.isCrowned()) {
				bitboard.king_pieces |= mask;
			}
		}
	}

	return bitboard;
}


Move Engine::findBestMove(const Game &game) {
	// initialize engine internal state
	Bitboard bitboard = convertBoardToBitboard(game.getBoard());
	bool is_whites_turn = (game.getTurn() == Turn::WHITE);

	Move best_move = game.getAvailableMoves().front(); // TODO: replace this with call to negamax function

	return best_move;
}
