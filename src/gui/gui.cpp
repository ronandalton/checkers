#include "gui/gui.h"

#include "gui/mainwindow.h"

#include <QApplication>


/**
 * Runs the game with a graphical user interface.
 */
int Gui::run(int argc, char *argv[]) {
	m_game.newGame(MatchType::HUMAN_VS_HUMAN);

	QApplication app(argc, argv);
	MainWindow window(&m_game, &m_engine);
	window.show();
	window.setFixedSize(window.size()); // prevent window being resized
	return app.exec();
}
