#include "gui/main_window.h"

#include "gui/render_area.h"


MainWindow::MainWindow() {
	setWindowTitle(tr("Checkers Game"));
	RenderArea *render_area = new RenderArea(this, &m_game_manager);
	setCentralWidget(render_area);
	m_game_manager.startGame();
}
