#ifndef RENDERAREA_H
#define RENDERAREA_H


#include <QWidget>
#include <QPixmap>
#include <vector>
#include <optional>

#include "game/game.h"
#include "engine/engine.h"
#include "game/board.h"
#include "game/coord.h"


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
	void loadSprites();
	void renderBoardBackground();
	void renderBoardPieces();
	void renderBoardHighlights();
	QPixmap* getPiecePixmap(Piece piece);

	Coord getSquareClicked(const QMouseEvent *event) const;
	void handleSquareClicked(Coord square_clicked);
	bool currentlyAcceptingInput() const;
	void handleSquareClickedWhenNoMoveInProgress(Coord square_clicked);
	void handleSquareClickedWhenMoveAlreadyInProgress(Coord square_clicked);
	bool isCurrentlySelectedSquare(Coord square_clicked) const;
	bool isALandingSquare(Coord square_clicked) const;
	bool squareHoldsAPieceThatBelongsToCurrentPlayer(Coord square_clicked) const;
	void clearSelection();
	void selectSquare(Coord square_clicked);
	void resetMovesAvailableSubset();
	void restrictMovesAvailableSubset();
	void updateLandingSquares();
	void addLandingSquare(Coord landing_square);
	void handleLandingSquareClicked(Coord square_clicked);
	void startMove();
	void makeHop(Coord landing_square);
	bool isMoveOver() const;
	void endMove();

	static constexpr int BOARD_ROWS_COLS = 8;
	static constexpr int SPRITE_SIZE = 64;

	Game *m_game;
	Engine *m_engine;

	Board m_board;
	std::optional<Coord> m_currently_selected_square;
	std::vector<Move> m_moves_available_subset;
	std::vector<Coord> m_landing_squares;
	bool m_move_in_progress = false;
	int m_num_hops_made_in_current_move = 0;

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
