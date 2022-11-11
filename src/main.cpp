#include "tui/tui.h"
#include "game/game.h"
#include "engine/engine.h"

#include <memory>


int main() {
	Tui::printIntro();
	
	do {
		Game game;
		game.newGame(Tui::askForMatchType());
		Tui::printMatchType(game);
		
		std::unique_ptr<Engine> engine;
		if (game.requiresEngine()) {
			engine = std::make_unique<Engine>();
		}
		
		while (!game.isOver()) {
			Tui::printGameState(game);
			Tui::doNextMove(&game, engine.get());
		}
		
		Tui::printGameState(game);
	} while (Tui::askToPlayAgain());
	
	Tui::printOutro();
	
	return 0;
}
