#include <qwmatrix.h>
#include <qlist.h>
#include <qlayout.h>
#include <cstdlib>
#include "minesweeper.h"

#define DATA(point)	data.pos[(point).y()][(point).x()]
#define MAX(a, b)	((a) > (b) ? (a) : (b))
#define MIN(a, b)	((a) < (b) ? (a) : (b))
#define MINUSIZE	16

bool Minesweeper::constructed = false;

const QColor Minesweeper::idColors[TotalIDs] = {
	QColor(0, 162, 232),	// Empty
	QColor(0, 162, 232),	// ID1
	QColor(181, 230, 29),	// ID2
	QColor(189, 15, 23),	// ID3
	QColor(63, 72, 204),	// ID4
	QColor(136, 0, 21),	// ID5
	QColor(34, 177, 76),	// ID6
	QColor(163, 73, 164),	// ID7
	QColor(237, 28, 36),	// ID8
	QColor(255, 0, 0),	// Mine
	QColor(255, 200, 100),	// FlagBG
	QColor(255, 0, 0),	// FlagFG
	QColor(0, 162, 232),	// Uncovered
	QColor(255, 255, 255),	// Background
};

void Minesweeper::reset(void)
{
	if (!constructed) {
		MinesweeperSelector selector(0);
		selector.setCaption(gameName());
		if (selector.exec() == QDialog::Accepted) {
			info.w = selector.sWidth->value();
			info.h = selector.sHeight->value();
			info.total = selector.sMines->value();
		} else {
			info.w = 9;
			info.h = 9;
			info.total = 64;
		}
		constructed = true;
	}
	info.remaining = info.total;
	info.first = true;
	info.died = false;
	data.last = QPoint(-1, -1);
	QList<QPoint> spaces;
	spaces.setAutoDelete(true);
	for (int r = 0; r < size().height(); r++)
		for (int c = 0; c < size().width(); c++) {
			data.pos[r][c] = UncoveredPosition;
			spaces.append(new QPoint(c, r));
		}
	while (info.remaining--) {
		int i = rand() % spaces.count();
		QPoint p(*spaces.at(i));
		spaces.remove(i);
		DATA(p) |= Mine;
	}
	info.remaining = info.total;
}

void Minesweeper::refresh(QPixmap *cache)
{
	if (info.flag.size() != pointRect(cache->size(), QPoint(0, 0)).size()) {
		info.flag.resize(pointRect(cache->size(), QPoint(0, 0)).size());
		info.mine.resize(pointRect(cache->size(), QPoint(0, 0)).size());

		QPainter painter(&info.flag);
		painter.setPen(NoPen);
		painter.setBrush(QBrush(idColor(FlagBG)));
		painter.drawRect(info.flag.rect());
		int x = 0, y = 0, s = info.flag.width();
		int ds = s / 10;
		QPointArray points;
		points.putPoints(0, 3,	x + s / 3, y + s / 4,	x + s * 2 / 3, y + s * 3 / 8,	x + s / 3, y + s / 2);
		painter.setBrush(QBrush(idColor(FlagFG)));
		painter.drawPolygon(points);
		painter.setBrush(QBrush(idColor(FlagFG).dark()));
		painter.drawRect(x + s / 3 - ds, y + s / 4, ds, s / 2);
		painter.end();

		painter.begin(&info.mine);
		painter.setPen(NoPen);
		painter.setBrush(QBrush(idColor(Background)));
		painter.drawRect(info.flag.rect());
		painter.setBrush(QBrush(idColor(FlagFG)));
		ds = s / 5;
		painter.drawEllipse(QRect(QPoint(ds, ds), info.mine.size() - QSize(ds, ds) * 2));
		ds = ds / 2;
		points.resize(0);
		points.putPoints(0, 4,	-ds, -s / 2,	ds, -s / 2,	ds, s / 2,	-ds, s / 2);
		QWMatrix conv;
		conv.translate(x + s / 2, y + s / 2);
		painter.drawPolygon(conv.map(points));
		conv.rotate(90);
		painter.drawPolygon(conv.map(points));
		conv.rotate(45);
		painter.drawPolygon(conv.map(points));
		conv.rotate(90);
		painter.drawPolygon(conv.map(points));
	}
	BoardGame::refresh(cache);
	//generalRefresh(cache);
}

