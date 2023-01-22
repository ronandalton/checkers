#include "gui/input_handler.h"

#include "gui/gui_game_data.h"
#include "game/game.h"
#include "game/board.h"
#include "gui/renderer.h"
#include "game/player.h"

#include <QMouseEvent>
#include <algorithm> // for std::find
#include <cassert>


InputHandler::InputHandler(GuiGameData *gui_game_data, Renderer *renderer) :
	m_game(&gui_game_data->game),
	m_board(&gui_game_data->board),
	m_currently_selected_square(&gui_game_data->currently_selected_square),
	m_landing_squares(&gui_game_data->landing_squares),
	m_renderer(renderer)
{
	resetMovesAvailableSubset();
}


void InputHandler::handleMouseEvent(QMouseEvent *event) {
	event->accept();

	m_state_changed = false;
	m_human_move_made = false;

	if (event->type() == QEvent::Type::MouseButtonPress) {
		Coord square_clicked = m_renderer->getBoardSquareAtPosition(event->position());
		handleSquareClicked(square_clicked);
	}

	if (m_state_changed) {
		emit repaintNeeded();
	}

	if (m_human_move_made) {
		emit humanMoveMade();
	}
}


void InputHandler::resetState() {
	if (m_is_move_in_progress) {
		cancelMoveInProgress();
	} else {
		clearSelection();
	}
}


bool InputHandler::isMoveInProgress() const {
	return m_is_move_in_progress;
}


void InputHandler::handleSquareClicked(Coord square_clicked) {
	if (currentlyAcceptingInput()) {
		if (m_is_move_in_progress) {
			handleSquareClickedWhenMoveAlreadyInProgress(square_clicked);
		} else {
			handleSquareClickedWhenNoMoveInProgress(square_clicked);
		}

		m_state_changed = true; // FIXME/TODO: not all clicks lead to a state change
	}
}


bool InputHandler::currentlyAcceptingInput() const {
	return !m_game->isOver() && m_game->getPlayerType(m_game->getTurn()) == Player::HUMAN;
}


void InputHandler::handleSquareClickedWhenNoMoveInProgress(Coord square_clicked) {
	if (isCurrentlySelectedSquare(square_clicked)) {
		clearSelection();
	} else if (isALandingSquare(square_clicked)) {
		handleLandingSquareClicked(square_clicked);
	} else if (squareHoldsAPieceThatBelongsToCurrentPlayer(square_clicked)) {
		selectSquare(square_clicked);
	} else {
		clearSelection();
	}
}


void InputHandler::handleSquareClickedWhenMoveAlreadyInProgress(Coord square_clicked) {
	if (isALandingSquare(square_clicked)) {
		handleLandingSquareClicked(square_clicked);
	}
}


bool InputHandler::isCurrentlySelectedSquare(Coord square_clicked) const {
	return m_currently_selected_square->has_value()
		&& m_currently_selected_square->value() == square_clicked;
}


bool InputHandler::isALandingSquare(Coord square_clicked) const {
	return std::find(m_landing_squares->begin(), m_landing_squares->end(),
		square_clicked) != m_landing_squares->end();
}


bool InputHandler::squareHoldsAPieceThatBelongsToCurrentPlayer(Coord square_clicked) const {
	if (!square_clicked.isValidPosition()) {
		return false;
	}

	Position position_clicked = square_clicked.getPosition();

	return m_board->pieceAt(position_clicked).belongsTo(m_game->getTurn());
}


void InputHandler::clearSelection() {
	m_currently_selected_square->reset();
	resetMovesAvailableSubset();
	updateLandingSquares();
}


void InputHandler::selectSquare(Coord square_clicked) {
	assert(!m_is_move_in_progress);
	assert(square_clicked.isValidPosition());

	resetMovesAvailableSubset();
	*m_currently_selected_square = square_clicked;
	m_num_hops_made_in_current_move = 0;
	restrictMovesAvailableSubset();
	updateLandingSquares();
}


void InputHandler::resetMovesAvailableSubset() {
	m_moves_available_subset = m_game->getAvailableMoves();
}


void InputHandler::restrictMovesAvailableSubset() {
	assert(m_currently_selected_square->has_value());

	Position current_position = m_currently_selected_square->value().getPosition();

	std::vector<Move> new_moves_available_subset;

	for (const Move &move : m_moves_available_subset) {
		assert(m_num_hops_made_in_current_move < move.getPositions().size());

		Position test_position = move.getPosition(m_num_hops_made_in_current_move);

		if (test_position == current_position) {
			new_moves_available_subset.push_back(move);
		}
	}

	m_moves_available_subset = new_moves_available_subset;
}


void InputHandler::updateLandingSquares() {
	m_landing_squares->clear();

	if (m_currently_selected_square->has_value()) {
		Position currently_selected_position = m_currently_selected_square->value().getPosition();

		for (const Move &move : m_moves_available_subset) {
			int num_hops_in_move = move.getPositions().size() - 1;
			bool is_move_continuation = (num_hops_in_move > m_num_hops_made_in_current_move);

			if (is_move_continuation) {
				Coord landing_square = move.getPosition(m_num_hops_made_in_current_move + 1).getCoord();
				addLandingSquare(landing_square);
			}
		}
	}
}


void InputHandler::addLandingSquare(Coord landing_square) {
	bool landing_square_already_in_list =
		std::find(m_landing_squares->begin(), m_landing_squares->end(),
		landing_square) != m_landing_squares->end();

	if (!landing_square_already_in_list) {
		m_landing_squares->push_back(landing_square);
	}
}


void InputHandler::handleLandingSquareClicked(Coord square_clicked) {
	if (!m_is_move_in_progress) {
		startMove();
	}

	makeHop(square_clicked);

	if (isMoveOver()) {
		endMove();
	}
}


void InputHandler::startMove() {
	m_is_move_in_progress = true;
}


void InputHandler::makeHop(Coord landing_square) {
	m_board->movePiece(m_currently_selected_square->value(), landing_square);
	m_currently_selected_square->value() = landing_square;
	m_num_hops_made_in_current_move++;
	restrictMovesAvailableSubset();
	updateLandingSquares();
}


bool InputHandler::isMoveOver() const {
	int moves_subset_size = m_moves_available_subset.size();

	if (moves_subset_size != 1) {
		return false;
	}

	int num_hops_in_move_left = m_moves_available_subset[0].getPositions().size() - 1;

	return m_num_hops_made_in_current_move == num_hops_in_move_left;
}


void InputHandler::endMove() {
	m_is_move_in_progress = false;
	m_game->doMove(m_moves_available_subset.at(0));
	clearSelection();

	assert(*m_board == m_game->getBoard());

	m_human_move_made = true;
}


void InputHandler::cancelMoveInProgress() {
	m_is_move_in_progress = false;
	clearSelection();
	*m_board = m_game->getBoard();
}
