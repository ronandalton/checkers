#ifndef TUI_H
#define TUI_H


class Game;
class Engine;
class Move;
enum class Turn;
enum class MatchType;

#include <vector>
#include <string>
#include <iosfwd>


class Tui {
public:
	Tui() = delete;
	
	static void printIntro();
	static MatchType askForMatchType();
	static void printMatchType(const Game &game);
	static void printGameState(const Game &game);
	static void doNextMove(Game *game, Engine *engine);
	static bool askToPlayAgain();
	static void printOutro();
	
private:
	static void printBoard(const Game &game);
	static void printTurn(const Game &game);
	static void printMovesAvailable(const Game &game);
	static Move askForMove(const std::vector<Move> &moves);
	static Move getComputerMove(const Game &game, Engine *engine);
	static void printMoveMade(Turn turn, const Move &move);
	static void printWinner(const Game &game);
	static std::string getMoveString(const Move &move);
	
	static std::vector<int> parseMoveString(const std::string &input);
	static Move findMatchingMove(const std::vector<int> &indexes, const std::vector<Move> &moves);
};


#endif
