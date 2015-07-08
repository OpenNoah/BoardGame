#ifndef GOMOKU3_H
#define GOMOKU3_H

#include "gomoku.h"

class Gomoku3 : public Gomoku
{
	Q_OBJECT
public:
	Gomoku3(QObject *parent) : Gomoku(parent) {}

	// Game type information
	virtual int playerCount(void) const {return 3;}
	virtual QString gameName(void) const {return tr("Gomoku 3P");}
	virtual bool aiAvailable(void) const {return false;}

protected:
	// Game type informaion
	virtual int winLength(void) const {return 4;}
	virtual QColor playerColor(int player) const {return playerColors[player];}
	virtual int specialColor(void) const {return 3;}

private:
	enum Players {Red = 0, Green = 1, Blue = 2, Special = 3};

	static const QColor playerColors[4];
};

#endif
