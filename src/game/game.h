#ifndef GAME_H
#define GAME_H


enum class Player;

#include "game/board.h"
#include "game/turn.h"
#include "game/move.h"
#include "game/matchtype.h"

#include <vector>


class Game {
public:
	void newGame(MatchType match_type);
	bool doMove(const Move &move);
	void setBoard(const Board &board);
	void setTurn(Turn turn);
	void setMatchType(MatchType match_type);

	const Board& getBoard() const;
	Turn getTurn() const;
	const std::vector<Move>& getAvailableMoves() const;
	MatchType getMatchType() const;
	bool isOver() const;
	Player getPlayerType(Turn turn) const;
	bool requiresEngine() const;

private:
	Board m_board;
	Turn m_turn {};
	MatchType m_match_type {};
	
	std::vector<Move> m_moves;
};


#endif
