#ifndef RENDERAREA_H
#define RENDERAREA_H


#include <QWidget>
#include <QPixmap>
#include <vector>

#include "game/game.h"
#include "engine/engine.h"
#include "game/board.h"


class RenderArea : public QWidget {
	Q_OBJECT

public:
	explicit RenderArea(Game *game, Engine *engine, QWidget *parent = nullptr);

protected:
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void paintEvent(QPaintEvent *event) override;

private:
	void renderBoardBackground();
	void renderBoardPieces();
	void renderBoardHighlights();
	QPixmap* getPiecePixmap(Piece piece);

	static constexpr int BOARD_ROWS_COLS = 8;
	int m_sprite_size = 64;

	Game *m_game;
	Engine *m_engine;

	int m_currently_selected_square = -1;
	std::vector<int> m_landing_squares;
	bool m_move_in_progress = false;

	// the following variables are needed to store the game state
	// while the user is in the process of making a multi-jump move
	int m_num_hops_made_in_current_move = 0;
	std::vector<Move> m_moves_available;
	Board m_board;

	QPixmap m_pixmap_dark_square;
	QPixmap m_pixmap_light_square;
	QPixmap m_pixmap_black_man;
	QPixmap m_pixmap_black_king;
	QPixmap m_pixmap_white_man;
	QPixmap m_pixmap_white_king;
	QPixmap m_pixmap_selected_piece_highlight;
	QPixmap m_pixmap_landing_square_highlight;
};


#endif // RENDERAREA_H
