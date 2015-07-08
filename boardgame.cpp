#include <qpainter.h>
#include <qfont.h>
#include "boardgame.h"

#define DATA(point)	data.pos[(point).y()][(point).x()]

const QColor BoardGame::playerColors[2] = {black, white};

void BoardGame::refresh(QPixmap *cache)
{
	if (gameName() == BoardGame::gameName()) {
		QPainter painter(cache);
		painter.setPen(NoPen);
		painter.setBrush(QBrush(white));
		painter.drawRect(cache->rect());
		painter.setPen(QPen(COLOR_LINE, 10));
		painter.setBrush(QBrush(COLOR_BOARD));
		painter.drawEllipse(cache->rect());
		QFont f("noah", 24);
		painter.setFont(f);
		painter.drawText(cache->rect(), AlignCenter, tr("Board Games!"));
		return;
	}
/*	generalRefresh(cache);
}

void BoardGame::generalRefresh(QPixmap *cache)
{*/
	QPainter painter(cache);
	drawBackground(cache, &painter);
	switch (boardType()) {
	case CenterLine:
		drawBoardLines(cache, &painter);
	case NoLine:
		break;
	case EdgeLine:
		drawEdgeLines(cache, &painter);
	}
	for (int r = 0; r < size().height(); r++)
		for (int c = 0; c < size().width(); c++)
			drawPoint(cache, &painter, QPoint(c, r), data.pos[r][c]);
	if (data.last.x() != -1)
		drawLast(cache, &painter, data.last, DATA(data.last));
}

void BoardGame::drawBackground(QPixmap *cache, QPainter *painter)
{
	painter->setPen(NoPen);
	painter->setBrush(QBrush(COLOR_BOARD));
	painter->drawRect(cache->rect());
}

void BoardGame::drawBoardLines(QPixmap *cache, QPainter *painter)
{
	float w = cache->width(), h = cache->height();
	float dw, dh;
	float us;
	if (square()) {
		float s = w < h ? w : h;
		dw = (w - s) / 2;
		dh = (h - s) / 2;
		us = (s / size().width());
		if (boardType() != CenterLine)
			us = (int)us;
		w = s;
		h = s;
	} else {
		us = ((w / size().width() < h / size().height() ? w / size().width() : h / size().height()));
		if (boardType() != CenterLine)
			us = (int)us;
		dw = (w - us * size().width()) / 2;
		dh = (h - us * size().height()) / 2;
		w = us * size().width();
		h = us * size().height();
	}
	painter->setPen(COLOR_LINE);
	painter->setBrush(NoBrush);
	for (int r = 0; r < size().height(); r++)
		painter->drawLine((int)(dw + us / 2), (int)(dh + us / 2 + us * r), \
				  (int)(dw + us / 2 + w - us), (int)(dh + us / 2 + us * r));
	for (int c = 0; c < size().height(); c++)
		painter->drawLine((int)(dw + us / 2 + us * c), (int)(dh + us / 2), \
				  (int)(dw + us / 2 + us * c), (int)(dh + us / 2 + h - us));
}

void BoardGame::drawEdgeLines(QPixmap *cache, QPainter *painter)
{
	painter->setPen(COLOR_LINE);
	painter->setBrush(NoBrush);
	for (int r = 0; r < size().height(); r++)
		for (int c = 0; c < size().width(); c++)
			painter->drawRect(pointRect(cache->size(), QPoint(c, r)));
}

QRect BoardGame::pointRect(const QSize& disp, const QPoint& point) const
{
	float w = disp.width(), h = disp.height();
	float dw, dh;
	float us;
	if (square()) {
		float s = w < h ? w : h;
		dw = (w - s) / 2;
		dh = (h - s) / 2;
		us = (s / size().width());
		if (boardType() != CenterLine)
			us = (int)us;
		w = s;
		h = s;
	} else {
		us = ((w / size().width() < h / size().height() ? w / size().width() : h / size().height()));
		if (boardType() != CenterLine)
			us = (int)us;
		dw = (w - us * size().width()) / 2;
		dh = (h - us * size().height()) / 2;
		w = us * size().width();
		h = us * size().height();
	}
	int r = point.y(), c = point.x();
	return QRect((int)(dw + us * c), (int)(dh + us * r), (int)us, (int)us);
}

