#include "gui/gui.h"

#include "gui/main_window.h"

#include <QApplication>


int Gui::run(int argc, char *argv[]) {
	QApplication app(argc, argv);
	MainWindow window;
	window.show();
	window.setFixedSize(window.size()); // prevent window from being resized
	return app.exec();
}
