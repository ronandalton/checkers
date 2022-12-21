#include "gui/mainwindow.h"

#include "gui/renderarea.h"
#include "game/game.h"
#include "engine/engine.h"


MainWindow::MainWindow(Game *game, Engine *engine)
	: QMainWindow()
{
	setWindowTitle(tr("Checkers Game"));
	RenderArea *render_area = new RenderArea(game, engine, this);
	setCentralWidget(render_area);
}
