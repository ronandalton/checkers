#include "gui/renderarea.h"

#include "game/coord.h"
#include "game/movegen.h"

#include <QPainter>
#include <QMouseEvent>
#include <algorithm> // for std::find
#include <cassert>


RenderArea::RenderArea(Game *game, Engine *engine, QWidget *parent)
	: m_game(game), m_engine(engine), QWidget(parent)
{
	loadSprites();

	resetMovesAvailableSubset();
	m_board = m_game->getBoard();

	setFixedSize(SPRITE_SIZE * BOARD_ROWS_COLS, SPRITE_SIZE * BOARD_ROWS_COLS);
}


void RenderArea::mousePressEvent(QMouseEvent *event) {
	event->accept();

	Coord square_clicked = getSquareClicked(event);
	handleSquareClicked(square_clicked);
}

void RenderArea::mouseReleaseEvent(QMouseEvent *event) {
	event->accept();
}


void RenderArea::mouseMoveEvent(QMouseEvent *event) {
	event->accept();
}


void RenderArea::paintEvent(QPaintEvent *event) {
	renderBoardBackground();
	renderBoardPieces();
	renderBoardHighlights();
}


void RenderArea::loadSprites() {
	m_pixmap_dark_square.load(":/images/dark_square.png");
	m_pixmap_light_square.load(":/images/light_square.png");
	m_pixmap_black_man.load(":/images/black_man.png");
	m_pixmap_black_king.load(":/images/black_king.png");
	m_pixmap_white_man.load(":/images/white_man.png");
	m_pixmap_white_king.load(":/images/white_king.png");
	m_pixmap_selected_piece_highlight.load(":images/selected_piece_highlight.png");
	m_pixmap_landing_square_highlight.load(":images/landing_square_highlight.png");
}


void RenderArea::renderBoardBackground() {
	QPainter painter(this);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

	for (int y = 0; y < BOARD_ROWS_COLS; y++) {
		for (int x = 0; x < BOARD_ROWS_COLS; x++) {
			const QPixmap &pixmap = Coord(x, y).isValid() ? m_pixmap_dark_square : m_pixmap_light_square;
			painter.drawPixmap(QRect(x * SPRITE_SIZE, y * SPRITE_SIZE,
				SPRITE_SIZE, SPRITE_SIZE), pixmap, pixmap.rect());
		}
	}
}


void RenderArea::renderBoardPieces() {
	QPainter painter(this);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

	for (Position pos : Position::ALL_VALID_POSITIONS) {
		Piece piece = m_board.pieceAt(pos);

		if (const QPixmap *pixmap = getPiecePixmap(piece)) {
			painter.drawPixmap(QRect(pos.getX() * SPRITE_SIZE, pos.getY() * SPRITE_SIZE,
				SPRITE_SIZE, SPRITE_SIZE), *pixmap, pixmap->rect());
		}
	}
}


void RenderArea::renderBoardHighlights() {
	QPainter painter(this);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

	// render selected piece highlight
	if (m_currently_selected_square != -1) {
		Position pos(m_currently_selected_square);

		painter.drawPixmap(QRect(pos.getX() * SPRITE_SIZE, pos.getY() * SPRITE_SIZE,
			SPRITE_SIZE, SPRITE_SIZE), m_pixmap_selected_piece_highlight,
			m_pixmap_selected_piece_highlight.rect());
	}

	// render landing square highlights
	for (int landing_square : m_landing_squares) {
		Position position(landing_square);
		painter.drawPixmap(QRect(position.getX() * SPRITE_SIZE,
			position.getY() * SPRITE_SIZE, SPRITE_SIZE, SPRITE_SIZE),
			m_pixmap_landing_square_highlight,
			m_pixmap_landing_square_highlight.rect());
	}
}


