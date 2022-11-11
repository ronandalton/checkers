#include "gtest/gtest.h"
#include "util.h"

#include "game/movegen.h"
#include "game/board.h"
#include "game/move.h"
#include "game/turn.h"

#include <vector>


TEST(game_movegen_generateMoves, testStartPosition) {
	Board board = Util::createBoard(
		"     -----------------     "
		" 00 |   b   b   b   b | 03 "
		" 04 | b   b   b   b   | 07 "
		" 08 |   b   b   b   b | 11 "
		" 12 | _   _   _   _   | 15 "
		" 16 |   _   _   _   _ | 19 "
		" 20 | w   w   w   w   | 23 "
		" 24 |   w   w   w   w | 27 "
		" 28 | w   w   w   w   | 31 "
		"     -----------------     "
	);

	Turn turn = Turn::BLACK;

	std::vector<Move> expected_moves = {
		{{8, 12}},
		{{8, 13}},
		{{9, 13}},
		{{9, 14}},
		{{10, 14}},
		{{10, 15}},
		{{11, 15}},
	};

	std::vector<Move> output_moves = MoveGen::generateMoves(board, turn);

	EXPECT_TRUE(Util::moveListsAreEquivalent(expected_moves, output_moves));
}


TEST(game_movegen_generateMoves, testNoPiecesForPlayerToMove) {
	Board board = Util::createBoard(
		"     -----------------     "
		" 00 |   _   _   _   b | 03 "
		" 04 | _   _   _   _   | 07 "
		" 08 |   _   _   _   _ | 11 "
		" 12 | _   _   _   _   | 15 "
		" 16 |   _   _   _   _ | 19 "
		" 20 | _   _   _   B   | 23 "
		" 24 |   _   _   _   _ | 27 "
		" 28 | _   _   _   B   | 31 "
		"     -----------------     "
	);

	Turn turn = Turn::WHITE;

	std::vector<Move> expected_moves = {};

	std::vector<Move> output_moves = MoveGen::generateMoves(board, turn);

	EXPECT_TRUE(Util::moveListsAreEquivalent(expected_moves, output_moves));
}


TEST(game_movegen_generateMoves, testMaxDoubleJumpLengthPosition) {
	Board board = Util::createBoard(
		"     -----------------     "
		" 00 |   _   _   _   _ | 03 "
		" 04 | _   w   w   w   | 07 "
		" 08 |   _   _   _   _ | 11 "
		" 12 | _   w   w   w   | 15 "
		" 16 |   _   _   _   _ | 19 "
		" 20 | _   w   w   w   | 23 "
		" 24 |   B   _   _   _ | 27 "
		" 28 | _   _   _   _   | 31 "
		"     -----------------     "
	);

	Turn turn = Turn::BLACK;

	std::vector<Move> expected_moves = {
		{{24, 17, 10, 3}, {21, 14, 7}}, // straight through skipping others
		{{24, 17, 8, 1, 10, 3}, {21, 13, 5, 6, 7}}, // around left skipping others
		{{24, 17, 26, 19, 10, 3}, {21, 22, 23, 15, 7}}, // around right skipping others
		{{24, 17, 8, 1, 10, 17, 26, 19, 10, 3}, {21, 13, 5, 6, 14, 22, 23, 15, 7}}, // around left then loop back
		{{24, 17, 26, 19, 10, 17, 8, 1, 10, 3}, {21, 22, 23, 15, 14, 13, 5, 6, 7}}, // around right then loop back
		{{24, 17, 8, 1, 10, 19, 26, 17, 10, 3}, {21, 13, 5, 6, 15, 23, 22, 14, 7}}, // around left then continue loop
		{{24, 17, 26, 19, 10, 1, 8, 17, 10, 3}, {21, 22, 23, 15, 6, 5, 13, 14, 7}}, // around right then continue loop
		{{24, 17, 10, 1, 8, 17, 26, 19, 10, 3}, {21, 14, 6, 5, 13, 22, 23, 15, 7}}, // straight through and around left
		{{24, 17, 10, 19, 26, 17, 8, 1, 10, 3}, {21, 14, 15, 23, 22, 13, 5, 6, 7}}, // straight through and around right
	};

	std::vector<Move> output_moves = MoveGen::generateMoves(board, turn);

	EXPECT_TRUE(Util::moveListsAreEquivalent(expected_moves, output_moves));
}


TEST(game_movegen_generateMoves, testMoveEndsOnCrown) {
	Board board = Util::createBoard(
		"     -----------------     "
		" 00 |   _   _   _   _ | 03 "
		" 04 | _   b   b   b   | 07 "
		" 08 |   w   _   _   _ | 11 "
		" 12 | _   _   _   B   | 15 "
		" 16 |   _   _   w   _ | 19 "
		" 20 | _   _   b   _   | 23 "
		" 24 |   _   _   _   _ | 27 "
		" 28 | _   _   _   _   | 31 "
		"     -----------------     "
	);

	Turn turn = Turn::WHITE;

	std::vector<Move> expected_moves = {
		{{8, 1}, {5}},
		{{18, 11, 2}, {15, 7}},
	};

	std::vector<Move> output_moves = MoveGen::generateMoves(board, turn);

	EXPECT_TRUE(Util::moveListsAreEquivalent(expected_moves, output_moves));
}


