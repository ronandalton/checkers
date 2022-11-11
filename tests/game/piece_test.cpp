#include "gtest/gtest.h"

#include "game/piece.h"
#include "game/turn.h"
#include "game/direction.h"


TEST(game_piece_defaultConstructor, testDefaultIsNone) {
	EXPECT_EQ(Piece(), Piece::NONE);
}


TEST(game_piece_typeConstructor, testAssignedCorrectValue) {
	EXPECT_EQ(Piece(Piece::BLACK_KING), Piece::BLACK_KING);
}


TEST(game_piece_crown, testWorksOnMen) {
	EXPECT_EQ(Piece(Piece::BLACK_MAN).crown(), Piece::BLACK_KING);
	EXPECT_EQ(Piece(Piece::WHITE_MAN).crown(), Piece::WHITE_KING);
}


TEST(game_piece_crown, testNoEffectOnKings) {
	EXPECT_EQ(Piece(Piece::BLACK_KING).crown(), Piece::BLACK_KING);
	EXPECT_EQ(Piece(Piece::WHITE_KING).crown(), Piece::WHITE_KING);
}


TEST(game_piece_crown, testNoEffectOnNoneType) {
	EXPECT_EQ(Piece(Piece::NONE).crown(), Piece::NONE);
}


TEST(game_piece_remove, testWorksOnActualPieces) {
	EXPECT_EQ(Piece(Piece::BLACK_MAN).remove(), Piece::NONE);
}


TEST(game_piece_remove, testNoEffectOnNoneType) {
	EXPECT_EQ(Piece(Piece::NONE).remove(), Piece::NONE);
}


TEST(game_piece_exists, testWorksOnActualPieces) {
	EXPECT_TRUE(Piece(Piece::WHITE_MAN).exists());
	EXPECT_TRUE(Piece(Piece::BLACK_KING).exists());
}


TEST(game_piece_exists, testWorksOnNoneType) {
	EXPECT_FALSE(Piece(Piece::NONE).exists());
}


TEST(game_piece_belongsTo, testWorksOnActualPieces) {
	EXPECT_FALSE(Piece(Piece::BLACK_KING).belongsTo(Turn::WHITE));
	EXPECT_TRUE(Piece(Piece::WHITE_MAN).belongsTo(Turn::WHITE));
	EXPECT_TRUE(Piece(Piece::BLACK_MAN).belongsTo(Turn::BLACK));
}


TEST(game_piece_belongsTo, testNonePieceBelongsToNeitherPlayer) {
	EXPECT_FALSE(Piece(Piece::NONE).belongsTo(Turn::BLACK));
	EXPECT_FALSE(Piece(Piece::NONE).belongsTo(Turn::WHITE));
}


TEST(game_piece_getTurn, testWorksOnActualPieces) {
	EXPECT_EQ(Piece(Piece::BLACK_MAN).getTurn(), Turn::BLACK);
	EXPECT_EQ(Piece(Piece::WHITE_MAN).getTurn(), Turn::WHITE);
	EXPECT_EQ(Piece(Piece::WHITE_KING).getTurn(), Turn::WHITE);
}


TEST(game_piece_getOppositeTurn, testWorksOnActualPieces) {
	EXPECT_EQ(Piece(Piece::WHITE_KING).getOppositeTurn(), Turn::BLACK);
	EXPECT_EQ(Piece(Piece::BLACK_MAN).getOppositeTurn(), Turn::WHITE);
	EXPECT_EQ(Piece(Piece::WHITE_KING).getOppositeTurn(), Turn::BLACK);
}


TEST(game_piece_isCrowned, testReturnsTrueForKings) {
	EXPECT_TRUE(Piece(Piece::BLACK_KING).isCrowned());
	EXPECT_TRUE(Piece(Piece::WHITE_KING).isCrowned());
}


TEST(game_piece_isCrowned, testReturnsFalseForMen) {
	EXPECT_FALSE(Piece(Piece::BLACK_MAN).isCrowned());
	EXPECT_FALSE(Piece(Piece::WHITE_MAN).isCrowned());
}


TEST(game_piece_isCrowned, testReturnsFalseForNoneType) {
	EXPECT_FALSE(Piece(Piece::NONE).isCrowned());
}


TEST(game_piece_canMoveInDirection, testBlackMen) {
	EXPECT_TRUE(Piece(Piece::BLACK_MAN).canMoveInDirection(Direction::DOWN_RIGHT));
	EXPECT_FALSE(Piece(Piece::BLACK_MAN).canMoveInDirection(Direction::UP_LEFT));
}


TEST(game_piece_canMoveInDirection, testWhiteMen) {
	EXPECT_TRUE(Piece(Piece::WHITE_MAN).canMoveInDirection(Direction::UP_RIGHT));
	EXPECT_FALSE(Piece(Piece::WHITE_MAN).canMoveInDirection(Direction::DOWN_LEFT));
}


TEST(game_piece_canMoveInDirection, testBlackKings) {
	EXPECT_TRUE(Piece(Piece::BLACK_KING).canMoveInDirection(Direction::UP_LEFT));
	EXPECT_TRUE(Piece(Piece::BLACK_KING).canMoveInDirection(Direction::DOWN_LEFT));
}


TEST(game_piece_canMoveInDirection, testWhiteKings) {
	EXPECT_TRUE(Piece(Piece::WHITE_KING).canMoveInDirection(Direction::DOWN_RIGHT));
	EXPECT_TRUE(Piece(Piece::WHITE_KING).canMoveInDirection(Direction::UP_RIGHT));
}


TEST(game_piece_canMoveInDirection, testNonePieces) {
	EXPECT_FALSE(Piece(Piece::NONE).canMoveInDirection(Direction::UP_RIGHT));
	EXPECT_FALSE(Piece(Piece::NONE).canMoveInDirection(Direction::DOWN_LEFT));
}
