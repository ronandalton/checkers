#include "tui/tui.h"

#include "game/game.h"
#include "engine/engine.h"
#include "game/move.h"
#include "game/turn.h"
#include "game/player.h"
#include "game/matchtype.h"

#include <cassert>
#include <iomanip> // for number padding
#include <algorithm> // for std::min
#include <iostream>


// welcomes the user to the game
// an extra new line is printed afterwards
void Tui::printIntro() {
	std::cout << "Welcome to the Checkers/Draughts game!\n";
	std::cout << '\n';
}


// asks user what type of game they want to play and returns the appropriate match type
// options are human vs human, human vs computer, computer vs human, computer vs computer
MatchType Tui::askForMatchType() {
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


// prints the player type of each player in the game eg. human or computer
// an extra new line is printed afterwards
void Tui::printMatchType(const Game &game) {
	std::cout << "Match type: ";
	
	for (Turn turn : {Turn::BLACK, Turn::WHITE}) {
		if (game.getPlayerType(turn) == Player::HUMAN) {
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


// if the game is not over, the board, turn and moves available are printed
// otherwise the board and winner of the game is printed
// an extra new line is printed afterwards
void Tui::printGameState(const Game &game) {
	printBoard(game);
	if (!game.isOver()) {
		printTurn(game);
		printMovesAvailable(game);
	} else {
		std::cout << '\n';
		printWinner(game);
	}
	
	std::cout << '\n';
}


// gets the next move, prints the move, and then does the move
// the move is got from the user or engine depending on the current player type
// an extra new line is printed afterwards
void Tui::doNextMove(Game *game, Engine *engine) {
	Move move;
	
	if (game->getPlayerType(game->getTurn()) == Player::HUMAN) {
		move = askForMove(game->getAvailableMoves());
	} else {
		move = getComputerMove(*game, engine);
	}
	
	printMoveMade(game->getTurn(), move);
	
	bool move_successful = game->doMove(move);
	
	assert(move_successful);
	
	std::cout << '\n';
}


// asks user if they want to play again and returns true if they do
// an extra new line is printed afterwards
bool Tui::askToPlayAgain() {
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


// thanks the user for playing
void Tui::printOutro() {
	std::cout << "Thanks for playing!\n";
}


void Tui::printBoard(const Game &game) { // TODO: update to make use of new utility functions like coord.isValid()
	char old_fill_character = std::cout.fill('0'); // set fill character to '0' and remember old one
	
	std::cout << "     -----------------     \n";
	
	bool counting_upwards = game.getMatchType() == MatchType::COMPUTER_VS_HUMAN ? true : false;
	
	int index = counting_upwards ? 0 : 31;
	
	for (int y = 0; y < 8; y++) {
		std::cout << ' ' << std::setw(2) << index + 1 << " | ";
		
		int last_index;
		
		for (int x = 0; x < 8; x++) {
			char symbol;
			
			if ((x + y) % 2 == 1) { // is valid square
				Piece piece = game.getBoard().pieceAt(index);
				
				if (piece.belongsTo(Turn::BLACK)) {
					symbol = piece.isCrowned() ? 'B' : 'b';
				} else if (piece.belongsTo(Turn::WHITE)) {
					symbol = piece.isCrowned() ? 'W' : 'w';
				} else {
					symbol = '_';
				}
				
				last_index = index;
				index += counting_upwards ? 1 : -1;
			} else { // is invalid square
				symbol = ' ';
			}
			
			std::cout << symbol << ' ';
		}
		
		std::cout << "| " << std::setw(2) << last_index + 1 << " \n";
	}

	std::cout << "     -----------------     \n";
	
	std::cout.fill(old_fill_character); // reset fill character
}


void Tui::printTurn(const Game &game) {
	std::cout << "It is " << (game.getTurn() == Turn::BLACK ? "black" : "white") << "'s turn to move.\n";
}


void Tui::printMovesAvailable(const Game &game) {
	std::cout << "Moves available: ";
	
	const std::vector<Move> &moves = game.getAvailableMoves();
	
	for (int move_index = 0; move_index < static_cast<int>(moves.size()); move_index++) {
		if (move_index > 0) {
			std::cout << ", ";
		}
		
		std::cout << getMoveString(moves[move_index]);
	}
	
	std::cout << '\n';
}


// asks the user to enter a move and verifies that it is in the moves list
// returns a copy of the move chosen from the moves list
Move Tui::askForMove(const std::vector<Move> &moves) {
	while (true) {
		std::cout << "Please enter the move to do: ";
		std::string input;
		std::getline(std::cin, input);
		
		std::vector<int> given_indexes = parseMoveString(input);
		
		Move matching_move = findMatchingMove(given_indexes, moves);
		
		if (matching_move.exists()) {
			return matching_move;
		}
	}
}


// display a message saying that the computer is thinking
// query the engine for the best move and return the move chosen
Move Tui::getComputerMove(const Game &game, Engine *engine) {
	std::cout << "The computer is thinking...";
	std::cout.flush();
	assert(engine);
	Move move = engine->findBestMove(game);
	std::cout << '\n';
	return move;
}


// print the action of making a move by the player whose turn it is
void Tui::printMoveMade(Turn turn, const Move &move) {
	std::cout << (turn == Turn::BLACK ? "Black" : "White") << " makes the move " << getMoveString(move) << '\n';
}


void Tui::printWinner(const Game &game) {
	assert(game.isOver());
	std::cout << (game.getTurn() == Turn::WHITE ? "Black" : "White") << " wins!\n";
}


// constructs a string representing the move and returns it
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


// parses input which is a string with numbers separated by non-digit characters
// returns list of numbers representing the indexes of the positions making up the move
// note that returned values are the indexes of the positions and not their display values
// note that numbers in the input that are invalid positions will have an invalid index in the output
// ie. the function returns a list of numbers in input, subtracting one from each
std::vector<int> Tui::parseMoveString(const std::string &input) {
	std::vector<int> indexes;
	
	int number = 0;
	bool last_character_was_number = false;
	
	for (int i = 0; i <= static_cast<int>(input.length()); i++) {
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


// finds the move that is represented by indexes in moves list and returns a copy of it
// the indexes list may skip over some positions as long as the move it represents is unambiguous
// if a match is not found or there are multiple matches then an empty move is returned
Move Tui::findMatchingMove(const std::vector<int> &indexes, const std::vector<Move> &moves) {
	if (indexes.empty()) {
		return {};
	}
	
	Move matching_move;
	int matches_found = 0;
	
	for (const Move &move : moves) { // test each move for match
		int indexes_position = 0;
		for (int moves_position = 0; moves_position < move.getLength(); moves_position++) {
			if (move.getPosition(moves_position) == indexes[indexes_position]) {
				indexes_position++;
				if (indexes_position == static_cast<int>(indexes.size())) { // found match
					if (static_cast<int>(indexes.size()) == move.getLength()) { // found exact match
						return move;
					} // found partial match
					matching_move = move;
					matches_found++;
					break;
				}
			}
		}
	}
	
	if (matches_found == 1) {
		return matching_move;
	} else {
		return {};
	}
}
