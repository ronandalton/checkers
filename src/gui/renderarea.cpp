#include "gui/renderarea.h"

#include "game/coord.h"
#include "game/movegen.h"

#include <QPainter>
#include <QMouseEvent>


RenderArea::RenderArea(Game *game, Engine *engine, QWidget *parent)
	: m_game(game), m_engine(engine), QWidget(parent)
{
	m_pixmap_dark_square.load(":/images/dark_square.png");
	m_pixmap_light_square.load(":/images/light_square.png");
	m_pixmap_black_man.load(":/images/black_man.png");
	m_pixmap_black_king.load(":/images/black_king.png");
	m_pixmap_white_man.load(":/images/white_man.png");
	m_pixmap_white_king.load(":/images/white_king.png");
	m_pixmap_selected_piece_highlight.load(":images/selected_piece_highlight.png");
	m_pixmap_landing_square_highlight.load(":images/landing_square_highlight.png");

	m_moves_available = m_game->getAvailableMoves();
	m_board = m_game->getBoard();

	setFixedSize(m_sprite_size * BOARD_ROWS_COLS, m_sprite_size * BOARD_ROWS_COLS);
}


void RenderArea::mousePressEvent(QMouseEvent *event) {
	event->accept();

	Coord square_clicked_coord(event->position().x() / m_sprite_size,
		event->position().y() / m_sprite_size);
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

		// update m_moves_available by removing moves that no longer start with current hop path
		std::vector<Move> new_moves_available;
		for (const Move &move : m_moves_available) {
			if (static_cast<int>(move.getPositions().at(m_num_hops_made_in_current_move)) // cast needed?
					== m_currently_selected_square) {
				new_moves_available.push_back(move);
			}
		}
		m_moves_available = new_moves_available;

		// check if we are at a leaf node of the multi-jump "tree"
		if (m_num_hops_made_in_current_move == m_moves_available.at(0).getPositions().size() - 1) {
			// the move is over
			m_game->doMove(m_moves_available.at(0));
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
		m_moves_available = m_game->getAvailableMoves();
		m_num_hops_made_in_current_move = 0;
	} else { // something is currently selected
		if (!m_move_in_progress) {
			m_moves_available.clear();
			for (const Move &move : m_game->getAvailableMoves()) {
				if (move.getPositions().at(0) == m_currently_selected_square) {
					m_moves_available.push_back(move);
				}
			}
		}

		// calculate landing squares
		for (const Move &move : m_moves_available) {
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


void RenderArea::renderBoardBackground() {
	QPainter painter(this);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

	for (int y = 0; y < BOARD_ROWS_COLS; y++) {
		for (int x = 0; x < BOARD_ROWS_COLS; x++) {
			const QPixmap &pixmap = Coord(x, y).isValid() ? m_pixmap_dark_square : m_pixmap_light_square;
			painter.drawPixmap(QRect(x * m_sprite_size, y * m_sprite_size,
				m_sprite_size, m_sprite_size), pixmap, pixmap.rect());
		}
	}
}


void RenderArea::renderBoardPieces() {
	QPainter painter(this);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

	for (Position pos : Position::ALL_POSITIONS) {
		Piece piece = m_board.pieceAt(pos);

		if (const QPixmap *pixmap = getPiecePixmap(piece)) {
			painter.drawPixmap(QRect(pos.getX() * m_sprite_size, pos.getY() * m_sprite_size,
				m_sprite_size, m_sprite_size), *pixmap, pixmap->rect());
		}
	}
}


void RenderArea::renderBoardHighlights() {
	QPainter painter(this);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

	// render selected piece highlight
	if (m_currently_selected_square != -1) {
		Position pos(m_currently_selected_square);

		painter.drawPixmap(QRect(pos.getX() * m_sprite_size, pos.getY() * m_sprite_size,
			m_sprite_size, m_sprite_size), m_pixmap_selected_piece_highlight,
			m_pixmap_selected_piece_highlight.rect());
	}

	// render landing square highlights
	for (int landing_square : m_landing_squares) {
		Position position(landing_square);
		painter.drawPixmap(QRect(position.getX() * m_sprite_size,
			position.getY() * m_sprite_size, m_sprite_size, m_sprite_size),
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
