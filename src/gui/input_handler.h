#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H


#include "game/move.h"
#include "game/coord.h"

#include <vector>
#include <optional>


class GameManager;
class Game;
class Board;
class Renderer;
class RenderArea;
class QMouseEvent;


class InputHandler {
public:
	InputHandler(GameManager *game_manager, Renderer *renderer, RenderArea *render_area);

	void handleMouseEvent(QMouseEvent *event);
	void resetState();

	std::optional<Coord> getCurrentlySelectedSquare() const;
	const std::vector<Coord>& getLandingSquares() const;
	bool isMoveInProgress() const;

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
	Renderer *m_renderer;
	RenderArea *m_render_area;

	std::optional<Coord> m_currently_selected_square;
	std::vector<Move> m_moves_available_subset;
	std::vector<Coord> m_landing_squares;
	bool m_is_move_in_progress = false;
	int m_num_hops_made_in_current_move = 0;
	bool m_state_changed = false;
};


#endif // INPUT_HANDLER_H
