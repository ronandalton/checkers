#include "gtest/gtest.h"

#include "game/board.h"
#include "game/piece.h"
#include "game/position.h"


TEST(game_board_constructor, testInitializedEmpty) {
	Board board;

	for (Position position : Position::ALL_POSITIONS) {
		EXPECT_EQ(board.pieceAt(position), Piece::NONE);
	}
}


TEST(game_board_pieceAt, testAssignment) {
	Board board;

	board.pieceAt(5) = Piece::BLACK_MAN;
	EXPECT_EQ(board.pieceAt(5), Piece::BLACK_MAN);

	board.pieceAt(26) = Piece::WHITE_KING;
	EXPECT_EQ(board.pieceAt(26), Piece::WHITE_KING);
}
