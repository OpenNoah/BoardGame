#include <qlist.h>
#include <cstdlib>
#include "reversi.h"

#define DATA(point)	data.pos[(point).y()][(point).x()]

void Reversi::reset(void)
{
	BoardGame::reset();
	data.pos[3][3] = White;
	data.pos[4][4] = White;
	data.pos[3][4] = Black;
	data.pos[4][3] = Black;
}

int Reversi::check(const QPoint& point, const QPoint& direction, const int player) const
{
	int cnt;
	if ((cnt = count(point + direction, direction, !player)) != 0) {
		cnt++;
		if (pointValid(point + cnt * direction) && DATA(point + cnt * direction) == player)
			return cnt - 1;
	}
	return 0;
}

bool Reversi::available(const QPoint& point, const int player) const
{
	if (DATA(point) != -1)
		return false;
	for (int d = 1; d > -2; d -= 2) {
		QPoint direction;
		direction = QPoint(1, 0) * d;
		if (check(point, direction, player))
			return true;
		direction = QPoint(1, 1) * d;
		if (check(point, direction, player))
			return true;
		direction = QPoint(0, 1) * d;
		if (check(point, direction, player))
			return true;
		direction = QPoint(-1, 1) * d;
		if (check(point, direction, player))
			return true;
	}
	return false;
}

QPointArray Reversi::availableSpaces(const int player) const
{
	QPointArray spaces;
	for (int r = 0; r < size().height(); r++)
		for (int c = 0; c < size().width(); c++)
			if (available(QPoint(c, r), player))
				spaces.putPoints(spaces.size(), 1, c, r);
	return spaces;
}

void Reversi::reverse(const QPoint& point, const QPoint& direction, const int cnt, const int player)
{
	QPoint p(point);
	for (int c = cnt + 1; c--; p += direction)
		DATA(p) = player;
}

bool Reversi::place(const QPoint& point, const int player)
{
	if (DATA(point) != -1)
		return false;
	for (int d = 1; d > -2; d -= 2) {
		int cnt;
		QPoint direction;
		direction = QPoint(1, 0) * d;
		if ((cnt = check(point, direction, player)) != 0)
			reverse(point, direction, cnt, player);
		direction = QPoint(1, 1) * d;
		if ((cnt = check(point, direction, player)) != 0)
			reverse(point, direction, cnt, player);
		direction = QPoint(0, 1) * d;
		if ((cnt = check(point, direction, player)) != 0)
			reverse(point, direction, cnt, player);
		direction = QPoint(-1, 1) * d;
		if ((cnt = check(point, direction, player)) != 0)
			reverse(point, direction, cnt, player);
	}
	if (DATA(point) == -1)
		return false;
	return true;
}

int Reversi::win(void)
{
	bool spaces = false;
	int cnt[playerCount()];
	for (int i = 0; i < playerCount(); i++)
		cnt[i] = 0;
	for (int r = 0; r < size().height(); r++)
		for (int c = 0; c < size().width(); c++) {
			if (data.pos[r][c] == -1) {
				spaces = true;
				continue;
			}
			cnt[data.pos[r][c]]++;
		}
	if (cnt[Black] == 0)
		return White;
	if (cnt[White] == 0)
		return Black;
	if (spaces)
		return -1;
	if (cnt[Black] == cnt[White])
		return -2;
	return cnt[Black] > cnt[White] ? Black : White;
}

int Reversi::reversible(const QPoint& point, const int player)
{
	int cnt = 0;
	for (int d = 1; d > -2; d -= 2) {
		QPoint direction;
		direction = QPoint(1, 0) * d;
		cnt += check(point, direction, player);
		direction = QPoint(1, 1) * d;
		cnt += check(point, direction, player);
		direction = QPoint(0, 1) * d;
		cnt += check(point, direction, player);
		direction = QPoint(-1, 1) * d;
		cnt += check(point, direction, player);
	}
	return cnt;
}

QPoint Reversi::ai(const int player)
{
	const int choices[8][8] = {
		{100,	-100,	50,	30,	30,	50,	-100,	100,},
		{-100,	-100,	-50,	-50,	-50,	-50,	-100,	-100,},
		{50,	-50,	10,	10,	10,	10,	-50,	50,},
		{30,	-50,	10,	0,	0,	10,	-50,	30,},
		{30,	-50,	10,	0,	0,	10,	-50,	30,},
		{50,	-50,	10,	10,	10,	10,	-50,	50,},
		{-100,	-100,	-50,	-50,	-50,	-50,	-100,	-100,},
		{100,	-100,	50,	30,	30,	50,	-100,	100,},
	};
	QPointArray spaces = availableSpaces(player);
	if (spaces.size() == 0)
		return BoardGame::ai(player);
	int values[spaces.size()];
	for (unsigned int i = 0; i < spaces.size(); i++) {
		QPoint p(spaces[i]);
		values[i] = choices[p.y()][p.x()] + reversible(p, player) * 5;
	}
	int max = 0;
	for (unsigned int i = 0; i < spaces.size(); i++)
		if (values[i] > values[max])
			max = i;
	QList<int> equals;
	equals.setAutoDelete(true);
	for (unsigned int i = 0; i < spaces.size(); i++)
		if (values[i] == values[max])
			equals.append(new int(i));
	return spaces[*equals.at(rand() % equals.count())];
}
