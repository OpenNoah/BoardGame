#ifndef BOARDGAME_H
#define BOARDGAME_H

#include <qobject.h>
#include <qpixmap.h>
#include <qpainter.h>

#define COLOR_BOARD	QColor(255, 200, 100)
#define COLOR_LINE	QColor(100, 100, 100)

class BoardGame : public QObject
{
	Q_OBJECT
public:
	BoardGame(QObject *parent) : QObject(parent) {reset();}
	virtual void reset(void);
	virtual void refresh(QPixmap *cache);
	//virtual void generalRefresh(QPixmap *cache);
	virtual QPoint findPoint(const QSize& disp, const QPoint& pos);
	virtual QPoint hoverCoordinate(QPixmap *cache, const QPoint& point) {return pointRect(cache->size(), point).topLeft();}
	virtual void hoverPixmap(QPixmap *cache, QPixmap *hover, const int player);
	virtual bool pointValid(const QPoint& p) const {return p.x() >= 0 && p.x() < size().width() && p.y() >= 0 && p.y() < size().height();}
	virtual bool spaceAvailable(const int player) {return true;}
	virtual bool place(const QPoint& point, const int player);
	virtual QPoint ai(const int player) {return QPoint(-1, -1);}
	virtual int win(void);
	virtual bool mousePressEvent(const ButtonState button, const QPoint& point, const int player) {data.mouse = point; return true;}
	virtual bool mouseMoveEvent(const ButtonState button, const QPoint& point, const int player) {return true;}
	virtual bool mouseReleaseEvent(const ButtonState button, const QPoint& point, const int player) {return place(point, player);}

	// Game type information
	virtual QSize size(void) const {return QSize(1, 1);}
	virtual int playerCount(void) const {return -1;}
	virtual QString gameName(void) const {return tr("None");}
	virtual QString winMessage(const int player, const QString msg) const;
	virtual bool aiAvailable(void) const {return false;}
	virtual QSize displaySize(const QSize& disp) const {return QSize(-1, -1);}

protected:
	enum BoardTypes {NoLine, CenterLine, EdgeLine};

	virtual int count(const QPoint& point, const QPoint& direction, const int player) const;
	virtual QRect pointRect(const QSize& disp, const QPoint& point) const;
	virtual void paintPoint(QPainter *painter, const QRect& rect, const int player) {painter->drawEllipse(rect);}
	virtual void drawBackground(QPixmap *cache, QPainter *painter);
	virtual void drawBoardLines(QPixmap *cache, QPainter *painter);
	virtual void drawEdgeLines(QPixmap *cache, QPainter *painter);
	virtual void drawPoint(QPixmap *cache, QPainter *painter, const QPoint& point, const int player);
	virtual void drawLast(QPixmap *cache, QPainter *painter, const QPoint& point, const int player);
	virtual void drawHover(QPixmap *hover, QPainter *painter, const int player);

	// Game type information
	virtual int winLength(void) const {return -1;}
	virtual bool square(void) const {return size().width() == size().height();}
	virtual BoardTypes boardType(void) const {return NoLine;}
	virtual QColor playerColor(int player) const {return playerColors[player];}

	struct {
		int pos[64][64];
		QPoint last, mouse;
	} data;

private:
	enum Players {Black = 0, White = 1};

	static const QColor playerColors[2];
};

#endif
