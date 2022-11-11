#include "gtest/gtest.h"

#include "game/direction.h"
#include "game/coord.h"


TEST(game_direction_defaultConstructor, testCorrectDefault) {
	EXPECT_EQ(Direction(), Direction::UP_LEFT);
}


TEST(game_direction_typeConstructor, testAssignedCorrectValue) {
	EXPECT_EQ(Direction(Direction::DOWN_RIGHT), Direction::DOWN_RIGHT);
}


TEST(game_direction_getReverse, testWorksCorrectly) {
	EXPECT_EQ(Direction(Direction::DOWN_LEFT).getReverse(), Direction::UP_RIGHT);
}


TEST(game_direction_isUpwards, testCorrectReturnValue) {
	EXPECT_TRUE(Direction(Direction::UP_LEFT).isUpwards());
	EXPECT_FALSE(Direction(Direction::DOWN_RIGHT).isUpwards());
}


TEST(game_direction_isDownwards, testCorrectReturnValue) {
	EXPECT_TRUE(Direction(Direction::DOWN_LEFT).isDownwards());
	EXPECT_FALSE(Direction(Direction::UP_RIGHT).isDownwards());
}


TEST(game_direction_getOffset, testCorrectReturnValue) {
	EXPECT_EQ(Direction(Direction::UP_RIGHT).getOffset(), Coord(1, -1));
	EXPECT_EQ(Direction(Direction::DOWN_LEFT).getOffset(), Coord(-1, 1));
}
