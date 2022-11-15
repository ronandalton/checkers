#ifndef TUI_H
#define TUI_H


#include "game/game.h"
#include "engine/engine.h"

#include <vector>
#include <string>

class Move;
enum class Turn;
enum class MatchType;


class Tui {
public:
	void run();

private:
	void printIntro() const;
	MatchType askForMatchType() const;
	void printMatchType() const;
	void printGameState() const;
	void doNextMove();
	bool askToPlayAgain() const;
	void printOutro() const;
	void printBoard() const;
	void printTurn() const;
	void printMovesAvailable() const;
	Move askForMove() const;
	Move getComputerMove();
	void printMoveMade(Turn turn, const Move& move) const;
	void printWinner() const;

	static std::string getMoveString(const Move& move);
	static std::vector<int> parseMoveString(const std::string& input);
	static const Move* findMatchingMove(const std::vector<int>& position_indexes, const std::vector<Move>& moves_available);

	Game m_game;
	Engine m_engine;
};


#endif // TUI_H