TEST(game_movegen_generateMoves, testMultipleMovesJumpingSamePieces) {
	Board board = Util::createBoard(
		"     -----------------     "
		" 00 |   _   _   _   _ | 03 "
		" 04 | _   b   _   w   | 07 "
		" 08 |   b   _   _   b | 11 "
		" 12 | _   b   B   _   | 15 "
		" 16 |   _   w   w   _ | 19 "
		" 20 | _   _   _   _   | 23 "
		" 24 |   _   w   w   _ | 27 "
		" 28 | _   _   _   _   | 31 "
		"     -----------------     "
	);

	Turn turn = Turn::BLACK;

	std::vector<Move> expected_moves = {
		{{14, 21, 30, 23, 14}, {17, 25, 26, 18}},
		{{14, 23, 30, 21, 14}, {18, 26, 25, 17}},
		{{13, 22, 29}, {17, 25}},
		{{13, 22, 31}, {17, 26}},
	};

	std::vector<Move> output_moves = MoveGen::generateMoves(board, turn);

	EXPECT_TRUE(Util::moveListsAreEquivalent(expected_moves, output_moves));
}


TEST(game_movegen_generateMoves, testHasPiecesButNoMoves) {
	Board board = Util::createBoard(
		"     -----------------     "
		" 00 |   b   _   _   _ | 03 "
		" 04 | w   _   _   _   | 07 "
		" 08 |   _   _   b   _ | 11 "
		" 12 | _   _   b   b   | 15 "
		" 16 |   _   _   B   w | 19 "
		" 20 | _   B   _   w   | 23 "
		" 24 |   b   _   _   w | 27 "
		" 28 | w   _   _   _   | 31 "
		"     -----------------     "
	);

	Turn turn = Turn::WHITE;

	std::vector<Move> expected_moves = {};

	std::vector<Move> output_moves = MoveGen::generateMoves(board, turn);

	EXPECT_TRUE(Util::moveListsAreEquivalent(expected_moves, output_moves));
}


TEST(game_movegen_getBoardAfterMove, testSimpleMove) {
	Board board = Util::createBoard(
		"     -----------------     "
		" 00 |   b   b   b   b | 03 "
		" 04 | b   b   b   b   | 07 "
		" 08 |   b   b   b   b | 11 "
		" 12 | _   _   _   _   | 15 "
		" 16 |   _   _   _   _ | 19 "
		" 20 | w   w   w   w   | 23 "
		" 24 |   w   w   w   w | 27 "
		" 28 | w   w   w   w   | 31 "
		"     -----------------     "
	);

	Move move = {{20, 16}};

	Board expected_board = Util::createBoard(
		"     -----------------     "
		" 00 |   b   b   b   b | 03 "
		" 04 | b   b   b   b   | 07 "
		" 08 |   b   b   b   b | 11 "
		" 12 | _   _   _   _   | 15 "
		" 16 |   w   _   _   _ | 19 "
		" 20 | _   w   w   w   | 23 "
		" 24 |   w   w   w   w | 27 "
		" 28 | w   w   w   w   | 31 "
		"     -----------------     "
	);

	Board actual_board = MoveGen::getBoardAfterMove(board, move);

	EXPECT_EQ(expected_board, actual_board);
}


TEST(game_movegen_getBoardAfterMove, testDoubleJumpMoveAndCrown) {
	Board board = Util::createBoard(
		"     -----------------     "
		" 00 |   b   _   _   W | 03 "
		" 04 | _   b   _   _   | 07 "
		" 08 |   _   _   _   _ | 11 "
		" 12 | _   b   _   b   | 15 "
		" 16 |   _   w   _   _ | 19 "
		" 20 | _   _   _   _   | 23 "
		" 24 |   _   _   B   _ | 27 "
		" 28 | _   _   _   _   | 31 "
		"     -----------------     "
	);

	Move move = {{17, 8, 1}, {13, 5}};

	Board expected_board = Util::createBoard(
		"     -----------------     "
		" 00 |   b   W   _   W | 03 "
		" 04 | _   _   _   _   | 07 "
		" 08 |   _   _   _   _ | 11 "
		" 12 | _   _   _   b   | 15 "
		" 16 |   _   _   _   _ | 19 "
		" 20 | _   _   _   _   | 23 "
		" 24 |   _   _   B   _ | 27 "
		" 28 | _   _   _   _   | 31 "
		"     -----------------     "
	);

	Board actual_board = MoveGen::getBoardAfterMove(board, move);

	EXPECT_EQ(expected_board, actual_board);
}


TEST(game_movegen_getBoardAfterMove, testCircularMultiJumpMove) {
	Board board = Util::createBoard(
		"     -----------------     "
		" 00 |   _   _   _   b | 03 "
		" 04 | b   w   W   _   | 07 "
		" 08 |   _   _   _   _ | 11 "
		" 12 | _   w   _   w   | 15 "
		" 16 |   _   _   _   _ | 19 "
		" 20 | _   _   w   w   | 23 "
		" 24 |   _   _   B   _ | 27 "
		" 28 | _   _   _   _   | 31 "
		"     -----------------     "
	);

	Move move = {{26, 19, 10, 1, 8, 17, 26}, {23, 15, 6, 5, 13, 22}};

	Board expected_board = Util::createBoard(
		"     -----------------     "
		" 00 |   _   _   _   b | 03 "
		" 04 | b   _   _   _   | 07 "
		" 08 |   _   _   _   _ | 11 "
		" 12 | _   _   _   _   | 15 "
		" 16 |   _   _   _   _ | 19 "
		" 20 | _   _   _   _   | 23 "
		" 24 |   _   _   B   _ | 27 "
		" 28 | _   _   _   _   | 31 "
		"     -----------------     "
	);

	Board actual_board = MoveGen::getBoardAfterMove(board, move);

	EXPECT_EQ(expected_board, actual_board);
}
