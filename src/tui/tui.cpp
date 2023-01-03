#include "tui/tui.h"

#include "game/game.h"
#include "engine/engine.h"
#include "game/move.h"
#include "game/turn.h"
#include "game/player.h"
#include "game/matchtype.h"
#include "game/coord.h"

#include <iostream>
#include <cassert>
#include <iomanip> // for number padding
#include <algorithm> // for std::min


/**
 * Runs the game with a text user interface.
 */
int Tui::run(int argc, char *argv[]) {
	printIntro();
	
	do {
		m_game.newGame(askForMatchType());
		printMatchType();
		
		while (!m_game.isOver()) {
			printGameState();
			doNextMove();
		}
		
		printGameState();
	} while (askToPlayAgain());
	
	printOutro();

	return 0;
}


/**
 * Prints a welcome message.
 */
void Tui::printIntro() const {
	std::cout << "Welcome to the Checkers/Draughts game!\n";
	std::cout << '\n';
}


/**
 * Asks the user what match type they want this game to be.
 * Eg. Human vs Computer, Human vs Human, etc...
 * @return The match type the user chose.
 */
MatchType Tui::askForMatchType() const {
	std::cout << "Please choose between the following match types:\n";
	std::cout << "\t(1) Human vs Human\n";
	std::cout << "\t(2) Human vs Computer\n";
	std::cout << "\t(3) Computer vs Human\n";
	std::cout << "\t(4) Computer vs Computer\n";
	std::cout << '\n';
	
	while (true) {
		std::cout << "Enter the number of your choice: ";
		std::string choice;
		std::getline(std::cin, choice);
		
		if (choice == "1") {
			return MatchType::HUMAN_VS_HUMAN;
		} else if (choice == "2") {
			return MatchType::HUMAN_VS_COMPUTER;
		} else if (choice == "3") {
			return MatchType::COMPUTER_VS_HUMAN;
		} else if (choice == "4") {
			return MatchType::COMPUTER_VS_COMPUTER;
		}
	}

}


/**
 * Prints the match type of the game.
 */
void Tui::printMatchType() const {
	std::cout << "Match type: ";
	
	for (Turn turn : {Turn::BLACK, Turn::WHITE}) {
		if (m_game.getPlayerType(turn) == Player::HUMAN) {
			std::cout << "Human";
		} else {
			std::cout << "Computer";
		}
		if (turn == Turn::BLACK) {
			std::cout << " vs ";
		}
	}
	
	std::cout << '\n';
	
	std::cout << '\n';
}


/**
 * Prints the current game state.
 * This includes the board, turn and moves available.
 * If the game is over, only the board is printed and the winner is announced.
 */
void Tui::printGameState() const {
	printBoard();
	if (!m_game.isOver()) {
		printTurn();
		printMovesAvailable();
	} else {
		std::cout << '\n';
		printWinner();
	}
	
	std::cout << '\n';
}


/**
 * Handles doing the next move.
 * The move is first queried (either from the user or engine) and then executed.
 */
void Tui::doNextMove() {
	Move move;
	
	if (m_game.getPlayerType(m_game.getTurn()) == Player::HUMAN) {
		move = askForMove();
	} else {
		move = getComputerMove();
	}
	
	printMoveMade(m_game.getTurn(), move);
	
	bool move_successful = m_game.doMove(move);
	
	assert(move_successful);
	
	std::cout << '\n';
}


/**
 * Asks the user if they want to play again.
 * @return True if they want to play again, false otherwise.
 */
bool Tui::askToPlayAgain() const {
	while (true) {
		std::cout << "Would you like to play again? ";
		std::string input;
		std::getline(std::cin, input);
		
		if (input[0] == 'y' || input[0] == 'Y') {
			std::cout << '\n';
			return true;
		}
		if (input[0] == 'n' || input[0] == 'N') {
			std::cout << '\n';
			return false;
		}
	}
}


/**
 * Thanks the user for playing.
 */
void Tui::printOutro() const {
	std::cout << "Thanks for playing!\n";
}


/**
 * Prints the board out in an ASCII representation.
 * Note that the board will be rotated if it is a computer vs human game.
 */
void Tui::printBoard() const {
	const char previous_fill_character = std::cout.fill('0'); // set fill character to '0' and remember old one
	
	std::cout << "     -----------------     \n";
	
	const bool counting_upwards = (m_game.getMatchType() == MatchType::COMPUTER_VS_HUMAN);
	
	int index = counting_upwards ? 0 : 31;
	
	for (int y = 0; y < 8; y++) {
		std::cout << ' ' << std::setw(2) << index + 1 << " | ";
		
		int last_index;
		
		for (int x = 0; x < 8; x++) {
			char symbol;
			
			if (Coord(x, y).isValidPosition()) {
				Piece piece = m_game.getBoard().pieceAt(index);
				
				if (piece.belongsTo(Turn::BLACK)) {
					symbol = piece.isCrowned() ? 'B' : 'b';
				} else if (piece.belongsTo(Turn::WHITE)) {
					symbol = piece.isCrowned() ? 'W' : 'w';
				} else {
					symbol = '_';
				}
				
				last_index = index;
				index += counting_upwards ? 1 : -1;
			} else { // invalid square
				symbol = ' ';
			}
			
			std::cout << symbol << ' ';
		}
		
		std::cout << "| " << std::setw(2) << last_index + 1 << " \n";
	}

	std::cout << "     -----------------     \n";
	
	std::cout.fill(previous_fill_character); // reset fill character
}


