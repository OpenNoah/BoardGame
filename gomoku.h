#ifndef GOMOKU_H
#define GOMOKU_H

#include "boardgame.h"

class Gomoku : public BoardGame
{
	Q_OBJECT
public:
	Gomoku(QObject *parent) : BoardGame(parent) {}
	virtual void refresh(QPixmap *cache);
	virtual QPoint ai(const int player);

	// Game type information
	virtual QSize size(void) const {return QSize(15, 15);}
	virtual int playerCount(void) const {return 2;}
	virtual QString gameName(void) const {return tr("Gomoku");}
	virtual bool aiAvailable(void) const {return true;}

protected:
	// Game type informaion
	virtual int winLength(void) const {return 5;}
	virtual BoardTypes boardType(void) const {return CenterLine;}
	virtual QColor playerColor(int player) const {return playerColors[player];}
	virtual int specialColor(void) const {return 2;}

private:
	enum Players {Black = 0, White = 1, Special = 2};

	static const QColor playerColors[3];
};

#endif
