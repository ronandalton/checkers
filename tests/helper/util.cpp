#include "util.h"

#include "game/board.h"
#include "game/piece.h"
#include "game/move.h"

#include <algorithm> // for std::is_permutation


// constructs a board from a string representation
// characters other than "_bwBW" are ignored
// pieces are listed in ascending order starting at index zero
Board Util::createBoard(const std::string &input) {
	Board board;

	int index = 0;

	for (char character : input) {
		Piece piece;

		if (character == '_') {
			piece = Piece::NONE;
		} else if (character == 'b') {
			piece = Piece::BLACK_MAN;
		} else if (character == 'w') {
			piece = Piece::WHITE_MAN;
		} else if (character == 'B') {
			piece = Piece::BLACK_KING;
		} else if (character == 'W') {
			piece = Piece::WHITE_KING;
		} else { // not a character representing a piece type
			continue;
		}

		board.pieceAt(index) = piece;
		index++;
	}
	
	return board;
}


// returns true if both move lists have the same moves in them (in any order)
bool Util::moveListsAreEquivalent(const std::vector<Move> &list_1, const std::vector<Move> &list_2) {
	return std::is_permutation(list_1.begin(), list_1.end(), list_2.begin(), list_2.end());
}
