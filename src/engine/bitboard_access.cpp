#include "engine/bitboard_access.h"

#include "engine/bitboard.h"
#include "engine/piece_codes.h"


int getPieceAtPosition(const Bitboard &bitboard, int position) {
	U32 mask = 1u << position;

	if (bitboard.black_pieces & mask) {
		if (bitboard.king_pieces & mask) {
			return PIECE_BLACK_KING;
		} else {
			return PIECE_BLACK_MAN;
		}
	} else if (bitboard.white_pieces & mask) {
		if (bitboard.king_pieces & mask) {
			return PIECE_WHITE_KING;
		} else {
			return PIECE_WHITE_MAN;
		}
	} else {
		return PIECE_NONE;
	}
}