QString Minesweeper::winMessage(const int player, const QString msg) const
{
	if (player == -2)
		return "Gameover.";
	return BoardGame::winMessage(player, msg);
}

QSize Minesweeper::displaySize(const QSize& disp) const
{
	if (disp.width() >= size().width() * MINUSIZE && disp.height() >= size().height() * MINUSIZE)
		return QSize(-1, -1);
	int us = MAX(MINUSIZE, disp.width() / size().width());
	us = MAX(us, disp.height() / size().height());
	return QSize(MAX(size().width() * us, disp.width()), MAX(size().height() * us, disp.height()));
}

QColor Minesweeper::idColor(int index) const
{
	if (index & UncoveredPosition)
		return idColors[Uncovered];
	return idColors[index];
}

void Minesweeper::drawHover(QPixmap *hover, QPainter *painter, const int player)
{
	// Background
	painter->setPen(NoPen);
	painter->setBrush(QBrush(idColor(Background)));
	painter->drawRect(hover->rect());

	// Context
	painter->setPen(NoPen);
	painter->setBrush(QBrush(idColor(Uncovered), Dense4Pattern));
	painter->drawRect(hover->rect());

	// Frame
	painter->setPen(black);
	painter->setBrush(NoBrush);
	painter->drawRect(hover->rect());
}

void Minesweeper::paintPoint(QPainter *painter, const QRect& rect, const int id)
{
	QPen pen(painter->pen());
	QBrush brush(painter->brush());

	if (id & FlaggedPosition) {
		painter->drawPixmap(rect.topLeft(), info.flag);
		goto frame;
	} else if (id & UncoveredPosition) {
		painter->drawRect(rect);
		goto frame;
	}

	// Background
	painter->setPen(NoPen);
	painter->setBrush(QBrush(idColor(Background)));
	painter->drawRect(rect);

	// Context
	if (id == Mine) {
		painter->drawPixmap(rect.topLeft(), info.mine);
		goto frame;
	} else if (id == Empty)
		goto frame;
	painter->setPen(brush.color());
	painter->setBrush(NoBrush);
	painter->setFont(QFont("noah", rect.width()));
	painter->drawText(rect, AlignCenter, QString::number(id));

frame:	// Frame
	painter->setPen(black);
	painter->setBrush(NoBrush);
	painter->drawRect(rect);
}

