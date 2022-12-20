#ifndef RENDERAREA_H
#define RENDERAREA_H


#include <QWidget>
#include <QPixmap>


class RenderArea : public QWidget {
	Q_OBJECT

public:
	explicit RenderArea(QWidget *parent = nullptr);

protected:
	void paintEvent(QPaintEvent *event) override;

private:
	static constexpr int BOARD_ROWS_COLS = 8;
	int m_sprite_size = 64;

	QPixmap m_pixmap_dark_square;
	QPixmap m_pixmap_light_square;
};


#endif // RENDERAREA_H
