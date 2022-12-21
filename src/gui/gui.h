#ifndef GUI_H
#define GUI_H


#include "game/game.h"
#include "engine/engine.h"


class Gui {
public:
	int run(int argc, char *argv[]);

private:
	Game m_game;
	Engine m_engine;
};


#endif // GUI_H