bool Minesweeper::mouseReleaseEvent(const ButtonState button, const QPoint& point, const int player)
{
	if (point == QPoint(-1, -1))
		return false;
	if (button != LeftButton && (DATA(point) & UncoveredPosition)) {
		DATA(point) ^= FlaggedPosition;
		return true;
	}
	if (point != data.mouse && data.mouse != QPoint(-1, -1) && (DATA(data.mouse) & UncoveredPosition)) {
		DATA(data.mouse) ^= FlaggedPosition;
		return true;
	}
	if (info.first)
		goto first;
	if (!(DATA(point) & UncoveredPosition) && DATA(point) == flagCount(point)) {
		for (int d = 1; d > -2; d -= 2) {
			QPoint direction;
			direction = QPoint(1, 0) * d;
			if (pointValid(point + direction))
				place(point + direction, player);
			direction = QPoint(1, 1) * d;
			if (pointValid(point + direction))
				place(point + direction, player);
			direction = QPoint(0, 1) * d;
			if (pointValid(point + direction))
				place(point + direction, player);
			direction = QPoint(-1, 1) * d;
			if (pointValid(point + direction))
				place(point + direction, player);
		}
		return true;
	}
	return place(point, player);
first:
	info.first = false;
	int cnt = (DATA(point) % UncoveredPosition == Mine) + mineCount(point);
	if (cnt != 0) {
		QPointArray spaces;
		for (int r = 0; r < size().height(); r++)
			for (int c = 0; c < size().width(); c++)
				if (data.pos[r][c] % UncoveredPosition != Mine)
					spaces.putPoints(spaces.size(), 1, c, r);
		if (DATA(point) % UncoveredPosition != Mine)
			spaces.at(spaces.find(point)).setX(-1);
		DATA(point) = UncoveredPosition;
		for (int d = 1; d > -2; d -= 2) {
			int i;
			QPoint direction, p;
			direction = QPoint(1, 0) * d;
			p = point + direction;
			if (pointValid(p)) {
				if ((i = spaces.find(p)) != -1)
					spaces.at(i).setX(-1);
				DATA(p) = UncoveredPosition;
			}
			direction = QPoint(1, 1) * d;
			p = point + direction;
			if (pointValid(p)) {
				if ((i = spaces.find(p)) != -1)
					spaces.at(i).setX(-1);
				DATA(p) = UncoveredPosition;
			}
			direction = QPoint(0, 1) * d;
			p = point + direction;
			if (pointValid(p)) {
				if ((i = spaces.find(p)) != -1)
					spaces.at(i).setX(-1);
				DATA(p) = UncoveredPosition;
			}
			direction = QPoint(-1, 1) * d;
			p = point + direction;
			if (pointValid(p)) {
				if ((i = spaces.find(p)) != -1)
					spaces.at(i).setX(-1);
				DATA(p) = UncoveredPosition;
			}
		}
		for (int i = 0; i < cnt; i++) {
			int index = rand() % spaces.count();
			while (spaces.at(index).x() == -1)
				index = rand() % spaces.count();
			QPoint p(spaces.at(index));
			spaces.at(index).setX(-1);
			DATA(p) |= Mine;
		}
	}
	return place(point, player);
}

bool Minesweeper::place(const QPoint& point, const int player)
{
	if (!(DATA(point) & UncoveredPosition) || (DATA(point) & FlaggedPosition))
		return false;
	if (DATA(point) % UncoveredPosition == Mine) {
		DATA(point) = Mine;
		info.died = true;
		return true;
	}
	int cnt = mineCount(point);
	DATA(point) = cnt;
	if (cnt != 0)
		return true;
	for (int d = 1; d > -2; d -= 2) {
		QPoint direction;
		direction = QPoint(1, 0) * d;
		if (pointValid(point + direction))
			place(point + direction, player);
		direction = QPoint(1, 1) * d;
		if (pointValid(point + direction))
			place(point + direction, player);
		direction = QPoint(0, 1) * d;
		if (pointValid(point + direction))
			place(point + direction, player);
		direction = QPoint(-1, 1) * d;
		if (pointValid(point + direction))
			place(point + direction, player);
	}
	return true;
}

int Minesweeper::mineCount(const QPoint& point) const
{
	int cnt = 0;
	for (int d = 1; d > -2; d -= 2) {
		QPoint direction;
		direction = QPoint(1, 0) * d;
		if (pointValid(point + direction))
			cnt += DATA(point + direction) % UncoveredPosition == Mine;
		direction = QPoint(1, 1) * d;
		if (pointValid(point + direction))
			cnt += DATA(point + direction) % UncoveredPosition == Mine;
		direction = QPoint(0, 1) * d;
		if (pointValid(point + direction))
			cnt += DATA(point + direction) % UncoveredPosition == Mine;
		direction = QPoint(-1, 1) * d;
		if (pointValid(point + direction))
			cnt += DATA(point + direction) % UncoveredPosition == Mine;
	}
	return cnt;
}

int Minesweeper::flagCount(const QPoint& point) const
{
	int cnt = 0;
	for (int d = 1; d > -2; d -= 2) {
		QPoint direction;
		direction = QPoint(1, 0) * d;
		if (pointValid(point + direction))
			cnt += DATA(point + direction) & FlaggedPosition ? 1 : 0;
		direction = QPoint(1, 1) * d;
		if (pointValid(point + direction))
			cnt += DATA(point + direction) & FlaggedPosition ? 1 : 0;
		direction = QPoint(0, 1) * d;
		if (pointValid(point + direction))
			cnt += DATA(point + direction) & FlaggedPosition ? 1 : 0;
		direction = QPoint(-1, 1) * d;
		if (pointValid(point + direction))
			cnt += DATA(point + direction) & FlaggedPosition ? 1 : 0;
	}
	return cnt;
}

