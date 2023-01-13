#include "gui/render_area.h"

#include <QPainter>


RenderArea::RenderArea(QWidget *parent, GameManager *game_manager) :
	QWidget(parent),
	m_renderer(game_manager, &m_input_handler),
	m_input_handler(game_manager, &m_renderer, this)
{
	QSize render_area_size = m_renderer.getRenderAreaSize();
	setFixedSize(render_area_size.width(), render_area_size.height());
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
