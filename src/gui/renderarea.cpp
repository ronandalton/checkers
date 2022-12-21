#include "gui/renderarea.h"

#include "game/coord.h"

#include <QPainter>


RenderArea::RenderArea(Game *game, Engine *engine, QWidget *parent)
	: m_game(game), m_engine(engine), QWidget(parent)
{
	m_pixmap_dark_square.load(":/images/dark_square.png");
	m_pixmap_light_square.load(":/images/light_square.png");
	m_pixmap_black_man.load(":/images/black_man.png");
	m_pixmap_black_king.load(":/images/black_king.png");
	m_pixmap_white_man.load(":/images/white_man.png");
	m_pixmap_white_king.load(":/images/white_king.png");

	setFixedSize(m_sprite_size * BOARD_ROWS_COLS, m_sprite_size * BOARD_ROWS_COLS);
}


void RenderArea::paintEvent(QPaintEvent *event) {
	renderBoardBackground();
	renderBoardPieces();
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
