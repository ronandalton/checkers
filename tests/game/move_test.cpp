#include "gtest/gtest.h"
#include "util.h" // TODO: do we need this?

#include "game/move.h"
#include "game/position.h"


TEST(game_move_constructor, testNormalMoveConstructor) {
	std::vector<Position> positions = {11, 15};

	Move move(positions);

	EXPECT_EQ(move.getPositions(), positions);
	EXPECT_EQ(move.getJumpedPiecePositions(), std::vector<Position>());
}


TEST(game_move_constructor, testJumpingMoveConstructor) {
	std::vector<Position> positions = {24, 17, 8};
	std::vector<Position> jumped_piece_positions = {21, 13};

	Move move(positions, jumped_piece_positions);

	EXPECT_EQ(move.getPositions(), positions);
	EXPECT_EQ(move.getJumpedPiecePositions(), jumped_piece_positions);
}


TEST(game_move_constructor, testPositionConstructor) {
	Position position = 9;

	Move move(position);

	EXPECT_EQ(move.getPositions(), std::vector<Position>({position}));
	EXPECT_EQ(move.getJumpedPiecePositions(), std::vector<Position>());
}


TEST(game_move_addPosition, testWorksCorrectly) {
	Move move(21);

	move.addPosition(16);

	EXPECT_EQ(move, Move({21, 16}));
}


TEST(game_move_addJumpedPiecePosition, testWorksCorrectly) {
	Move move({29, 22, 13}, {25});

	move.addJumpedPiecePosition(17);

	EXPECT_EQ(move, Move({29, 22, 13}, {25, 17}));
}


TEST(game_move_testEqualityOperator, testWorksCorrectly) {
	EXPECT_TRUE(Move({23, 19}) == Move({23, 19}, {}));
	EXPECT_FALSE(Move({4, 13, 20}, {8, 16}) == Move({4, 13, 20}));
}


TEST(game_move_getLength, testWorksCorrectly) {
	EXPECT_EQ(Move(5).getLength(), 1);
	EXPECT_EQ(Move({15, 10}).getLength(), 2);
	EXPECT_EQ(Move({13, 22}, {17}).getLength(), 2);
	EXPECT_EQ(Move({31, 22, 15, 6}, {26, 18, 10}).getLength(), 4);
}


TEST(game_move_getPosition, testWorksCorrectly) {
	Move move({3, 10, 19}, {7, 15});

	EXPECT_EQ(move.getPosition(0), 3);
	EXPECT_EQ(move.getPosition(1), 10);
	EXPECT_EQ(move.getPosition(2), 19);
}


TEST(game_move_getPositions, testWorksCorrectly) {
	std::vector<Position> positions = {0, 9};
	std::vector<Position> jumped_piece_positions = {5};

	Move move(positions, jumped_piece_positions);

	EXPECT_EQ(move.getPositions(), positions);
}


TEST(game_move_getJumpedPiecePositions, testWorksCorrectly) {
	std::vector<Position> positions = {0, 9, 18};
	std::vector<Position> jumped_piece_positions = {5, 14};

	Move move(positions, jumped_piece_positions);

	EXPECT_EQ(move.getJumpedPiecePositions(), jumped_piece_positions);
}


TEST(game_move_getStartPosition, testWorksCorrectly) {
	EXPECT_EQ(Move({28, 24}).getStartPosition(), 28);
}


TEST(game_move_getEndPosition, testWorksCorrectly) {
	EXPECT_EQ(Move({12, 21}, {16}).getEndPosition(), 21);
}


TEST(game_move_isJump, testWorksCorrectly) {
	EXPECT_TRUE(Move({22, 29}, {25}).isJump());
	EXPECT_TRUE(Move({11, 18, 9}, {15, 14}).isJump());

	EXPECT_FALSE(Move(23).isJump());
	EXPECT_FALSE(Move({5, 8}).isJump());
}


TEST(game_move_exists, testWorksCorrectly) {
	EXPECT_FALSE(Move().exists());
	
	EXPECT_TRUE(Move({15, 19}).exists());
	EXPECT_TRUE(Move({21, 30}, {25}).exists());
	EXPECT_TRUE(Move(0).exists());
}
