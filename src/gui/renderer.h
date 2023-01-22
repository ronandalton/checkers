#ifndef RENDERER_H
#define RENDERER_H


#include <QPixmap>
#include <vector>
#include <optional>


class GuiGameData;
class Piece;
class Coord;
class Board;
class QPainter;


class Renderer {
public:
	Renderer(GuiGameData *gui_game_data);

	void renderBoard(QPainter *painter);
	Coord getBoardSquareAtPosition(QPointF point) const;
	QSize getRenderAreaSize() const;

private:
	void loadSprites();
	void renderBoardBackground(QPainter *painter);
	void renderBoardPieces(QPainter *painter);
	void renderBoardHighlights(QPainter *painter);
	void renderCurrentlySelectedPieceHighlight(QPainter *painter);
	void renderLandingSquareHighlights(QPainter *painter);
	void renderTile(QPainter *painter, const QPixmap &pixmap, Coord position);
	QPixmap* getPiecePixmap(Piece piece);

	static constexpr int BOARD_ROWS_COLS = 8;
	static constexpr int SPRITE_SIZE = 64;

	Board *m_board;
	std::optional<Coord> *m_currently_selected_square;
	std::vector<Coord> *m_landing_squares;

	QPixmap m_pixmap_dark_square;
	QPixmap m_pixmap_light_square;
	QPixmap m_pixmap_black_man;
	QPixmap m_pixmap_black_king;
	QPixmap m_pixmap_white_man;
	QPixmap m_pixmap_white_king;
	QPixmap m_pixmap_selected_piece_highlight;
	QPixmap m_pixmap_landing_square_highlight;
};


#endif // RENDERER_H
