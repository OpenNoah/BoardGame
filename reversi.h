#ifndef REVERSI_H
#define REVERSI_H

#include "boardgame.h"

class Reversi : public BoardGame
{
	Q_OBJECT
public:
	Reversi(QObject *parent) : BoardGame(parent) {}
	virtual void reset(void);
	//virtual void refresh(QPixmap *cache) {generalRefresh(cache);}
	virtual bool spaceAvailable(const int player) {return availableSpaces(player).size() != 0;}
	virtual bool place(const QPoint& point, const int player);
	virtual int win(void);
	virtual QPoint ai(const int player);

	// Game type information
	virtual QSize size(void) const {return QSize(8, 8);}
	virtual int playerCount(void) const {return 2;}
	virtual QString gameName(void) const {return tr("Reversi");}
	virtual bool aiAvailable(void) const {return true;}

protected:
	virtual bool available(const QPoint& point, const int player) const;
	virtual QPointArray availableSpaces(const int player) const;

	// Game type information
	virtual BoardTypes boardType(void) const {return EdgeLine;}

private:
	int check(const QPoint& point, const QPoint& direction, const int player) const;
	void reverse(const QPoint& point, const QPoint& direction, const int cnt, const int player);
	int reversible(const QPoint& point, const int player);

	enum Players {Black = 0, White = 1};
};

#endif
