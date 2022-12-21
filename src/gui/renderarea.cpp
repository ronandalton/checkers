#include "gui/renderarea.h"

#include "game/coord.h"

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

	setFixedSize(m_sprite_size * BOARD_ROWS_COLS, m_sprite_size * BOARD_ROWS_COLS);
}


void RenderArea::mousePressEvent(QMouseEvent *event) {
	event->accept();

	int previously_selected_square = m_currently_selected_square;
	m_currently_selected_square = -1;

	Coord clicked_square_position(event->position().x() / m_sprite_size,
		event->position().y() / m_sprite_size);

	int square_clicked = clicked_square_position.isValid()
		? static_cast<int>(clicked_square_position.getPosition())
		: -1;

	if (square_clicked == -1) {
		return;
	}

	const Move *move_to_make = nullptr;

	if (previously_selected_square != -1) {
		for (const Move &move : m_game->getAvailableMoves()) {
			if (move.getStartPosition() == previously_selected_square
					&& move.getPosition(1) == square_clicked) {
				move_to_make = &move;
				break;
			}
		}
		// TODO: handle double jumps
	}

	if (move_to_make != nullptr) {
		m_game->doMove(*move_to_make);
	} else {
		if (square_clicked != previously_selected_square) {
			Turn turn = m_game->getTurn();
			if (m_game->getBoard().pieceAt(square_clicked).belongsTo(turn)) {
				m_currently_selected_square = square_clicked;
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
		Piece piece = m_game->getBoard().pieceAt(pos);

		if (const QPixmap *pixmap = getPiecePixmap(piece)) {
			painter.drawPixmap(QRect(pos.getX() * m_sprite_size, pos.getY() * m_sprite_size,
				m_sprite_size, m_sprite_size), *pixmap, pixmap->rect());
		}
	}
}


void RenderArea::renderBoardHighlights() {
	QPainter painter(this);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

	if (m_currently_selected_square != -1) {
		Position pos(m_currently_selected_square);

		// render selected piece highlight
		painter.drawPixmap(QRect(pos.getX() * m_sprite_size, pos.getY() * m_sprite_size,
			m_sprite_size, m_sprite_size), m_pixmap_selected_piece_highlight,
			m_pixmap_selected_piece_highlight.rect());

		// render landing square highlights
		for (const Move &move : m_game->getAvailableMoves()) {
			if (move.getStartPosition() == m_currently_selected_square) {
				Position next_pos = move.getPosition(1);

				painter.drawPixmap(QRect(next_pos.getX() * m_sprite_size,
					next_pos.getY() * m_sprite_size, m_sprite_size, m_sprite_size),
					m_pixmap_landing_square_highlight,
					m_pixmap_landing_square_highlight.rect());
			}
		}
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
