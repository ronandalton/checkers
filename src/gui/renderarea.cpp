#include "renderarea.h"

#include <QPainter>


RenderArea::RenderArea(QWidget *parent)
	: QWidget(parent)
{
	m_pixmap_dark_square.load(":/images/dark_square.png");
	m_pixmap_light_square.load(":/images/light_square.png");

	setFixedSize(m_sprite_size * BOARD_ROWS_COLS, m_sprite_size * BOARD_ROWS_COLS);
}


void RenderArea::paintEvent(QPaintEvent *event) {
	QPainter painter(this);

	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

	for (int y = 0; y < BOARD_ROWS_COLS; y++) {
		for (int x = 0; x < BOARD_ROWS_COLS; x++) {
			const QPixmap &pixmap = (x + y) % 2 ? m_pixmap_dark_square : m_pixmap_light_square;
			painter.drawPixmap(QRect(x * m_sprite_size, y * m_sprite_size,
				m_sprite_size, m_sprite_size), pixmap, pixmap.rect());
		}
	}
}