QPixmap* RenderArea::getPiecePixmap(Piece piece) {
	if (!piece.exists()) {
		return nullptr;
	}

	QPixmap *pixmap = nullptr;

	if (piece == Piece::BLACK_MAN) {
		pixmap = &m_pixmap_black_man;
	} else if (piece == Piece::BLACK_KING) {
		pixmap = &m_pixmap_black_king;
	} else if (piece == Piece::WHITE_MAN) {
		pixmap = &m_pixmap_white_man;
	} else if (piece == Piece::WHITE_KING) {
		pixmap = &m_pixmap_white_king;
	}

	return pixmap;
}


Coord RenderArea::getSquareClicked(const QMouseEvent *event) const {
	return Coord(
		event->position().x() / SPRITE_SIZE,
		event->position().y() / SPRITE_SIZE
	);
}


void RenderArea::handleSquareClicked(Coord square_clicked) {
	if (currentlyAcceptingInput()) {
		if (m_move_in_progress) {
			handleSquareClickedWhenMoveAlreadyInProgress(square_clicked);
		} else {
			handleSquareClickedWhenNoMoveInProgress(square_clicked);
		}

		repaint();
	}
}


bool RenderArea::currentlyAcceptingInput() const {
	return !m_game->isOver();
}


void RenderArea::handleSquareClickedWhenNoMoveInProgress(Coord square_clicked) {
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


void RenderArea::handleSquareClickedWhenMoveAlreadyInProgress(Coord square_clicked) {
	if (isALandingSquare(square_clicked)) {
		handleLandingSquareClicked(square_clicked);
	}
}


bool RenderArea::isCurrentlySelectedSquare(Coord square_clicked) const {
	return m_currently_selected_square
		&& *m_currently_selected_square == square_clicked;
}


bool RenderArea::isALandingSquare(Coord square_clicked) const {
	return std::find(m_landing_squares.begin(), m_landing_squares.end(),
		square_clicked) != m_landing_squares.end();
}


bool RenderArea::squareHoldsAPieceThatBelongsToCurrentPlayer(Coord square_clicked) const {
	if (!square_clicked.isValid()) {
		return false;
	}

	Position position_clicked = square_clicked.getPosition();

	return m_board.pieceAt(position_clicked).belongsTo(m_game->getTurn());
}

void RenderArea::clearSelection() {
	m_currently_selected_square.reset();
	resetMovesAvailableSubset();
	updateLandingSquares();
}


void RenderArea::selectSquare(Coord square_clicked) {
	assert(!m_move_in_progress);

	resetMovesAvailableSubset();
	m_currently_selected_square = square_clicked;
	m_num_hops_made_in_current_move = 0;
	restrictMovesAvailableSubset();
	updateLandingSquares();
}


void RenderArea::resetMovesAvailableSubset() {
	m_moves_available_subset = m_game->getAvailableMoves();
}


void RenderArea::restrictMovesAvailableSubset() {
	assert(m_currently_selected_square && m_currently_selected_square->isValid());

	Position current_position = m_currently_selected_square->getPosition();

	std::vector<Move> new_moves_available;

	for (const Move &move : m_moves_available_subset) {
		Position reference_position = move.getPosition(m_num_hops_made_in_current_move);

		if (reference_position == current_position) {
			new_moves_available.push_back(move);
		}
	}

	m_moves_available_subset = new_moves_available;
}


void RenderArea::updateLandingSquares() {
	m_landing_squares.clear();

	if (m_currently_selected_square) {
		Position currently_selected_position = m_currently_selected_square->getPosition();

		for (const Move &move : m_moves_available_subset) {
			if (move.getStartPosition() == currently_selected_position) {
				Coord landing_square = move.getPosition(1).getCoord();
				addLandingSquare(landing_square);
			}
		}
	}
}


void RenderArea::addLandingSquare(Coord landing_square) {
	bool landing_square_already_in_list =
		std::find(m_landing_squares.begin(), m_landing_squares.end(),
		landing_square) != m_landing_squares.end();

	if (!landing_square_already_in_list) {
		m_landing_squares.push_back(landing_square);
	}
}


void RenderArea::handleLandingSquareClicked(Coord square_clicked) {
	if (!m_move_in_progress) {
		startMove();
	}

	makeHop(square_clicked);

	if (isMoveOver()) {
		endMove();
	}
}


void RenderArea::startMove() {
	m_move_in_progress = true;
	m_num_hops_made_in_current_move = 0;
}


void RenderArea::makeHop(Coord landing_square) {
	m_board.movePiece(from_square, to_square);
	m_currently_selected_square = landing_square;
	m_num_hops_made_in_current_move++;
	restrictMovesAvailableSubset();
	updateLandingSquares();
}


bool RenderArea::isMoveOver() const {
	int moves_subset_size = m_moves_available_subset.size();

	if (moves_subset_size > 1) {
		return false;
	}

	int num_hops_in_move_left = m_moves_available_subset.at(0).getPositions().size() - 1;

	return m_num_hops_made_in_current_move == num_hops_in_move_left;
}


void RenderArea::endMove() {
	m_move_in_progress = false;
	m_game->doMove(m_moves_available_subset.at(0));
	clearSelection();

	assert(m_board == m_game->getBoard());
}


/*
	Coord square_clicked_coord(event->position().x() / SPRITE_SIZE,
		event->position().y() / SPRITE_SIZE);
	int position_clicked = square_clicked_coord.isValid()
		? static_cast<int>(square_clicked_coord.getPosition())
		: -1;

	bool clicked_landing_square = false;
	for (int landing_square : m_landing_squares) {
		if (position_clicked == landing_square) {
			clicked_landing_square = true;
			break;
		}
	}

	if (clicked_landing_square) {
		// make the move
		m_board = MoveGen::getBoardAfterSimpleMove(m_board, m_currently_selected_square, position_clicked);

		m_currently_selected_square = position_clicked;
		m_num_hops_made_in_current_move++;

		// update m_moves_available_subset by removing moves that no longer start with current hop path
		std::vector<Move> new_moves_available;
		for (const Move &move : m_moves_available_subset) {
			if (static_cast<int>(move.getPositions().at(m_num_hops_made_in_current_move)) // cast needed?
					== m_currently_selected_square) {
				new_moves_available.push_back(move);
			}
		}
		m_moves_available_subset = new_moves_available;

		// check if we are at a leaf node of the multi-jump "tree"
		if (m_num_hops_made_in_current_move == m_moves_available_subset.at(0).getPositions().size() - 1) {
			// the move is over
			m_game->doMove(m_moves_available_subset.at(0));
			m_board = m_game->getBoard(); // shouldn't be needed
			m_currently_selected_square = -1;
			m_move_in_progress = false;
		} else {
			m_move_in_progress = true;
		}
	} else { // didn't click a landing square
		// update the selection
		if (!m_move_in_progress) {
			if (position_clicked != -1
					&& m_board.pieceAt(position_clicked).belongsTo(m_game->getTurn())
					&& position_clicked != m_currently_selected_square) {
				m_currently_selected_square = position_clicked;
			} else {
				m_currently_selected_square = -1;
			}
		}
	}

	m_landing_squares.clear();
	if (m_currently_selected_square == -1) {
		m_moves_available_subset = m_game->getAvailableMoves();
		m_num_hops_made_in_current_move = 0;
	} else { // something is currently selected
		if (!m_move_in_progress) {
			m_moves_available_subset.clear();
			for (const Move &move : m_game->getAvailableMoves()) {
				if (move.getPositions().at(0) == m_currently_selected_square) {
					m_moves_available_subset.push_back(move);
				}
			}
		}

		// calculate landing squares
		for (const Move &move : m_moves_available_subset) {
			if (move.getPositions().size() - 1 >= m_num_hops_made_in_current_move + 1) {
				int new_landing_position = move.getPositions().at(m_num_hops_made_in_current_move + 1);
				bool landing_position_already_recorded = false;

				for (int landing_position : m_landing_squares) {
					if (new_landing_position == landing_position) {
						landing_position_already_recorded = true;
						break;
					}
				}

				if (!landing_position_already_recorded) {
					m_landing_squares.push_back(new_landing_position);
				}
			}
		}
	}

	repaint();
*/

