#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H


#include "game/move.h"
#include "game/coord.h"

#include <QObject>
#include <vector>
#include <optional>


class GuiGameData;
class Game;
class Board;
class Renderer;
class QMouseEvent;


class InputHandler : public QObject {
	Q_OBJECT

public:
	InputHandler(GuiGameData *gui_game_data, Renderer *renderer);

	void handleMouseEvent(QMouseEvent *event);
	void resetState();
	bool isMoveInProgress() const;

signals:
	void repaintNeeded();
	void humanMoveMade();

private:
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
	void cancelMoveInProgress();

	Game *m_game;
	Board *m_board;
	std::optional<Coord> *m_currently_selected_square;
	std::vector<Coord> *m_landing_squares;
	Renderer *m_renderer;

	std::vector<Move> m_moves_available_subset;
	bool m_is_move_in_progress = false;
	int m_num_hops_made_in_current_move = 0;

	bool m_state_changed = false;
	bool m_human_move_made = false;
};


#endif // INPUT_HANDLER_H
