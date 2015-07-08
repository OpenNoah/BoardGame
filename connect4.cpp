#include "connect4.h"

#define DATA(point) data.pos[point.y()][point.x()]

const QColor Connect4::playerColors[2] = {yellow, red};

void Connect4::refresh(QPixmap *cache)
{
	QPainter painter(cache);
	drawBackground(cache, &painter);
	drawEdgeLines(cache, &painter);
	for (int r = 0; r < size().height(); r++)
		for (int c = 0; c < size().width(); c++)
			if (data.pos[r][c] != -1)
				drawPoint(cache, &painter, QPoint(c, r), data.pos[r][c]);
	if (data.last.x() != -1)
		drawLast(cache, &painter, data.last, DATA(data.last));
}

bool Connect4::place(const QPoint& point, const int player)
{
	int r;
	for (r = size().height() - 1; r >= 0; r--)
		if (data.pos[r][point.x()] == -1)
			break;
	if (r < 0)
		return false;
	data.pos[r][point.x()] = player;
	data.last = QPoint(point.x(), r);
	return true;
}
