#ifndef RENDERAREA_H
#define RENDERAREA_H


#include <QWidget>
#include <QPixmap>

#include "game/game.h"
#include "engine/engine.h"


class RenderArea : public QWidget {
	Q_OBJECT

public:
	explicit RenderArea(Game *game, Engine *engine, QWidget *parent = nullptr);

protected:
	void paintEvent(QPaintEvent *event) override;

private:
	void renderBoardBackground();
	void renderBoardPieces();
	QPixmap* getPiecePixmap(Piece piece);

	static constexpr int BOARD_ROWS_COLS = 8;
	int m_sprite_size = 64;

	Game *m_game;
	Engine *m_engine;

	QPixmap m_pixmap_dark_square;
	QPixmap m_pixmap_light_square;
	QPixmap m_pixmap_black_man;
	QPixmap m_pixmap_black_king;
	QPixmap m_pixmap_white_man;
	QPixmap m_pixmap_white_king;
};


#endif // RENDERAREA_H
