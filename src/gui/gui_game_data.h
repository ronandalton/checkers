#ifndef GUI_GAME_DATA_H
#define GUI_GAME_DATA_H


#include "game/game.h"
#include "game/board.h"
#include "game/coord.h"

#include <optional>
#include <vector>


struct GuiGameData {
	Game game;
	Board board;
	std::optional<Coord> currently_selected_square;
	std::vector<Coord> landing_squares;
};


#endif // GUI_GAME_DATA_H
