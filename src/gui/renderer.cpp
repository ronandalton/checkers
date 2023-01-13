#include "gui/renderer.h"

#include "gui/game_manager.h"
#include "gui/input_handler.h"
#include "game/board.h"
#include "game/piece.h"

#include <QPainter>


Renderer::Renderer(GameManager *game_manager, InputHandler *input_handler) :
	m_board(&game_manager->getBoardRef()),
	m_input_handler(input_handler)
{
	loadSprites();
}


void Renderer::renderBoard(QPainter *painter) {
	painter->setRenderHint(QPainter::SmoothPixmapTransform, true);

	renderBoardBackground(painter);
	renderBoardPieces(painter);
	renderBoardHighlights(painter);
}


Coord Renderer::getBoardSquareAtPosition(QPointF point) const {
	return Coord(
		point.x() / SPRITE_SIZE,
		point.y() / SPRITE_SIZE
	);
}


QSize Renderer::getRenderAreaSize() const {
	int width_and_height = SPRITE_SIZE * BOARD_ROWS_COLS;
	return QSize(width_and_height, width_and_height);
}


void Renderer::loadSprites() {
	m_pixmap_dark_square.load(":/images/dark_square.png");
	m_pixmap_light_square.load(":/images/light_square.png");
	m_pixmap_black_man.load(":/images/black_man.png");
	m_pixmap_black_king.load(":/images/black_king.png");
	m_pixmap_white_man.load(":/images/white_man.png");
	m_pixmap_white_king.load(":/images/white_king.png");
	m_pixmap_selected_piece_highlight.load(":/images/selected_piece_highlight.png");
	m_pixmap_landing_square_highlight.load(":/images/landing_square_highlight.png");
}


void Renderer::renderBoardBackground(QPainter *painter) {
	for (int y = 0; y < BOARD_ROWS_COLS; y++) {
		for (int x = 0; x < BOARD_ROWS_COLS; x++) {
			const QPixmap &pixmap = Coord(x, y).isValidPosition()
				? m_pixmap_dark_square : m_pixmap_light_square;
			renderTile(painter, pixmap, Coord(x, y));
		}
	}
}


void Renderer::renderBoardPieces(QPainter *painter) {
	for (Position pos : Position::ALL_POSITIONS) {
		Piece piece = m_board->pieceAt(pos);

		QPixmap *pixmap = getPiecePixmap(piece);

		if (pixmap) {
			renderTile(painter, *pixmap, pos.getCoord());
		}
	}
}


void Renderer::renderBoardHighlights(QPainter *painter) {
	renderCurrentlySelectedPieceHighlight(painter);
	renderLandingSquareHighlights(painter);
}


void Renderer::renderCurrentlySelectedPieceHighlight(QPainter *painter) {
	std::optional<Coord> currently_selected_square = m_input_handler->getCurrentlySelectedSquare();
	if (currently_selected_square) {
		renderTile(painter, m_pixmap_selected_piece_highlight, *currently_selected_square);
	}
}


void Renderer::renderLandingSquareHighlights(QPainter *painter) {
	const std::vector<Coord> &landing_squares = m_input_handler->getLandingSquares();
	for (Coord landing_square : landing_squares) {
		renderTile(painter, m_pixmap_landing_square_highlight, landing_square);
	}
}


void Renderer::renderTile(QPainter *painter, const QPixmap &pixmap, Coord position) {
	int pixel_pos_x = position.getX() * SPRITE_SIZE;
	int pixel_pos_y = position.getY() * SPRITE_SIZE;

	QRect source_rectangle = pixmap.rect();
	QRect target_rectangle(pixel_pos_x, pixel_pos_y, SPRITE_SIZE, SPRITE_SIZE);

	painter->drawPixmap(target_rectangle, pixmap, source_rectangle);
}


QPixmap* Renderer::getPiecePixmap(Piece piece) {
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


