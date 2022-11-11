#include "engine/hash.h"

#include "engine/bitboard.h"
#include "engine/bitstring_list.h"
#include "engine/piece_codes.h"
#include "engine/bitboard_access.h"


hash_t getHash(const Bitboard &bitboard, bool is_whites_turn, const BitstringList &bitstring_list) {
	hash_t hash = 0;

	for (int position = 0; position < 32; position++) {
		int piece = getPieceAtPosition(bitboard, position);

		if (piece != PIECE_NONE) {
			hash ^= bitstring_list.getBitstringForPieceSquare(position, piece);
		}
	}

	if (is_whites_turn) {
		hash ^= bitstring_list.getBitstringForWhitesTurn();
	}

	return hash;
}
