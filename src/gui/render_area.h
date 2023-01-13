#ifndef RENDER_AREA_H
#define RENDER_AREA_H


#include "gui/renderer.h"
#include "gui/input_handler.h"

#include <QWidget>


class GameManager;


class RenderArea : public QWidget {
	Q_OBJECT

public:
	RenderArea(QWidget *parent, GameManager *game_manager);

protected:
	void paintEvent(QPaintEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
	Renderer m_renderer;
	InputHandler m_input_handler;
};


#endif // RENDER_AREA_H