QPoint BoardGame::findPoint(const QSize& disp, const QPoint& pos)
{
	float w = disp.width(), h = disp.height();
	float dw, dh;
	float us;
	if (square()) {
		float s = w < h ? w : h;
		dw = (w - s) / 2;
		dh = (h - s) / 2;
		us = (s / size().width());
		if (boardType() != CenterLine)
			us = (int)us;
		w = s;
		h = s;
	} else {
		us = ((w / size().width() < h / size().height() ? w / size().width() : h / size().height()));
		if (boardType() != CenterLine)
			us = (int)us;
		dw = (w - us * size().width()) / 2;
		dh = (h - us * size().height()) / 2;
		w = us * size().width();
		h = us * size().height();
	}
	QPoint p((int)(pos.x() - dw), (int)(pos.y() - dh));
	if (p.x() < 0 || p.y() < 0)
		return QPoint(-1, -1);
	p = QPoint((int)(p.x() / us), (int)(p.y() / us));
	if (p.x() >= size().width() || p.y() >= size().height())
		return QPoint(-1, -1);
	return p;
}

void BoardGame::drawPoint(QPixmap *cache, QPainter *painter, const QPoint& point, const int player)
{
	if (player == -1)
		return;
	painter->setPen(NoPen);
	painter->setBrush(QBrush(playerColor(player)));
	paintPoint(painter, pointRect(cache->size(), point), player);
}

void BoardGame::drawLast(QPixmap *cache, QPainter *painter, const QPoint& point, const int player)
{
	painter->setPen(QPen(playerColor(player), 0, DashLine));
	painter->setBrush(NoBrush);
	painter->drawRect(pointRect(cache->size(), point));
}

void BoardGame::drawHover(QPixmap *hover, QPainter *painter, const int player)
{
	drawBackground(hover, painter);
	painter->setPen(NoPen);
	painter->setBrush(QBrush(playerColor(player), Dense4Pattern));
	paintPoint(painter, hover->rect(), player);
}

void BoardGame::hoverPixmap(QPixmap *cache, QPixmap *hover, const int player)
{
	QRect rect = pointRect(cache->size(), QPoint(0, 0));
	hover->resize(rect.size());
	QPainter painter(hover);
	drawHover(hover, &painter, player);
	if (boardType() != EdgeLine)
		return;
	painter.setPen(COLOR_LINE);
	painter.setBrush(NoBrush);
	painter.drawRect(hover->rect());
}

void BoardGame::reset(void)
{
	data.last = QPoint(-1, -1);
	for (int r = 0; r < size().height(); r++)
		for (int c = 0; c < size().width(); c++)
			data.pos[r][c] = -1;
}

int BoardGame::count(const QPoint& point, const QPoint& direction, const int player) const
{
	int c;
	QPoint p(point);
	for (c = 0; pointValid(p) && DATA(p) == player; p += direction, c++);
	return c;
}

int BoardGame::win(void)
{
	bool space = false;
	for (int r = 0; r < size().height(); r++)
		for (int c = 0; c < size().width(); c++) {
			if (data.pos[r][c] == -1) {
				space = true;
				continue;
			}
			if (count(QPoint(c, r), QPoint(1, 0), data.pos[r][c]) >= winLength())
				return data.pos[r][c];
			if (count(QPoint(c, r), QPoint(1, 1), data.pos[r][c]) >= winLength())
				return data.pos[r][c];
			if (count(QPoint(c, r), QPoint(0, 1), data.pos[r][c]) >= winLength())
				return data.pos[r][c];
			if (count(QPoint(c, r), QPoint(-1, 1), data.pos[r][c]) >= winLength())
				return data.pos[r][c];
		}
	if (space)
		return -1;
	return -2;
}

bool BoardGame::place(const QPoint& point, const int player)
{
	if (DATA(point) != -1)
		return false;
	DATA(point) = player;
	data.last = point;
	return true;
}

QString BoardGame::winMessage(const int player, const QString msg) const
{
	if (player == -2)
		return msg;
	else
		return tr("Player %1: %2 won!").arg(player + 1).arg(msg);
}
