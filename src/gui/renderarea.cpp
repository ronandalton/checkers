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


void RenderArea::paintEvent(QPaintEvent *event) {
	QPainter painter(this);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

	renderBoardBackground(painter);
	renderBoardPieces(painter);
	renderBoardHighlights(painter);
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


void RenderArea::renderBoardBackground(QPainter &painter) {
	for (int y = 0; y < BOARD_ROWS_COLS; y++) {
		for (int x = 0; x < BOARD_ROWS_COLS; x++) {
			const QPixmap &pixmap = Coord(x, y).isValid()
				? m_pixmap_dark_square : m_pixmap_light_square;
			renderTile(painter, pixmap, Coord(x, y));
		}
	}
}


void RenderArea::renderBoardPieces(QPainter &painter) {
	for (Position pos : Position::ALL_VALID_POSITIONS) {
		Piece piece = m_board.pieceAt(pos);

		if (const QPixmap *pixmap = getPiecePixmap(piece)) {
			renderTile(painter, *pixmap, pos.getCoord());
		}
	}
}


void RenderArea::renderBoardHighlights(QPainter &painter) {
	renderCurrentlySelectedPieceHighlight(painter);
	renderLandingSquareHighlights(painter);
}


void RenderArea::renderCurrentlySelectedPieceHighlight(QPainter &painter) {
	if (m_currently_selected_square) {
		renderTile(painter, m_pixmap_selected_piece_highlight, *m_currently_selected_square);
	}
}


void RenderArea::renderLandingSquareHighlights(QPainter &painter) {
	for (Coord landing_square : m_landing_squares) {
		renderTile(painter, m_pixmap_landing_square_highlight, landing_square);
	}
}


void RenderArea::renderTile(QPainter &painter, const QPixmap &pixmap, Coord position) {
	int pixel_pos_x = position.getX() * SPRITE_SIZE;
	int pixel_pos_y = position.getY() * SPRITE_SIZE;

	QRect destination_rect(pixel_pos_x, pixel_pos_y, SPRITE_SIZE, SPRITE_SIZE);

	painter.drawPixmap(destination_rect, pixmap, pixmap.rect());
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
			int num_hops_in_move = move.getPositions().size() - 1;
			bool is_move_continuation = (num_hops_in_move > m_num_hops_made_in_current_move);

			if (is_move_continuation) {
				Coord landing_square = move.getPosition(m_num_hops_made_in_current_move + 1).getCoord();
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
}


void RenderArea::makeHop(Coord landing_square) {
	m_board.movePiece(*m_currently_selected_square, landing_square);
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
