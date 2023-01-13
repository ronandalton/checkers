#include "gui/input_handler.h"

#include "gui/game_manager.h"
#include "gui/renderer.h"
#include "gui/render_area.h"

#include <QMouseEvent>
#include <algorithm> // for std::find
#include <cassert>


InputHandler::InputHandler(GameManager *game_manager, Renderer *renderer, RenderArea *render_area) :
	m_game(&game_manager->getGameRef()),
	m_board(&game_manager->getBoardRef()),
	m_renderer(renderer),
	m_render_area(render_area)
{
	resetMovesAvailableSubset();
}


void InputHandler::handleMouseEvent(QMouseEvent *event) {
	event->accept();

	m_state_changed = false;

	if (event->type() == QEvent::Type::MouseButtonPress) {
		Coord square_clicked = m_renderer->getBoardSquareAtPosition(event->position());
		handleSquareClicked(square_clicked);
	}

	if (m_state_changed) {
		m_render_area->repaint();
	}
}


void InputHandler::resetState() {
	if (m_is_move_in_progress) {
		cancelMoveInProgress();
	} else {
		clearSelection();
	}
}


std::optional<Coord> InputHandler::getCurrentlySelectedSquare() const {
	return m_currently_selected_square;
}


const std::vector<Coord>& InputHandler::getLandingSquares() const {
	return m_landing_squares;
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

		m_state_changed = true; // FIXME: not all clicks lead to a state change
	}
}


bool InputHandler::currentlyAcceptingInput() const {
	return !m_game->isOver();
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
	return m_currently_selected_square
		&& *m_currently_selected_square == square_clicked;
}


bool InputHandler::isALandingSquare(Coord square_clicked) const {
	return std::find(m_landing_squares.begin(), m_landing_squares.end(),
		square_clicked) != m_landing_squares.end();
}


bool InputHandler::squareHoldsAPieceThatBelongsToCurrentPlayer(Coord square_clicked) const {
	if (!square_clicked.isValidPosition()) {
		return false;
	}

	Position position_clicked = square_clicked.getPosition();

	return m_board->pieceAt(position_clicked).belongsTo(m_game->getTurn());
}


void InputHandler::clearSelection() {
	m_currently_selected_square.reset();
	resetMovesAvailableSubset();
	updateLandingSquares();
}


void InputHandler::selectSquare(Coord square_clicked) {
	assert(!m_is_move_in_progress);
	assert(square_clicked.isValidPosition());

	resetMovesAvailableSubset();
	m_currently_selected_square = square_clicked;
	m_num_hops_made_in_current_move = 0;
	restrictMovesAvailableSubset();
	updateLandingSquares();
}


void InputHandler::resetMovesAvailableSubset() {
	m_moves_available_subset = m_game->getAvailableMoves();
}


void InputHandler::restrictMovesAvailableSubset() {
	assert(m_currently_selected_square);

	Position current_position = m_currently_selected_square->getPosition();

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
	m_landing_squares.clear();

	if (m_currently_selected_square) {
		Position currently_selected_position = m_currently_selected_square->getPosition();

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
		std::find(m_landing_squares.begin(), m_landing_squares.end(),
		landing_square) != m_landing_squares.end();

	if (!landing_square_already_in_list) {
		m_landing_squares.push_back(landing_square);
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
	m_board->movePiece(*m_currently_selected_square, landing_square);
	m_currently_selected_square = landing_square;
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
}


void InputHandler::cancelMoveInProgress() {
	m_is_move_in_progress = false;
	clearSelection();
	*m_board = m_game->getBoard();
}
