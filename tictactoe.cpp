#include <qpainter.h>
#include <qlist.h>
#include <cstdlib>
#include "tictactoe.h"

#define DATA(point) data.pos[point.y()][point.x()]

const QColor TicTacToe::playerColors[2] = {red, blue};

void TicTacToe::paintPoint(QPainter *painter, const QRect& rect, const int player)
{
	int ds = rect.width() / 5;
	if (player == O) {
		painter->drawEllipse(rect);
		painter->setPen(NoPen);
		painter->setBrush(COLOR_BOARD);
		painter->drawEllipse(QRect(rect.topLeft() + QPoint(ds, ds), rect.size() - QSize(ds, ds) * 2));
	} else if (player == X) {
		int x = rect.x(), y = rect.y(), s = rect.width();
		ds = (int)((float)ds / 1.5);
		QPointArray points;
		// 3 points per line:	Point 1			Point 2				Point 3
		points.putPoints(0, 3,	x + ds, y,		x + s / 2, y + s / 2 - ds,	x + s - ds, y);
		points.putPoints(3, 3,	x + s, y + ds,		x + s / 2 + ds, y + s / 2,	x + s, y + s - ds);
		points.putPoints(6, 3,	x + s - ds, y + s,	x + s / 2, y + s / 2 + ds,	x + ds, y + s);
		points.putPoints(9, 3,	x, y + s - ds,		x + s / 2 - ds, y + s / 2,	x, y + ds);
		painter->drawPolygon(points);
	} else
		BoardGame::paintPoint(painter, rect, player);
}

QPoint TicTacToe::ai(const int player)
{
	int l = winLength(), res[l][l];
	for (int r = 0; r < l; r++)
		for (int c = 0; c < l; c++) {
			res[r][c] = -1;
			if (data.pos[r][c] != -1)
				continue;
			res[r][c] = 0;
			QPoint pos(c, r), direction(1, 0);
			if (count(pos, direction, player) + count(pos, -direction, player) == l - 1)
				return pos;
			if (count(pos, direction, !player) + count(pos, -direction, !player) == l - 1)
				res[r][c] += 100;
			direction = QPoint(1, 1);
			if (count(pos, direction, player) + count(pos, -direction, player) == l - 1)
				return pos;
			if (count(pos, direction, !player) + count(pos, -direction, !player) == l - 1)
				res[r][c] += 100;
			direction = QPoint(0, 1);
			if (count(pos, direction, player) + count(pos, -direction, player) == l - 1)
				return pos;
			if (count(pos, direction, !player) + count(pos, -direction, !player) == l - 1)
				res[r][c] += 100;
			direction = QPoint(-1, 1);
			if (count(pos, direction, player) + count(pos, -direction, player) == l - 1)
				return pos;
			if (count(pos, direction, !player) + count(pos, -direction, !player) == l - 1)
				res[r][c] += 100;
			/*if (r == size().height() / 2 && c == size().width() / 2)
				res[r][c] += 50;*/
		}
	QPoint pos(-1, -1);
	QList<QPoint> posList;
	posList.setAutoDelete(true);
	for (int r = 0; r < l; r++)
		for (int c = 0; c < l; c++) {
			if (res[r][c] == -1)
				continue;
			posList.append(new QPoint(c, r));
			if (r == size().height() / 2 && c == size().width() / 2)
				for (int i = 0; i < 5; i++)
					posList.append(new QPoint(c, r));
			if (pos.x() == -1) {
				pos = QPoint(c, r);
				continue;
			}
			if (res[pos.y()][pos.x()] > res[r][c])
				continue;
			pos = QPoint(c, r);
		}
	if (pos.x() == -1 || res[pos.y()][pos.x()] == 0)
		pos = *posList.at(rand() % posList.count());
	return pos;
}
