#include <qpainter.h>
#include "gomoku.h"

#define DATA(point)	data.pos[point.y()][point.x()]

const QColor Gomoku::playerColors[3] = {black, white, QColor(255, 127, 39)};

void Gomoku::refresh(QPixmap *cache)
{
	QPainter painter(cache);
	drawBackground(cache, &painter);
	drawPoint(cache, &painter, QPoint(7, 7), specialColor());
	drawPoint(cache, &painter, QPoint(3, 3), specialColor());
	drawPoint(cache, &painter, QPoint(3, 11), specialColor());
	drawPoint(cache, &painter, QPoint(11, 3), specialColor());
	drawPoint(cache, &painter, QPoint(11, 11), specialColor());
	drawBoardLines(cache, &painter);
	for (int r = 0; r < size().height(); r++)
		for (int c = 0; c < size().width(); c++)
			if (data.pos[r][c] != -1)
				drawPoint(cache, &painter, QPoint(c, r), data.pos[r][c]);
	if (data.last.x() != -1)
		drawLast(cache, &painter, data.last, DATA(data.last));
}

QPoint Gomoku::ai(const int player)
{
	int qiju[2][15][15][8][2];  // Situation table
	int i, j, k, q, a = 1, b = 0;
	int y1 = 0, y2 = 0, x1 = 0, x2 = 0, x, y;
	int a1[15][15], a2[15][15];

	for (i = 0; i < 2; i++)
		for (j = 0; j < 15; j++)
			for (k = 0; k < 15; k++)
				for (q = 0; q < 8; q++)
					for (int d = 0; d < 2; d++)
						qiju[i][j][k][q][d] = 0;

	// Write situation table
	for (k = 0; k < 2; k++)
		for (i = 0; i < 15; i++)
			for (j = 0; j < 15; j++)
				if (data.pos[i][j] == -1)
					for (q = 0; q < 8; q++) {
						switch (q) {
						case 0:
							while (j - a >= 0 && data.pos[i][j - a] == k)
								a++;
							qiju[k][i][j][q][1] = data.pos[i][j - a] == -1;
							break;
						case 1:
							while (i - a >= 0 && j - a >= 0 && data.pos[i - a][j - a] == k)
								a++;
							qiju[k][i][j][q][1] = data.pos[i - a][j - a] == -1;
							break;
						case 2:
							while (i - a >= 0 && data.pos[i - a][j] == k)
								a++;
							qiju[k][i][j][q][1] = data.pos[i - a][j] == -1;
							break;
						case 3:
							while (i - a >= 0 && j + a <= 14 && data.pos[i - a][j + a] == k)
								a++;
							qiju[k][i][j][q][1] = data.pos[i - a][j + a] == -1;
							break;
						case 4:
							while (j + a <= 14 && data.pos[i][j + a] == k)
								a++;
							qiju[k][i][j][q][1] = data.pos[i][j + a] == -1;
							break;
						case 5:
							while (i + a <= 14 && j + a <= 14 && data.pos[i + a][j + a] == k)
								a++;
							qiju[k][i][j][q][1] = data.pos[i + a][j + a] == -1;
							break;
						case 6:
							while (i + a <= 14 && data.pos[i + a][j] == k)
								a++;
							qiju[k][i][j][q][1] = data.pos[i + a][j] == -1;
							break;
						case 7:
							while (i + a <= 14 && j - a >= 0 && data.pos[i + a][j - a] == k)
								a++;
							qiju[k][i][j][q][1] = data.pos[i + a][j - a] == -1;
							break;
						}
						qiju[k][i][j][q][0] = a - 1;
						a = 1;
					}

	// Get marks
	for (i = 0; i < 15; i++)
		for (j = 0; j < 15; j++) {
			k = 0;
			for (q = 0; q < 4; q++) {
				if ((qiju[k][i][j][q][0] + qiju[k][i][j][q + 4][0]) >= 4)
					b += k == player ? 50000 : 7000;
				if ((qiju[k][i][j][q][0] + qiju[k][i][j][q + 4][0]) == 3 && qiju[k][i][j][q][1] == 1 && qiju[k][i][j][q + 4][1] == 1)
					b += 301;
				if ((qiju[k][i][j][q][0] + qiju[k][i][j][q + 4][0]) == 2 && qiju[k][i][j][q][1] == 1 && qiju[k][i][j][q + 4][1] == 1)
					b += 43;
				if ((qiju[k][i][j][q][0] + qiju[k][i][j][q + 4][0]) == 1 && qiju[k][i][j][q][1] == 1 && qiju[k][i][j][q + 4][1] == 1)
					b += 11;
				if ((qiju[k][i][j][q][0] + qiju[k][i][j][q + 4][0]) == 3 && ((qiju[k][i][j][q][1] == 1 && qiju[k][i][j][q + 4][1] == 0) || (qiju[k][i][j][q][1] == 0 && qiju[k][i][j][q + 4][1] == 1)))
					b += 63;
				if ((qiju[k][i][j][q][0] + qiju[k][i][j][q + 4][0]) == 2 && ((qiju[k][i][j][q][1] == 1 && qiju[k][i][j][q + 4][1] == 0) || (qiju[k][i][j][q][1] == 0 && qiju[k][i][j][q + 4][1] == 1)))
					b += 6;
				if ((qiju[k][i][j][q][0] + qiju[k][i][j][q + 4][0]) == 1 && ((qiju[k][i][j][q][1] == 1 && qiju[k][i][j][q + 4][1] == 0) || (qiju[k][i][j][q][1] == 0 && qiju[k][i][j][q + 4][1] == 1)))
					b += 1;
			}
			if (b == 126 || b == 189 || b == 252)
				b = 1500;
			else if (b == 106)
				b = 1000;
			a1[i][j] = b;
			b = 0;
			k = 1;
			for (q = 0; q < 4; q++) {
				if ((qiju[k][i][j][q][0] + qiju[k][i][j][q + 4][0]) >= 4)
					b += k == player ? 50000 : 30000;
				if ((qiju[k][i][j][q][0] + qiju[k][i][j][q + 4][0]) == 3 && qiju[k][i][j][q][1] == 1 && qiju[k][i][j][q + 4][1] == 1)
					b += 1500;
				if ((qiju[k][i][j][q][0] + qiju[k][i][j][q + 4][0]) == 2 && qiju[k][i][j][q][1] == 1 && qiju[k][i][j][q + 4][1] == 1)
					b += 51;
				if ((qiju[k][i][j][q][0] + qiju[k][i][j][q + 4][0]) == 1 && qiju[k][i][j][q][1] == 1 && qiju[k][i][j][q + 4][1] == 1)
					b += 16;
				if ((qiju[k][i][j][q][0] + qiju[k][i][j][q + 4][0]) == 3 && ((qiju[k][i][j][q][1] == 1 && qiju[k][i][j][q + 4][1] == 0) || (qiju[k][i][j][q][1] == 0 && qiju[k][i][j][q + 4][1] == 1)))
					b += 71;
				if ((qiju[k][i][j][q][0] + qiju[k][i][j][q + 4][0]) == 2 && ((qiju[k][i][j][q][1] == 1 && qiju[k][i][j][q + 4][1] == 0) || (qiju[k][i][j][q][1] == 0 && qiju[k][i][j][q + 4][1] == 1)))
					b += 7;
				if ((qiju[k][i][j][q][0] + qiju[k][i][j][q + 4][0]) == 1 && ((qiju[k][i][j][q][1] == 1 && qiju[k][i][j][q + 4][1] == 0) || (qiju[k][i][j][q][1] == 0 && qiju[k][i][j][q + 4][1] == 1)))
					b += 2;
			}
			if (b == 142 || b == 213 || b == 284)
				b = 1500;
			else if (b == 122)
				b = 1300;
			a2[i][j] = b;
			b = 0;
		}

	// Get highest position
	for (i = 0; i < 15; i++)
		for (j = 0; j < 15; j++) {
			if (a1[y1][x1] < a1[i][j] || (a1[y1][x1] == 0 && data.pos[i][j] == -1)) {
				y1 = i;
				x1 = j;
			}
			if (a2[y2][x2] < a2[i][j] || (a2[y2][x2] == 0 && data.pos[i][j] == -1)) {
				y2 = i;
				x2 = j;
			}
		}
	if (a2[y2][x2] >= a1[y1][x1]) {
		x = x2;
		y = y2;
	} else {
		x = x1;
		y = y1;
	}
	if (data.last.x() == -1) {
		x = 7;
		y = 7;
	}
	return QPoint(x, y);
}
