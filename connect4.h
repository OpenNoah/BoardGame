#ifndef CONNECT4_H
#define CONNECT4_H

#include "boardgame.h"

class Connect4 : public BoardGame
{
	Q_OBJECT
public:
	Connect4(QObject *parent) : BoardGame(parent) {}
	virtual void refresh(QPixmap *cache);
	virtual bool place(const QPoint& point, const int player);

	// Game type information
	virtual QSize size(void) const {return QSize(7, 6);}
	virtual int playerCount(void) const {return 2;}
	virtual QString gameName(void) const {return tr("Connect Four");}
	virtual bool aiAvailable(void) const {return false;}

protected:
	// Game type information
	virtual int winLength(void) const {return 4;}
	virtual BoardTypes boardType(void) const {return EdgeLine;}
	virtual QColor playerColor(int player) const {return playerColors[player];}

private:
	enum Players {Yellow = 0, Red = 1};

	static const QColor playerColors[2];
};

#endif
