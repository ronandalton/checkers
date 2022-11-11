#include "game/game.h"

#include "game/player.h"
#include "game/piece.h"
#include "game/movegen.h"
#include "game/position.h"

#include <algorithm> // for std::find


// set up the board and member variables for a new game and set match type
void Game::newGame(MatchType match_type) {
	// set board to starting position
	for (Position position : Position::ALL_POSITIONS) {
		int row = position.getY();
		if (row < 3) {
			m_board.pieceAt(position) = Piece::BLACK_MAN;
		} else if (row < 5) {
			m_board.pieceAt(position) = Piece::NONE;
		} else {
			m_board.pieceAt(position) = Piece::WHITE_MAN;
		}
	}
	
	m_turn = Turn::BLACK;
	
	m_match_type = match_type;

	m_moves = MoveGen::generateMoves(m_board, m_turn);
}


// does move and gets game state ready for next turn
// returns false if move is invalid and nothing is done, true otherwise
bool Game::doMove(const Move &move) {
	if (std::find(m_moves.begin(), m_moves.end(), move) == m_moves.end()) {
		return false; // move was not in the valid moves list
	}
	
	m_board = MoveGen::getBoardAfterMove(m_board, move);
	
	// switch turns
	if (m_turn == Turn::BLACK) {
		m_turn = Turn::WHITE;
	} else {
		m_turn = Turn::BLACK;
	}
	
	m_moves = MoveGen::generateMoves(m_board, m_turn);
	
	return true;
}


// set board and update moves available
void Game::setBoard(const Board &board) {
	if (!(board == m_board)) {
		m_board = board;
		m_moves = MoveGen::generateMoves(m_board, m_turn);
	}
}


// set turn and update moves available if different
void Game::setTurn(Turn turn) {
	if (turn != m_turn) {
		m_turn = turn;
		m_moves = MoveGen::generateMoves(m_board, m_turn);
	}
}


void Game::setMatchType(MatchType match_type) {
	m_match_type = match_type;
}


const Board& Game::getBoard() const {
	return m_board;
}


Turn Game::getTurn() const {
	return m_turn;
}


const std::vector<Move>& Game::getAvailableMoves() const {
	return m_moves;
}


MatchType Game::getMatchType() const {
	return m_match_type;
}


bool Game::isOver() const {
	return m_moves.empty();
}


// returns player type of the player whose turn is specified
Player Game::getPlayerType(Turn turn) const {
	if (m_match_type == MatchType::HUMAN_VS_COMPUTER) {
		return turn == Turn::BLACK ? Player::HUMAN : Player::COMPUTER;
	} else if (m_match_type == MatchType::COMPUTER_VS_HUMAN) {
		return turn == Turn::BLACK ? Player::COMPUTER : Player::HUMAN;
	} else if (m_match_type == MatchType::HUMAN_VS_HUMAN) {
		return Player::HUMAN;
	} else { // is COMPUTER_VS_COMPUTER
		return Player::COMPUTER;
	}
}


// returns true if at least one player in the game is a computer
bool Game::requiresEngine() const {
	return getPlayerType(Turn::BLACK) == Player::COMPUTER || getPlayerType(Turn::WHITE) == Player::COMPUTER;
}
