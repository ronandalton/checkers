#include "mainwindow.h"

#include "renderarea.h"


MainWindow::MainWindow()
	: QMainWindow()
{
	setWindowTitle(tr("Checkers Game"));
	RenderArea *render_area = new RenderArea(this);
	setCentralWidget(render_area);
}