int Minesweeper::win(void)
{
	if (info.died) {
		for (int r = 0; r < size().height(); r++)
			for (int c = 0; c < size().width(); c++) {
				data.pos[r][c] %= UncoveredPosition;
				if (data.pos[r][c] != Mine)
					data.pos[r][c] = mineCount(QPoint(c, r));
			}
		return -2;
	}
	int cnt = 0;
	for (int r = 0; r < size().height(); r++)
		for (int c = 0; c < size().width(); c++)
			cnt += (data.pos[r][c] & UncoveredPosition) ? 1 : 0;
	if (cnt == info.total)
		return 0;
	return -1;
}

MinesweeperSelector::MinesweeperSelector(QWidget *parent) : QDialog(parent, 0, true)
{
	QGridLayout *layout = new QGridLayout(this);
	layout->addMultiCellWidget(sWidth = new QSlider(9, 30, 5, 9, Horizontal, this), 0, 0, 0, 1);
	layout->addWidget(lWidth = new QLabel(tr("Width: 9"), this), 0, 2);
	layout->addMultiCellWidget(sHeight = new QSlider(9, 24, 5, 9, Horizontal, this), 1, 1, 0, 1);
	layout->addWidget(lHeight = new QLabel(tr("Height: 9"), this), 1, 2);
	layout->addMultiCellWidget(sMines = new QSlider(10, 64, 5, 10, Horizontal, this), 2, 2, 0, 1);
	layout->addWidget(lMines = new QLabel(tr("Mines: 10"), this), 2, 2);
	layout->addWidget(pbEasy = new QPushButton(tr("&Easy"), this), 3, 0);
	layout->addWidget(pbMedium = new QPushButton(tr("&Medium"), this), 3, 1);
	layout->addWidget(pbExpert = new QPushButton(tr("E&xpert"), this), 3, 2);
	layout->addMultiCellWidget(pbAccept = new QPushButton(tr("&Ok"), this), 4, 4, 0, 2);
	pbAccept->setDefault(true);
	connect(sWidth, SIGNAL(sliderMoved(int)), this, SLOT(sliderUpdate(int)));
	connect(sWidth, SIGNAL(valueChanged(int)), this, SLOT(sliderUpdate(int)));
	connect(sHeight, SIGNAL(sliderMoved(int)), this, SLOT(sliderUpdate(int)));
	connect(sHeight, SIGNAL(valueChanged(int)), this, SLOT(sliderUpdate(int)));
	connect(sMines, SIGNAL(sliderMoved(int)), this, SLOT(sliderUpdate(int)));
	connect(sMines, SIGNAL(valueChanged(int)), this, SLOT(sliderUpdate(int)));
	connect(pbEasy, SIGNAL(clicked()), this, SLOT(easy()));
	connect(pbMedium, SIGNAL(clicked()), this, SLOT(medium()));
	connect(pbExpert, SIGNAL(clicked()), this, SLOT(expert()));
	connect(pbAccept, SIGNAL(clicked()), this, SLOT(accept()));
	setMinimumWidth(240);
}

void MinesweeperSelector::sliderUpdate(int)
{
	lWidth->setText(QString(tr("Width: %1")).arg(sWidth->value()));
	lHeight->setText(QString(tr("Height: %1")).arg(sHeight->value()));
	sMines->setMaxValue((sWidth->value() - 1) * (sHeight->value() - 1));
	lMines->setText(QString(tr("Mines: %1")).arg(sMines->value()));
}

void MinesweeperSelector::easy(void)
{
	sWidth->setValue(9);
	sHeight->setValue(9);
	sMines->setValue(10);
}

void MinesweeperSelector::medium(void)
{
	sWidth->setValue(16);
	sHeight->setValue(16);
	sMines->setValue(40);
}

void MinesweeperSelector::expert(void)
{
	sWidth->setValue(30);
	sHeight->setValue(16);
	sMines->setValue(99);
}
