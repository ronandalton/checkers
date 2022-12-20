#include "gui/gui.h"

#include "gui/mainwindow.h"

#include <QApplication>


/**
 * Runs the game with a graphical user interface.
 */
int Gui::run(int argc, char *argv[]) {
	QApplication app(argc, argv);
	MainWindow window;
	window.show();
	window.setFixedSize(window.size()); // prevent window being resized
	return app.exec();
}
