#include "gtest/gtest.h"

#include "game/position.h"
#include "game/coord.h"
#include "game/direction.h"
#include "game/turn.h"


TEST(game_position_allPositionsArray, testCorrectSizeAndValues) {
	int index = 0;

	for (Position position : Position::ALL_POSITIONS) {
		EXPECT_EQ(position, index);
		index++;
	}

	EXPECT_EQ(index, 32);
}


TEST(game_position_isValidIndex, testTrueCases) {
	EXPECT_TRUE(Position::isValidIndex(0));
	EXPECT_TRUE(Position::isValidIndex(1));
	EXPECT_TRUE(Position::isValidIndex(12));
	EXPECT_TRUE(Position::isValidIndex(25));
	EXPECT_TRUE(Position::isValidIndex(31));
}


TEST(game_position_isValidIndex, testFalseCases) {
	EXPECT_FALSE(Position::isValidIndex(-9));
	EXPECT_FALSE(Position::isValidIndex(-2));
	EXPECT_FALSE(Position::isValidIndex(-1));
	EXPECT_FALSE(Position::isValidIndex(32));
	EXPECT_FALSE(Position::isValidIndex(33));
	EXPECT_FALSE(Position::isValidIndex(41));
}


TEST(game_position_constructor, testDefaultIsZero) {
	EXPECT_EQ(Position(), 0);
}


TEST(game_position_constructor, testIntegerConstructor) {
	EXPECT_EQ(Position(13), 13);
}


TEST(game_position_getX, testCorrectness) {
	EXPECT_EQ(Position(6).getX(), 4);
	EXPECT_EQ(Position(19).getX(), 7);
}


TEST(game_position_getY, testCorrectness) {	
	EXPECT_EQ(Position(27).getY(), 6);
	EXPECT_EQ(Position(28).getY(), 7);
}


TEST(game_position_getCoord, testCorrectness) {
	EXPECT_EQ(Position(0).getCoord(), Coord(1, 0));
	EXPECT_EQ(Position(31).getCoord(), Coord(6, 7));
	EXPECT_EQ(Position(5).getCoord(), Coord(2, 1));
	EXPECT_EQ(Position(16).getCoord(), Coord(1, 4));
	EXPECT_EQ(Position(13).getCoord(), Coord(2, 3));
	EXPECT_EQ(Position(19).getCoord(), Coord(7, 4));
	EXPECT_EQ(Position(20).getCoord(), Coord(0, 5));
}


TEST(game_position_isCrowningSquare, testCorrectness) {
	EXPECT_TRUE(Position(3).isCrowningSquare(Turn::WHITE));
	EXPECT_FALSE(Position(4).isCrowningSquare(Turn::WHITE));
	EXPECT_FALSE(Position(27).isCrowningSquare(Turn::BLACK));
	EXPECT_TRUE(Position(28).isCrowningSquare(Turn::BLACK));
	EXPECT_TRUE(Position(0).isCrowningSquare(Turn::WHITE));
	EXPECT_TRUE(Position(31).isCrowningSquare(Turn::BLACK));
	EXPECT_FALSE(Position(23).isCrowningSquare(Turn::BLACK));
}


TEST(game_position_offsetIsValid, testTrueCases) {
	EXPECT_TRUE(Position(16).offsetIsValid(Direction::DOWN_LEFT, false));
	EXPECT_TRUE(Position(9).offsetIsValid(Direction::UP_RIGHT, true));
	EXPECT_TRUE(Position(22).offsetIsValid(Direction::UP_LEFT, true));
	EXPECT_TRUE(Position(7).offsetIsValid(Direction::DOWN_RIGHT, false));
}


TEST(game_position_offsetIsValid, testFalseCases) {
	EXPECT_FALSE(Position(16).offsetIsValid(Direction::UP_LEFT, true));
	EXPECT_FALSE(Position(27).offsetIsValid(Direction::DOWN_RIGHT, false));
	EXPECT_FALSE(Position(7).offsetIsValid(Direction::UP_RIGHT, true));
}


TEST(game_position_getOffset, testCorrectness) {
	EXPECT_EQ(Position(9).getOffset(Direction::DOWN_RIGHT, true), Position(18));
	EXPECT_EQ(Position(21).getOffset(Direction::DOWN_LEFT, true), Position(28));
	EXPECT_EQ(Position(4).getOffset(Direction::UP_RIGHT, false), Position(0));
	EXPECT_EQ(Position(26).getOffset(Direction::UP_LEFT, false), Position(22));
}
