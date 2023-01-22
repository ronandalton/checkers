#include "gui/render_area.h"

#include "gui/game_manager.h"
#include "gui/engine_thread.h"

#include <QPainter>


RenderArea::RenderArea(QWidget *parent, GameManager *game_manager) :
	QWidget(parent),
	m_renderer(game_manager->getGuiGameDataPtr()),
	m_input_handler(game_manager->getGuiGameDataPtr(), &m_renderer)
{
	QSize render_area_size = m_renderer.getRenderAreaSize();
	setFixedSize(render_area_size.width(), render_area_size.height());

	connect(&m_input_handler, &InputHandler::humanMoveMade,
		game_manager->getEngineThreadPtr(), &EngineThread::makeMovePerhaps,
		Qt::QueuedConnection);

	connect(game_manager, &GameManager::gameStarted,
		this, &RenderArea::repaintProxy, Qt::QueuedConnection);
	connect(&m_input_handler, &InputHandler::repaintNeeded,
		this, &RenderArea::repaintProxy, Qt::QueuedConnection);
	connect(game_manager->getEngineThreadPtr(), &EngineThread::engineMoveMade,
		this, &RenderArea::repaintProxy, Qt::QueuedConnection);
}


InputHandler* RenderArea::getInputHandlerPtr() {
	return &m_input_handler;
}


void RenderArea::paintEvent(QPaintEvent *event) {
	QPainter painter(this);

	m_renderer.renderBoard(&painter);
}


void RenderArea::mousePressEvent(QMouseEvent *event) {
	m_input_handler.handleMouseEvent(event);
}


void RenderArea::mouseReleaseEvent(QMouseEvent *event) {
	m_input_handler.handleMouseEvent(event);
}


void RenderArea::mouseMoveEvent(QMouseEvent *event) {
	m_input_handler.handleMouseEvent(event);
}


void RenderArea::mouseDoubleClickEvent(QMouseEvent *event) {
	m_input_handler.handleMouseEvent(event);
}


void RenderArea::repaintProxy() {
	// FIXME: this workaround shouldn't be needed
	repaint();
}
