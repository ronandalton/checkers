#include "gtest/gtest.h"
#include "util.h"

#include "game/game.h"
#include "game/board.h"
#include "game/turn.h"
#include "game/matchtype.h"
#include "game/move.h"
#include "game/player.h"

#include <vector>


TEST(game_game_newGame, testEntireBoardGetsReset) {
	Game game;

	Board board_all_black_kings = Util::createBoard(
		"     -----------------     "
		" 00 |   B   B   B   B | 03 "
		" 04 | B   B   B   B   | 07 "
		" 08 |   B   B   B   B | 11 "
		" 12 | B   B   B   B   | 15 "
		" 16 |   B   B   B   B | 19 "
		" 20 | B   B   B   B   | 23 "
		" 24 |   B   B   B   B | 27 "
		" 28 | B   B   B   B   | 31 "
		"     -----------------     "
	);

	game.setBoard(board_all_black_kings);

	Board expected_board_after_reset = Util::createBoard(
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
	
	game.newGame(MatchType::HUMAN_VS_HUMAN);

	Board actual_board_after_reset = game.getBoard();

	EXPECT_EQ(expected_board_after_reset, actual_board_after_reset);
}


TEST(game_game_newGame, testTurnGetsReset) {
	Game game;

	game.setTurn(Turn::WHITE);

	Turn expected_turn_after_reset = Turn::BLACK;

	game.newGame(MatchType::HUMAN_VS_HUMAN);

	Turn actual_turn_after_reset = game.getTurn();

	EXPECT_EQ(expected_turn_after_reset, actual_turn_after_reset);
}


TEST(game_game_newGame, testMatchTypeGetsSetToValueGiven) {
	Game game;

	game.setMatchType(MatchType::COMPUTER_VS_COMPUTER);

	MatchType wanted_match_type = MatchType::COMPUTER_VS_HUMAN;

	game.newGame(wanted_match_type);

	MatchType actual_match_type = game.getMatchType();

	EXPECT_EQ(wanted_match_type, actual_match_type);
}


TEST(game_game_newGame, testMovesAvailableIsUpdated) {
	Game game;

	Board random_board = Util::createBoard(
		"     -----------------     "
		" 00 |   b   _   W   _ | 03 "
		" 04 | b   _   _   _   | 07 "
		" 08 |   _   _   W   _ | 11 "
		" 12 | B   B   _   _   | 15 "
		" 16 |   _   _   _   _ | 19 "
		" 20 | _   _   _   _   | 23 "
		" 24 |   _   _   _   w | 27 "
		" 28 | _   _   w   w   | 31 "
		"     -----------------     "
	);

	game.setBoard(random_board);

	std::vector<Move> expected_moves_after = {
		{{8, 12}},
		{{8, 13}},
		{{9, 13}},
		{{9, 14}},
		{{10, 14}},
		{{10, 15}},
		{{11, 15}},
	};

	game.newGame(MatchType::HUMAN_VS_HUMAN);

	std::vector<Move> actual_moves_after = game.getAvailableMoves();

	EXPECT_TRUE(Util::moveListsAreEquivalent(expected_moves_after, actual_moves_after));
}


TEST(game_game_doMove, testFailureOnInvalidMove) {
	Game game;

	Board board = Util::createBoard(
		"     -----------------     "
		" 00 |   b   _   _   b | 03 "
		" 04 | b   _   _   _   | 07 "
		" 08 |   _   _   _   _ | 11 "
		" 12 | _   B   _   _   | 15 "
		" 16 |   _   _   _   w | 19 "
		" 20 | _   _   _   _   | 23 "
		" 24 |   B   _   _   w | 27 "
		" 28 | _   _   w   w   | 31 "
		"     -----------------     "
	);

	game.setBoard(board);

	Turn turn = game.getTurn();

	std::vector<Move> moves = game.getAvailableMoves();

	Move invalid_move({10, 14});

	EXPECT_FALSE(game.doMove(invalid_move));

	EXPECT_EQ(board, game.getBoard());
	EXPECT_EQ(turn, game.getTurn());
	EXPECT_TRUE(Util::moveListsAreEquivalent(moves, game.getAvailableMoves()));
}


TEST(game_game_doMove, testBoardGetsUpdated) {
	Game game;

	Board board = Util::createBoard(
		"     -----------------     "
		" 00 |   _   _   _   _ | 03 "
		" 04 | _   _   _   _   | 07 "
		" 08 |   W   _   _   _ | 11 "
		" 12 | _   B   _   _   | 15 "
		" 16 |   _   W   _   _ | 19 "
		" 20 | B   _   _   _   | 23 "
		" 24 |   _   _   _   _ | 27 "
		" 28 | _   _   _   _   | 31 "
		"     -----------------     "
	);

	game.setBoard(board);
	game.setTurn(Turn::BLACK);
	
	EXPECT_TRUE(game.doMove({{13, 22}, {17}}));

	Board expected_board = Util::createBoard(
		"     -----------------     "
		" 00 |   _   _   _   _ | 03 "
		" 04 | _   _   _   _   | 07 "
		" 08 |   W   _   _   _ | 11 "
		" 12 | _   _   _   _   | 15 "
		" 16 |   _   _   _   _ | 19 "
		" 20 | B   _   B   _   | 23 "
		" 24 |   _   _   _   _ | 27 "
		" 28 | _   _   _   _   | 31 "
		"     -----------------     "
	);

	Board actual_board = game.getBoard();

	EXPECT_EQ(expected_board, actual_board);
}


TEST(game_game_doMove, testTurnGetsUpdated) {
	Game game;

	Board board = Util::createBoard(
		"     -----------------     "
		" 00 |   _   _   _   _ | 03 "
		" 04 | _   _   _   W   | 07 "
		" 08 |   _   _   _   _ | 11 "
		" 12 | _   _   _   B   | 15 "
		" 16 |   _   B   _   _ | 19 "
		" 20 | _   _   _   _   | 23 "
		" 24 |   _   _   _   _ | 27 "
		" 28 | _   _   _   _   | 31 "
		"     -----------------     "
	);

	game.setBoard(board);
	game.setTurn(Turn::BLACK);

	EXPECT_TRUE(game.doMove({{17, 14}}));

	EXPECT_EQ(game.getTurn(), Turn::WHITE);
}


TEST(game_game_doMove, testMovesAvailableGetUpdated) {
	Game game;

	Board board = Util::createBoard(
		"     -----------------     "
		" 00 |   _   b   _   _ | 03 "
		" 04 | _   b   _   _   | 07 "
		" 08 |   b   _   _   _ | 11 "
		" 12 | _   _   _   _   | 15 "
		" 16 |   _   w   _   _ | 19 "
		" 20 | _   _   _   _   | 23 "
		" 24 |   _   _   _   _ | 27 "
		" 28 | _   _   _   _   | 31 "
		"     -----------------     "
	);

	game.setBoard(board);
	game.setTurn(Turn::BLACK);

	std::vector<Move> expected_moves_before = {
		{{1, 6}},
		{{5, 9}},
		{{8, 12}},
		{{8, 13}},
	};

	std::vector<Move> actual_moves_before = game.getAvailableMoves();

	EXPECT_TRUE(Util::moveListsAreEquivalent(expected_moves_before, actual_moves_before));

	EXPECT_TRUE(game.doMove({{8, 13}}));

	std::vector<Move> expected_moves_after = {
		{{17, 8}, {13}},
	};

	std::vector<Move> actual_moves_after = game.getAvailableMoves();

	EXPECT_TRUE(Util::moveListsAreEquivalent(expected_moves_after, actual_moves_after));
}


TEST(game_game_setAndGetBoard, testWorksCorrectly) {
	Game game;

	Board board = Util::createBoard(
		"     -----------------     "
		" 00 |   _   b   _   _ | 03 "
		" 04 | _   _   _   W   | 07 "
		" 08 |   b   _   _   _ | 11 "
		" 12 | _   _   _   _   | 15 "
		" 16 |   _   _   _   _ | 19 "
		" 20 | _   _   _   _   | 23 "
		" 24 |   _   _   _   _ | 27 "
		" 28 | _   w   _   w   | 31 "
		"     -----------------     "
	);

	game.setBoard(board);

	EXPECT_EQ(board, game.getBoard());
}


TEST(game_game_setAndGetTurn, testWorksCorrectly) {
	Game game;

	Turn turn = Turn::WHITE;

	game.setTurn(turn);

	EXPECT_EQ(turn, game.getTurn());
}


TEST(game_game_setAndGetMatchType, testWorksCorrectly) {
	Game game;

	MatchType match_type = MatchType::COMPUTER_VS_HUMAN;

	game.setMatchType(match_type);

	EXPECT_EQ(match_type, game.getMatchType());
}


TEST(game_game_getAvailableMoves, testWorksCorrectly) {
	Game game;

	Board board = Util::createBoard(
		"     -----------------     "
		" 00 |   _   b   _   _ | 03 "
		" 04 | _   _   _   _   | 07 "
		" 08 |   _   _   _   _ | 11 "
		" 12 | _   _   _   _   | 15 "
		" 16 |   _   _   _   b | 19 "
		" 20 | _   w   _   _   | 23 "
		" 24 |   _   _   _   _ | 27 "
		" 28 | _   _   _   _   | 31 "
		"     -----------------     "
	);

	game.setBoard(board);

	game.setTurn(Turn::BLACK);

	std::vector<Move> moves = {
		{{1, 5}},
		{{1, 6}},
		{{19, 23}},
	};

	EXPECT_TRUE(Util::moveListsAreEquivalent(moves, game.getAvailableMoves()));
}


TEST(game_game_isOver, testTrueOnHasNoPiecesLeft) {
	Game game;

	Board board = Util::createBoard(
		"     -----------------     "
		" 00 |   _   _   _   _ | 03 "
		" 04 | _   _   _   _   | 07 "
		" 08 |   _   _   _   _ | 11 "
		" 12 | _   _   _   _   | 15 "
		" 16 |   _   _   _   _ | 19 "
		" 20 | _   B   _   _   | 23 "
		" 24 |   _   W   _   _ | 27 "
		" 28 | _   _   _   B   | 31 "
		"     -----------------     "
	);

	game.setBoard(board);
	game.setTurn(Turn::BLACK);

	EXPECT_FALSE(game.isOver());

	EXPECT_TRUE(game.doMove({{21, 30}, {25}}));

	EXPECT_TRUE(game.isOver());
}


TEST(game_game_isOver, testHasPiecesLeftButNoLegalMove) {
	Game game;

	Board board = Util::createBoard(
		"     -----------------     "
		" 00 |   b   _   _   _ | 03 "
		" 04 | w   _   _   _   | 07 "
		" 08 |   _   _   _   _ | 11 "
		" 12 | _   _   _   _   | 15 "
		" 16 |   _   _   _   _ | 19 "
		" 20 | B   B   _   _   | 23 "
		" 24 |   _   _   _   _ | 27 "
		" 28 | w   _   _   _   | 31 "
		"     -----------------     "
	);

	game.setBoard(board);
	game.setTurn(Turn::BLACK);

	EXPECT_FALSE(game.isOver());

	EXPECT_TRUE(game.doMove({{20, 24}}));

	EXPECT_TRUE(game.isOver());
}


TEST(game_game_getPlayerType, testWorksCorrectly) {
	Game game;

	game.setMatchType(MatchType::HUMAN_VS_HUMAN);
	EXPECT_EQ(game.getPlayerType(Turn::BLACK), Player::HUMAN);
	EXPECT_EQ(game.getPlayerType(Turn::WHITE), Player::HUMAN);

	game.setMatchType(MatchType::HUMAN_VS_COMPUTER);
	EXPECT_EQ(game.getPlayerType(Turn::BLACK), Player::HUMAN);
	EXPECT_EQ(game.getPlayerType(Turn::WHITE), Player::COMPUTER);

	game.setMatchType(MatchType::COMPUTER_VS_HUMAN);
	EXPECT_EQ(game.getPlayerType(Turn::BLACK), Player::COMPUTER);
	EXPECT_EQ(game.getPlayerType(Turn::WHITE), Player::HUMAN);

	game.setMatchType(MatchType::COMPUTER_VS_COMPUTER);
	EXPECT_EQ(game.getPlayerType(Turn::BLACK), Player::COMPUTER);
	EXPECT_EQ(game.getPlayerType(Turn::WHITE), Player::COMPUTER);
}


TEST(game_game_requiresEngine, testTrueCases) {
	Game game;

	game.setMatchType(MatchType::HUMAN_VS_COMPUTER);
	EXPECT_TRUE(game.requiresEngine());

	game.setMatchType(MatchType::COMPUTER_VS_HUMAN);
	EXPECT_TRUE(game.requiresEngine());

	game.setMatchType(MatchType::COMPUTER_VS_COMPUTER);
	EXPECT_TRUE(game.requiresEngine());
}

TEST(game_game_requiresEngine, testFalseCases) {
	Game game;

	game.setMatchType(MatchType::HUMAN_VS_HUMAN);
	EXPECT_FALSE(game.requiresEngine());
}
