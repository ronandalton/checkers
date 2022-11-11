#include "engine/convert.h"

#include "game/board.h"
#include "game/turn.h"
#include "game/piece.h"
#include "game/position.h"
#include "engine/bitboard.h"


Bitboard convertBoardToBitboard(const Board &board) {
	Bitboard bitboard;

	bitboard.black_pieces = 0;
	bitboard.white_pieces = 0;
	bitboard.king_pieces = 0;

	for (int position = 0; position < 32; position++) {
		Piece piece = board.pieceAt(position);

		if (piece.exists()) {
			U32 mask = 1u << position;

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