/**
 * Prints whose turn it is.
 */
void Tui::printTurn() const {
	std::cout << "It is " << (m_game.getTurn() == Turn::BLACK ? "black" : "white") << "'s turn to move.\n";
}


/**
 * Prints a list of the moves available.
 */
void Tui::printMovesAvailable() const {
	const std::vector<Move> &moves = m_game.getAvailableMoves();

	if (moves.size() == 0) {
		std::cout << "There are no moves available.\n";
		return;
	}

	std::cout << "Moves available: ";
	
	for (int move_index = 0; move_index < static_cast<int>(moves.size()); move_index++) {
		if (move_index > 0) {
			std::cout << ", ";
		}
		
		std::cout << getMoveString(moves[move_index]);
	}
	
	std::cout << '\n';
}


/**
 * Asks the user to enter a move to do and verifies that it is valid.
 * @return The move chosen. It will always be one of the currently available moves.
 */
Move Tui::askForMove() const {
	while (true) {
		std::cout << "Please enter the move to do: ";
		std::string input;
		std::getline(std::cin, input);
		
		std::vector<int> given_indexes = parseMoveString(input);
		
		const Move *matching_move = findMatchingMove(given_indexes, m_game.getAvailableMoves());
		
		if (matching_move != nullptr) {
			return *matching_move;
		}
	}
}


/**
 * Gets the computer player to chose the move it wants to do.
 * A thinking message is displayed to the user while the computer is thinking.
 * @return The move that the computer chose.
 */
Move Tui::getComputerMove() {
	std::cout << "The computer is thinking...";
	std::cout.flush();
	Move move = m_engine.findBestMove(m_game);
	std::cout << '\n';
	return move;
}


/**
 * Prints the move made.
 * @param turn Whose turn it was when the move was made.
 * @param move The move that was made.
 */
void Tui::printMoveMade(Turn turn, const Move &move) const {
	std::cout << (turn == Turn::BLACK ? "Black" : "White") << " makes the move " << getMoveString(move) << '\n';
}


/**
 * Prints the winner of the game.
 * Should only be called when the game is over.
 */
void Tui::printWinner() const {
	assert(m_game.isOver());
	std::cout << (m_game.getTurn() == Turn::WHITE ? "Black" : "White") << " wins!\n";
}


/**
 * Returns a string representation of the given move.
 * @param The move.
 * @return A string representation of the move.
 */
std::string Tui::getMoveString(const Move &move) {
	std::string output;

	for (int i = 0; i < move.getLength(); i++) {
		if (i > 0) {
			output += (move.isJump() ? 'x' : '-');
		}
		
		output += std::to_string(move.getPosition(i) + 1);
	}
	
	return output;
}


/**
 * Parses the given move string into a list of position index integers.
 * All characters that aren't numbers are considered separators.
 * Large numbers are capped at 32 (signifying an invalid index) to avoid overflow.
 * @param input The move string to be parsed.
 * @return A list of position indexes.
 */
std::vector<int> Tui::parseMoveString(const std::string &input) {
	std::vector<int> indexes;
	
	int number = 0;
	bool last_character_was_number = false;
	
	for (size_t i = 0; i <= input.length(); i++) {
		if (input[i] >= '0' && input[i] <= '9') {
			number = std::min(number * 10 + input[i] - '0', 33);
			last_character_was_number = true;
		} else if (last_character_was_number) {
			indexes.push_back(number - 1);
			number = 0;
			last_character_was_number = false;
		}
	}
	
	return indexes;
}


/**
 * Attempts to find the move that is being represented by the list of indexes given.
 * @param position_indexes The list of position indexes that make up the move.
 * Note that this list can contain just a subset of the positions of the move it represents,
 * as long as it is not ambiguous what move it is representing.
 * @param moves_available The list of moves that we can choose from.
 * @return A pointer to the move that was matched, or nullptr if no match could be made.
 */
const Move* Tui::findMatchingMove(const std::vector<int> &position_indexes, const std::vector<Move> &moves_available) {
	if (position_indexes.empty()) {
		return nullptr;
	}
	
	const Move *matching_move;
	int matches_found = 0;
	
	for (const Move &move : moves_available) { // test each move for match
		int indexes_position = 0;
		for (int moves_position = 0; moves_position < move.getLength(); moves_position++) {
			if (move.getPosition(moves_position) == position_indexes[indexes_position]) {
				indexes_position++;
				if (indexes_position == static_cast<int>(position_indexes.size())) { // found match
					if (static_cast<int>(position_indexes.size()) == move.getLength()) { // found exact match
						return &move;
					} // found partial match
					matching_move = &move;
					matches_found++;
					break;
				}
			}
		}
	}
	
	if (matches_found == 1) {
		return matching_move;
	} else {
		return nullptr;
	}
}
