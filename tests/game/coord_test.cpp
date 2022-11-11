#include "gtest/gtest.h"

#include "game/coord.h"
#include "game/position.h"
#include "game/direction.h"


TEST(game_coord_applyOffset, testSingleOffset) {
	EXPECT_EQ(Coord(2, 5).applyOffset(Direction::UP_LEFT, false), Coord(1, 4));
	EXPECT_EQ(Coord(2, 5).applyOffset(Direction::UP_RIGHT, false), Coord(3, 4));
	EXPECT_EQ(Coord(2, 5).applyOffset(Direction::DOWN_RIGHT, false), Coord(3, 6));
	EXPECT_EQ(Coord(2, 5).applyOffset(Direction::DOWN_LEFT, false), Coord(1, 6));
}


TEST(game_coord_applyOffset, testDoubleOffset) {
	EXPECT_EQ(Coord(3, 2).applyOffset(Direction::UP_LEFT, true), Coord(1, 0));
	EXPECT_EQ(Coord(3, 2).applyOffset(Direction::UP_RIGHT, true), Coord(5, 0));
	EXPECT_EQ(Coord(3, 2).applyOffset(Direction::DOWN_RIGHT, true), Coord(5, 4));
	EXPECT_EQ(Coord(3, 2).applyOffset(Direction::DOWN_LEFT, true), Coord(1, 4));
}


TEST(game_coord_getX, testReturnsCorrectValue) {
	EXPECT_EQ(Coord(1, 2).getX(), 1);
}


TEST(game_coord_getY, testReturnsCorrectValue) {
	EXPECT_EQ(Coord(1, 2).getY(), 2);
}


TEST(game_coord_isValid, testValidPositions) {
	// test inside board boundaries
	EXPECT_TRUE(Coord(3, 2).isValid());
	EXPECT_TRUE(Coord(4, 3).isValid());

	// test edges of board:
	EXPECT_TRUE(Coord(3, 0).isValid());
	EXPECT_TRUE(Coord(7, 0).isValid());
	EXPECT_TRUE(Coord(4, 7).isValid());
	EXPECT_TRUE(Coord(0, 7).isValid());
}


TEST(game_coord_isValid, testInvalidPositions) {
	// test inside of board
	EXPECT_FALSE(Coord(1, 5).isValid());
	EXPECT_FALSE(Coord(2, 6).isValid());

	// test inner edges of board
	EXPECT_FALSE(Coord(0, 0).isValid());
	EXPECT_FALSE(Coord(4, 0).isValid());
	EXPECT_FALSE(Coord(7, 1).isValid());
	EXPECT_FALSE(Coord(7, 7).isValid());
	EXPECT_FALSE(Coord(0, 6).isValid());

	// test outer edges of board
	EXPECT_FALSE(Coord(-1, -1).isValid());
	EXPECT_FALSE(Coord(3, -1).isValid());
	EXPECT_FALSE(Coord(8, -1).isValid());
	EXPECT_FALSE(Coord(8, 0).isValid());
	EXPECT_FALSE(Coord(8, 8).isValid());
	EXPECT_FALSE(Coord(2, 8).isValid());
	EXPECT_FALSE(Coord(-1, 8).isValid());
	EXPECT_FALSE(Coord(-1, 0).isValid());

	// test outside of board
	EXPECT_FALSE(Coord(-5, 18).isValid());
	EXPECT_FALSE(Coord(12, 3).isValid());
	EXPECT_FALSE(Coord(3, -2).isValid());
	EXPECT_FALSE(Coord(-2, -2).isValid());
}


TEST(game_coord_getPosition, testWorksWithValidPositions) {
	EXPECT_EQ(Coord(1, 0).getPosition(), 0);
	EXPECT_EQ(Coord(6, 7).getPosition(), 31);
	EXPECT_EQ(Coord(7, 2).getPosition(), 11);
	EXPECT_EQ(Coord(0, 3).getPosition(), 12);
	EXPECT_EQ(Coord(2, 7).getPosition(), 29);
	EXPECT_EQ(Coord(3, 6).getPosition(), 25);
}
